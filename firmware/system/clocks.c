/*
 * clocks.c
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
 * \brief System clocks functions implementation.
 *
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 *
 * \version 0.2.0 (Ported to STM32L4)
 *
 * \date 2020/10/22
 *
 * \addtogroup clocks
 * \{
 */

#include "stm32l476xx.h"
#include "clocks.h"

/* External functions */
extern void SystemClock_Config(void);
extern void SystemCoreClockUpdate(void);
extern uint32_t SystemCoreClock;

int clocks_setup(clocks_config_t clks)
{
    /* Note: For STM32L4, the clock configuration is done in SystemClock_Config()
     * which is called from main(). This function is kept for API compatibility
     * but the actual configuration is hardcoded to 80 MHz from PLL.
     *
     * Clock tree for STM32L476RG:
     * - MSI 4 MHz → PLL (×40 ÷ 2) → 80 MHz SYSCLK
     * - HCLK  = SYSCLK = 80 MHz (AHB)
     * - PCLK1 = HCLK/1 = 80 MHz (APB1)
     * - PCLK2 = HCLK/1 = 80 MHz (APB2)
     */

    /* Configure system clock to 80 MHz */
    SystemClock_Config();

    /* Update SystemCoreClock variable */
    SystemCoreClockUpdate();

    return 0;
}

clocks_config_t clocks_read()
{
    clocks_config_t clks;

    /* Update SystemCoreClock variable */
    SystemCoreClockUpdate();

    /* For STM32L4, map clocks to MSP430 equivalents:
     * - MCLK (Master Clock) → SYSCLK (System Clock)
     * - SMCLK (Sub-Master Clock) → PCLK1 (APB1 Clock)
     * - ACLK (Auxiliary Clock) → LSI or LSE (Low Speed Clock)
     */
    clks.mclk_hz = SystemCoreClock;         /* 80 MHz */
    clks.smclk_hz = SystemCoreClock;        /* 80 MHz (APB1 = AHB/1) */
    clks.aclk_hz = 32000;                   /* 32 kHz (LSI approximation) */

    return clks;
}

/**
 * \brief NMI Handler (Not used for clock failures on STM32L4)
 *
 * On STM32L4, clock failures are handled by the RCC interrupt (RCC_IRQn).
 * This NMI handler is kept for compatibility but does nothing.
 *
 * \return None.
 */
void NMI_Handler(void)
{
    /* For STM32L4, clock security system (CSS) can be enabled to detect
     * HSE failures. If enabled, it generates an NMI interrupt.
     * For now, we just trap here.
     */
    while(1)
    {
        /* Trap if NMI occurs */
    }
}

/** \} End of clocks group */
