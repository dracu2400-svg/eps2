/*
 * wdt.c
 *
 * Copyright (C) 2019, SpaceLab.
 *
 * This file is part of EPS 2.0.
 *
 * EPS 2.0 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * EPS 2.0 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with EPS 2.0. If not, see <http://www.gnu.org/licenses/>.
 *
 */

/**
 * \brief Internal watchdog timer driver implementation (Ported to STM32L4 IWDG).
 *
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 *
 * \version 0.3.0 (STM32L4 port)
 *
 * \date 2020/10/24
 *
 * \addtogroup wdt
 * \{
 */

#include "stm32l476xx.h"

#include <config/config.h>
#include <system/sys_log/sys_log.h>

#include "wdt.h"

/* IWDG Key register values */
#define IWDG_KEY_RELOAD     0xAAAA  /**< Reload counter */
#define IWDG_KEY_ENABLE     0xCCCC  /**< Enable IWDG */
#define IWDG_KEY_WRITE      0x5555  /**< Enable register write access */

/* IWDG Prescaler values (STM32L4) */
#define IWDG_PR_DIV4        0  /**< Prescaler /4 */
#define IWDG_PR_DIV8        1  /**< Prescaler /8 */
#define IWDG_PR_DIV16       2  /**< Prescaler /16 */
#define IWDG_PR_DIV32       3  /**< Prescaler /32 */
#define IWDG_PR_DIV64       4  /**< Prescaler /64 */
#define IWDG_PR_DIV128      5  /**< Prescaler /128 */
#define IWDG_PR_DIV256      6  /**< Prescaler /256 */

int wdt_init(wdt_config_t config)
{
    uint32_t prescaler = 0;
    uint32_t reload = 0;

    /* Note: STM32L4 IWDG is always clocked by LSI (~32 kHz)
     * Clock source parameter is ignored for compatibility
     */
    (void)config.clk_src;

    /* Map MSP430 clock divider to STM32 IWDG prescaler */
    /* For ~1 second timeout with 32 kHz LSI, we use prescaler /32 with reload ~1000 */
    switch(config.clk_div)
    {
        case WDT_CLK_DIV_32K:
            prescaler = IWDG_PR_DIV32;  /* /32 prescaler */
            reload = 1000;              /* ~1 second timeout: 32000 / 32 / 1000 = 1 Hz */
            break;
        case WDT_CLK_DIV_8192:
            prescaler = IWDG_PR_DIV8;   /* /8 prescaler */
            reload = 4095;              /* Max reload value: ~1 second */
            break;
        case WDT_CLK_DIV_512:
            prescaler = IWDG_PR_DIV4;   /* /4 prescaler */
            reload = 4095;              /* Max reload value: ~0.5 second */
            break;
        case WDT_CLK_DIV_64:
            prescaler = IWDG_PR_DIV4;   /* /4 prescaler */
            reload = 512;               /* ~64 ms timeout */
            break;
        case WDT_CLK_DIV_2G:         /* Very long timeout, use max values */
        case WDT_CLK_DIV_128M:
        case WDT_CLK_DIV_8192K:
        case WDT_CLK_DIV_512K:
            prescaler = IWDG_PR_DIV256; /* /256 prescaler */
            reload = 4095;              /* Max reload value: ~32 seconds */
            break;
        default:
        #if CONFIG_DRIVERS_DEBUG_ENABLED == 1
            sys_log_print_event_from_module(SYS_LOG_ERROR, WDT_MODULE_NAME, "Error during initialization: Invalid clock divider!");
            sys_log_new_line();
        #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
            return -1;      /* Invalid clock divider value */
    }

    /* Enable write access to IWDG_PR and IWDG_RLR registers */
    IWDG->KR = IWDG_KEY_WRITE;

    /* Set prescaler */
    IWDG->PR = prescaler;

    /* Set reload value (12-bit max = 4095) */
    IWDG->RLR = reload & 0xFFF;

    /* Reload counter */
    IWDG->KR = IWDG_KEY_RELOAD;

    /* Start watchdog (once started, cannot be stopped except by reset) */
    IWDG->KR = IWDG_KEY_ENABLE;

#if CONFIG_DRIVERS_DEBUG_ENABLED == 1
    sys_log_print_event_from_module(SYS_LOG_INFO, WDT_MODULE_NAME, "IWDG initialized with prescaler ");
    sys_log_print_uint(prescaler);
    sys_log_print_msg(" and reload ");
    sys_log_print_uint(reload);
    sys_log_new_line();
#endif /* CONFIG_DRIVERS_DEBUG_ENABLED */

    return 0;
}

void wdt_reset(void)
{
    /* Reload watchdog counter */
    IWDG->KR = IWDG_KEY_RELOAD;
}

/** \} End of wdt group */
