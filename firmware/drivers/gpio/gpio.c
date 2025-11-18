/*
 * gpio.c
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
 * \brief GPIO driver implementation (Ported to STM32L4).
 *
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 *
 * \version 0.2.0 (STM32L4 port)
 *
 * \date 2020/10/24
 *
 * \addtogroup gpio
 * \{
 */

#include "stm32l476xx.h"

#include <config/config.h>
#include <system/sys_log/sys_log.h>

#include "gpio.h"

/**
 * \brief STM32 pin mapping structure.
 */
typedef struct {
    GPIO_TypeDef *port;     /**< GPIO port (GPIOA, GPIOB, GPIOC). */
    uint8_t pin;            /**< Pin number (0-15). */
} stm32_pin_t;

/**
 * \brief GPIO pin mapping table (GPIO_PIN_x → STM32 port/pin).
 *
 * Based on STM32_PIN_MAPPING.md:
 * - GPIO_PIN_0-7:   PA0-PA7
 * - GPIO_PIN_8-15:  PB0-PB7
 * - GPIO_PIN_16-23: PC0-PC7
 * - GPIO_PIN_24-31: PA8-PA15
 * - GPIO_PIN_32+:   Not mapped (reserved for future expansion)
 */
static const stm32_pin_t pin_map[] = {
    [GPIO_PIN_0]  = {GPIOA, 0},   /* PA0  - LED_STATUS */
    [GPIO_PIN_1]  = {GPIOA, 1},   /* PA1  - UART_TX2 */
    [GPIO_PIN_2]  = {GPIOA, 2},   /* PA2  - UART_RX2 */
    [GPIO_PIN_3]  = {GPIOA, 3},   /* PA3  - ADC_IN3 */
    [GPIO_PIN_4]  = {GPIOA, 4},   /* PA4  - SPI1_NSS */
    [GPIO_PIN_5]  = {GPIOA, 5},   /* PA5  - SPI1_SCK */
    [GPIO_PIN_6]  = {GPIOA, 6},   /* PA6  - SPI1_MISO */
    [GPIO_PIN_7]  = {GPIOA, 7},   /* PA7  - SPI1_MOSI */
    [GPIO_PIN_8]  = {GPIOB, 0},   /* PB0  - ADC_IN15 */
    [GPIO_PIN_9]  = {GPIOB, 1},   /* PB1  - ADC_IN16 */
    [GPIO_PIN_10] = {GPIOB, 2},   /* PB2  - GPIO */
    [GPIO_PIN_11] = {GPIOB, 3},   /* PB3  - SPI2_NSS */
    [GPIO_PIN_12] = {GPIOB, 4},   /* PB4  - SPI2_MISO */
    [GPIO_PIN_13] = {GPIOB, 5},   /* PB5  - SPI2_MOSI */
    [GPIO_PIN_14] = {GPIOB, 6},   /* PB6  - UART_TX1 */
    [GPIO_PIN_15] = {GPIOB, 7},   /* PB7  - UART_RX1 */
    [GPIO_PIN_16] = {GPIOB, 8},   /* PB8  - I2C1_SCL */
    [GPIO_PIN_17] = {GPIOB, 9},   /* PB9  - I2C1_SDA */
    [GPIO_PIN_18] = {GPIOB, 10},  /* PB10 - UART_TX3 */
    [GPIO_PIN_19] = {GPIOB, 11},  /* PB11 - UART_RX3 */
    [GPIO_PIN_20] = {GPIOB, 12},  /* PB12 - SPI2_NSS_ALT */
    [GPIO_PIN_21] = {GPIOB, 13},  /* PB13 - SPI2_SCK */
    [GPIO_PIN_22] = {GPIOB, 14},  /* PB14 - GPIO */
    [GPIO_PIN_23] = {GPIOB, 15},  /* PB15 - GPIO */
    [GPIO_PIN_24] = {GPIOC, 0},   /* PC0  - ADC_IN1 */
    [GPIO_PIN_25] = {GPIOC, 1},   /* PC1  - ADC_IN2 */
    [GPIO_PIN_26] = {GPIOC, 2},   /* PC2  - ADC_IN3 */
    [GPIO_PIN_27] = {GPIOC, 3},   /* PC3  - ADC_IN4 */
    [GPIO_PIN_28] = {GPIOC, 4},   /* PC4  - GPIO */
    [GPIO_PIN_29] = {GPIOC, 5},   /* PC5  - GPIO */
    [GPIO_PIN_30] = {GPIOC, 6},   /* PC6  - GPIO */
    [GPIO_PIN_31] = {GPIOC, 7},   /* PC7  - GPIO */
};

#define PIN_MAP_SIZE    (sizeof(pin_map) / sizeof(pin_map[0]))

/**
 * \brief Enables the clock for a given GPIO port.
 *
 * \param[in] port is the GPIO port to enable clock for.
 *
 * \return None.
 */
static void gpio_enable_port_clock(GPIO_TypeDef *port)
{
    if (port == GPIOA)
    {
        RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
    }
    else if (port == GPIOB)
    {
        RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
    }
    else if (port == GPIOC)
    {
        RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
    }
}

int gpio_init(gpio_pin_t pin, gpio_config_t config)
{
    /* Validate pin number */
    if (pin >= PIN_MAP_SIZE)
    {
    #if CONFIG_DRIVERS_DEBUG_ENABLED == 1
        sys_log_print_event_from_module(SYS_LOG_ERROR, GPIO_MODULE_NAME, "Invalid pin to initialize!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        return -1;  /* Invalid GPIO pin */
    }

    GPIO_TypeDef *port = pin_map[pin].port;
    uint8_t pin_num = pin_map[pin].pin;

    /* Enable GPIO port clock */
    gpio_enable_port_clock(port);

    /* Configure pin mode in MODER register */
    /* MODER: 00 = Input, 01 = Output, 10 = Alternate function, 11 = Analog */
    uint32_t moder = port->MODER;
    moder &= ~(0x3UL << (pin_num * 2));  /* Clear mode bits */

    if (config.mode == GPIO_MODE_OUTPUT_PIN)
    {
        moder |= (0x1UL << (pin_num * 2));  /* Set to output mode (01) */
        port->MODER = moder;

        /* Configure output type as push-pull (default) */
        port->OTYPER &= ~(1UL << pin_num);  /* 0 = Push-pull */

        /* Set output speed to medium (default) */
        uint32_t ospeedr = port->OSPEEDR;
        ospeedr &= ~(0x3UL << (pin_num * 2));
        ospeedr |= (0x1UL << (pin_num * 2));  /* 01 = Medium speed */
        port->OSPEEDR = ospeedr;

        /* No pull-up/pull-down (default) */
        uint32_t pupdr = port->PUPDR;
        pupdr &= ~(0x3UL << (pin_num * 2));  /* 00 = No pull-up/pull-down */
        port->PUPDR = pupdr;
    }
    else if (config.mode == GPIO_MODE_INPUT_PIN)
    {
        moder |= (0x0UL << (pin_num * 2));  /* Set to input mode (00) */
        port->MODER = moder;

        /* Configure pull-up/pull-down as floating (default) */
        uint32_t pupdr = port->PUPDR;
        pupdr &= ~(0x3UL << (pin_num * 2));  /* 00 = No pull-up/pull-down */
        port->PUPDR = pupdr;
    }
    else
    {
    #if CONFIG_DRIVERS_DEBUG_ENABLED == 1
        sys_log_print_event_from_module(SYS_LOG_ERROR, GPIO_MODULE_NAME, "Invalid mode during the initialization!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        return -1;  /* Invalid mode */
    }

    return 0;
}

int gpio_set_state(gpio_pin_t pin, bool level)
{
    /* Validate pin number */
    if (pin >= PIN_MAP_SIZE)
    {
    #if CONFIG_DRIVERS_DEBUG_ENABLED == 1
        sys_log_print_event_from_module(SYS_LOG_ERROR, GPIO_MODULE_NAME, "Invalid pin to set!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        return -1;  /* Invalid GPIO pin */
    }

    GPIO_TypeDef *port = pin_map[pin].port;
    uint8_t pin_num = pin_map[pin].pin;

    /* Use BSRR register for atomic bit set/reset */
    /* BSRR bits [15:0] = set bits, bits [31:16] = reset bits */
    if (level)
    {
        port->BSRR = (1UL << pin_num);  /* Set bit */
    }
    else
    {
        port->BSRR = (1UL << (pin_num + 16));  /* Reset bit */
    }

    return 0;
}

int gpio_get_state(gpio_pin_t pin)
{
    /* Validate pin number */
    if (pin >= PIN_MAP_SIZE)
    {
    #if CONFIG_DRIVERS_DEBUG_ENABLED == 1
        sys_log_print_event_from_module(SYS_LOG_ERROR, GPIO_MODULE_NAME, "Invalid pin to read!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        return -1;  /* Invalid GPIO pin */
    }

    GPIO_TypeDef *port = pin_map[pin].port;
    uint8_t pin_num = pin_map[pin].pin;

    /* Read pin state from IDR (Input Data Register) */
    if (port->IDR & (1UL << pin_num))
    {
        return 1;  /* Pin is HIGH */
    }
    else
    {
        return 0;  /* Pin is LOW */
    }
}

int gpio_toggle(gpio_pin_t pin)
{
    /* Validate pin number */
    if (pin >= PIN_MAP_SIZE)
    {
    #if CONFIG_DRIVERS_DEBUG_ENABLED == 1
        sys_log_print_event_from_module(SYS_LOG_ERROR, GPIO_MODULE_NAME, "Invalid pin to toggle!");
        sys_log_new_line();
    #endif /* CONFIG_DRIVERS_DEBUG_ENABLED */
        return -1;  /* Invalid GPIO pin */
    }

    GPIO_TypeDef *port = pin_map[pin].port;
    uint8_t pin_num = pin_map[pin].pin;

    /* Toggle pin state using ODR register */
    port->ODR ^= (1UL << pin_num);

    return 0;
}

/** \} End of gpio group */
