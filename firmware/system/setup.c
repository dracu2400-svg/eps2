/*
 * setup.c
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
 * \brief System setup implementation (Ported to STM32L4).
 *
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 *
 * \version 0.2.0 (STM32L4 port)
 *
 * \date 2020/10/22
 *
 * \defgroup setup System setup
 * \ingroup system
 * \{
 */

#include <FreeRTOS.h>
#include <task.h>

/*
 * Note: For ARM Cortex-M4F, the FreeRTOS tick timer (SysTick) is configured
 * automatically in the FreeRTOS ARM_CM4F port (freertos/portable/GCC/ARM_CM4F/port.c).
 *
 * The vApplicationSetupTimerInterrupt() callback is specific to the MSP430 port
 * and is NOT used by the ARM Cortex-M port.
 *
 * The ARM_CM4F port uses vPortSetupTimerInterrupt() internally, which configures
 * SysTick to generate interrupts at configTICK_RATE_HZ (1000 Hz by default).
 *
 * SysTick configuration:
 * - Clock source: SYSCLK (80 MHz)
 * - Tick rate: 1000 Hz (1 ms per tick)
 * - SysTick reload value: (80,000,000 / 1000) - 1 = 79,999
 *
 * No additional timer setup is required for STM32L4.
 */

/** \} End of setup group */
