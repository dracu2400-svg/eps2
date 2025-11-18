/*
 * gpio.h - MSP430 GPIO HAL Compatibility Stub
 * 
 * Copyright (C) 2019, SpaceLab.
 * 
 * This file is part of EPS 2.0.
 */

#ifndef HAL_GPIO_H_
#define HAL_GPIO_H_

#include <stdint.h>

/* Port definitions (stub) */
#define GPIO_PORT_P1    1
#define GPIO_PORT_P2    2
#define GPIO_PORT_P3    3
#define GPIO_PORT_P4    4
#define GPIO_PORT_P5    5
#define GPIO_PORT_P6    6

/* Pin definitions */
#define GPIO_PIN0       (0x0001)
#define GPIO_PIN1       (0x0002)
#define GPIO_PIN2       (0x0004)
#define GPIO_PIN3       (0x0008)
#define GPIO_PIN4       (0x0010)
#define GPIO_PIN5       (0x0020)
#define GPIO_PIN6       (0x0040)
#define GPIO_PIN7       (0x0080)
#define GPIO_PIN_ALL8   (0x00FF)

/* Direction definitions */
#define GPIO_INPUT      0
#define GPIO_OUTPUT     1

/* Drive strength */
#define GPIO_FULL_OUTPUT_DRIVE_STRENGTH     0
#define GPIO_REDUCED_OUTPUT_DRIVE_STRENGTH  1

/* Stub function prototypes - to be implemented with STM32 GPIO */
static inline void GPIO_setAsOutputPin(uint8_t port, uint16_t pin) { /* Stub */ }
static inline void GPIO_setAsInputPin(uint8_t port, uint16_t pin) { /* Stub */ }
static inline void GPIO_setAsInputPinWithPullDownResistor(uint8_t port, uint16_t pin) { /* Stub */ }
static inline void GPIO_setAsInputPinWithPullUpResistor(uint8_t port, uint16_t pin) { /* Stub */ }
static inline void GPIO_setOutputHighOnPin(uint8_t port, uint16_t pin) { /* Stub */ }
static inline void GPIO_setOutputLowOnPin(uint8_t port, uint16_t pin) { /* Stub */ }
static inline void GPIO_toggleOutputOnPin(uint8_t port, uint16_t pin) { /* Stub */ }
static inline uint8_t GPIO_getInputPinValue(uint8_t port, uint16_t pin) { return 0; }
static inline void GPIO_setDriveStrength(uint8_t port, uint16_t pin, uint8_t strength) { /* Stub */ }

#endif /* HAL_GPIO_H_ */
