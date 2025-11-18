/*
 * system.c
 *
 * Copyright (C) 2020, SpaceLab.
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
 * \brief System management routines implementation (Ported to STM32L4).
 *
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 *
 * \version 0.2.0 (STM32L4 port)
 *
 * \date 2020/10/22
 *
 * \addtogroup system
 * \{
 */

#include "stm32l476xx.h"
#include "core_cm4.h"
#include <drivers/gpio/gpio.h>

#include "system.h"

sys_time_t sys_time = 0;

void system_reset(void)
{
    /* Perform system reset using NVIC (CMSIS function) */
    NVIC_SystemReset();

    /* Never reached */
    while(1);
}

uint8_t system_get_reset_cause(void)
{
    /* Read reset flags from RCC Control/Status Register (CSR) */
    uint32_t csr = RCC->CSR;
    uint8_t reset_cause = 0;

    /* Map STM32L4 reset flags to a simple cause code
     * RCC_CSR reset flags (bits 31-24):
     * - Bit 31: LPWRRSTF (Low-power reset flag)
     * - Bit 30: WWDGRSTF (Window watchdog reset flag)
     * - Bit 29: IWDGRSTF (Independent watchdog reset flag)
     * - Bit 28: SFTRSTF  (Software reset flag)
     * - Bit 27: BORRSTF  (BOR reset flag)
     * - Bit 26: PINRSTF  (Pin reset flag)
     * - Bit 25: OBLRSTF  (Option byte loader reset flag)
     * - Bit 24: FWRSTF   (Firewall reset flag)
     */

    if (csr & (1UL << 29))  /* IWDGRSTF */
    {
        reset_cause = 1;  /* Independent watchdog reset */
    }
    else if (csr & (1UL << 30))  /* WWDGRSTF */
    {
        reset_cause = 2;  /* Window watchdog reset */
    }
    else if (csr & (1UL << 28))  /* SFTRSTF */
    {
        reset_cause = 3;  /* Software reset */
    }
    else if (csr & (1UL << 27))  /* BORRSTF */
    {
        reset_cause = 4;  /* Brown-out reset */
    }
    else if (csr & (1UL << 26))  /* PINRSTF */
    {
        reset_cause = 5;  /* External pin reset */
    }
    else if (csr & (1UL << 31))  /* LPWRRSTF */
    {
        reset_cause = 6;  /* Low-power reset */
    }
    else
    {
        reset_cause = 0;  /* Unknown */
    }

    /* Clear reset flags by setting RMVF bit (bit 23) */
    RCC->CSR |= (1UL << 23);

    return reset_cause;
}

void system_set_time(sys_time_t tm)
{
    sys_time = tm;
}

void system_increment_time(void)
{
    sys_time++;
}

sys_time_t system_get_time(void)
{
    return sys_time;
}

sys_hw_version_t system_get_hw_version()
{
    gpio_pin_t bit_0_pin = GPIO_PIN_20;
    gpio_pin_t bit_1_pin = GPIO_PIN_21;

    /* Initializing bit 0 GPIO pin */
    if (gpio_init(bit_0_pin, (gpio_config_t){.mode=GPIO_MODE_INPUT}) != 0)
    {
        return HW_VERSION_UNKNOWN;
    }

    /* Initializing bit 1 GPIO pin */
    if (gpio_init(bit_1_pin, (gpio_config_t){.mode=GPIO_MODE_INPUT}) != 0)
    {
        return HW_VERSION_UNKNOWN;
    }

    int bit_0_state = gpio_get_state(bit_0_pin);
    int bit_1_state = gpio_get_state(bit_1_pin);

    if ((bit_0_state == -1) || (bit_1_state == -1))
    {
        return HW_VERSION_UNKNOWN;
    }

    return ((uint8_t)bit_1_state << 1) | (uint8_t)bit_0_state;
}

/** \} End of system group */
