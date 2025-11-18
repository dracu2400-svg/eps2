/*
 * main.c
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
 * \brief Main file.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.0
 * 
 * \date 2020/10/21
 * 
 * \defgroup main Main file
 * \{
 */

#include <FreeRTOS.h>
#include <task.h>

#include "stm32l476xx.h"
#include "devices/watchdog/watchdog.h"
#include "system/clocks.h"
#include "app/tasks/tasks.h"

/* External functions from system_stm32l4xx.c */
extern void SystemInit(void);
extern void SystemClock_Config(void);

int main(void)
{
    /* STM32L4 system initialization (done in startup already, but called for completeness) */
    SystemInit();

    /* Configure system clock to 80 MHz */
    SystemClock_Config();

    /* Watchdog device initialization */
    watchdog_init();

    /* System clocks setup (for compatibility with existing API) */
    clocks_setup((clocks_config_t){.mclk_hz = 80000000UL, .smclk_hz=80000000UL, .aclk_hz=32000});

    /* Create all the tasks */
    create_tasks();

    /* Start the FreeRTOS scheduler */
    vTaskStartScheduler();

    /* Will only get here if there was insufficient memory to create the idle and/or timer task */
    while(1)
    {
        /* Trap here if scheduler fails */
    }

    return 0;  /* Never reached */
}

/** \} End of main group */
