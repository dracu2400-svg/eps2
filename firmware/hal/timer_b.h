/*
 * timer_b.h - MSP430 Timer_B HAL Compatibility Stub
 * 
 * Copyright (C) 2019, SpaceLab.
 */

#ifndef HAL_TIMER_B_H_
#define HAL_TIMER_B_H_

#include <stdint.h>

/* Base addresses */
#define TIMER_B0_BASE   0x0000
#define TIMER_B1_BASE   0x0001

/* Timer_B is functionally similar to Timer_A with some extensions */
/* For simplicity, reuse Timer_A definitions */

#include "timer_a.h"

/* Timer_B specific additions */
#define TIMER_B_CLOCKSOURCE_DIVIDER_1   TIMER_A_CLOCKSOURCE_DIVIDER_1
#define TIMER_B_CLOCKSOURCE_DIVIDER_2   TIMER_A_CLOCKSOURCE_DIVIDER_2
#define TIMER_B_CLOCKSOURCE_DIVIDER_4   TIMER_A_CLOCKSOURCE_DIVIDER_4
#define TIMER_B_CLOCKSOURCE_DIVIDER_8   TIMER_A_CLOCKSOURCE_DIVIDER_8

/* Stub function prototypes (similar to Timer_A) */
typedef Timer_A_initUpModeParam Timer_B_initUpModeParam;
typedef Timer_A_initContinuousModeParam Timer_B_initContinuousModeParam;
typedef Timer_A_initCompareModeParam Timer_B_initCompareModeParam;

static inline void Timer_B_initUpMode(uint16_t baseAddress, Timer_B_initUpModeParam *param) { /* Stub */ }
static inline void Timer_B_initContinuousMode(uint16_t baseAddress, Timer_B_initContinuousModeParam *param) { /* Stub */ }
static inline void Timer_B_initCompareMode(uint16_t baseAddress, Timer_B_initCompareModeParam *param) { /* Stub */ }
static inline void Timer_B_startCounter(uint16_t baseAddress, uint16_t timerMode) { /* Stub */ }
static inline void Timer_B_stop(uint16_t baseAddress) { /* Stub */ }

#endif /* HAL_TIMER_B_H_ */
