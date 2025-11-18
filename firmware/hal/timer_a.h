/*
 * timer_a.h - MSP430 Timer_A HAL Compatibility Stub
 * 
 * Copyright (C) 2019, SpaceLab.
 */

#ifndef HAL_TIMER_A_H_
#define HAL_TIMER_A_H_

#include <stdint.h>

/* Base addresses */
#define TIMER_A0_BASE   0x0000
#define TIMER_A1_BASE   0x0001
#define TIMER_A2_BASE   0x0002

/* Clock sources */
#define TIMER_A_CLOCKSOURCE_EXTERNAL_TXCLK  0
#define TIMER_A_CLOCKSOURCE_ACLK            1
#define TIMER_A_CLOCKSOURCE_SMCLK           2
#define TIMER_A_CLOCKSOURCE_INVERTED_EXTERNAL_TXCLK 3

/* Clock dividers */
#define TIMER_A_CLOCKSOURCE_DIVIDER_1   0
#define TIMER_A_CLOCKSOURCE_DIVIDER_2   1
#define TIMER_A_CLOCKSOURCE_DIVIDER_4   2
#define TIMER_A_CLOCKSOURCE_DIVIDER_8   3
#define TIMER_A_CLOCKSOURCE_DIVIDER_12  4
#define TIMER_A_CLOCKSOURCE_DIVIDER_16  5
#define TIMER_A_CLOCKSOURCE_DIVIDER_24  6
#define TIMER_A_CLOCKSOURCE_DIVIDER_32  7

/* Timer modes */
#define TIMER_A_STOP_MODE       0
#define TIMER_A_UP_MODE         1
#define TIMER_A_CONTINUOUS_MODE 2
#define TIMER_A_UPDOWN_MODE     3

/* Capture/Compare modes */
#define TIMER_A_CAPTURECOMPARE_REGISTER_0   0
#define TIMER_A_CAPTURECOMPARE_REGISTER_1   1
#define TIMER_A_CAPTURECOMPARE_REGISTER_2   2
#define TIMER_A_CAPTURECOMPARE_REGISTER_3   3
#define TIMER_A_CAPTURECOMPARE_REGISTER_4   4

/* Output modes */
#define TIMER_A_OUTPUTMODE_OUTBITVALUE  0
#define TIMER_A_OUTPUTMODE_SET          1
#define TIMER_A_OUTPUTMODE_TOGGLE_RESET 2
#define TIMER_A_OUTPUTMODE_SET_RESET    3
#define TIMER_A_OUTPUTMODE_TOGGLE       4
#define TIMER_A_OUTPUTMODE_RESET        5
#define TIMER_A_OUTPUTMODE_TOGGLE_SET   6
#define TIMER_A_OUTPUTMODE_RESET_SET    7

/* Interrupts */
#define TIMER_A_TAIE_INTERRUPT_ENABLE       0x02
#define TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE  0x01
#define TIMER_A_CAPTURECOMPARE_INTERRUPT_FLAG   0x01

/* Timer init parameter structures */
typedef struct {
    uint16_t clockSource;
    uint16_t clockSourceDivider;
    uint16_t timerPeriod;
    uint16_t timerInterruptEnable_TAIE;
    uint16_t captureCompareInterruptEnable_CCR0_CCIE;
    uint16_t timerClear;
    uint8_t startTimer;
} Timer_A_initUpModeParam;

typedef struct {
    uint16_t clockSource;
    uint16_t clockSourceDivider;
    uint16_t timerInterruptEnable_TAIE;
    uint16_t timerClear;
    uint8_t startTimer;
} Timer_A_initContinuousModeParam;

typedef struct {
    uint16_t compareRegister;
    uint16_t compareInterruptEnable;
    uint16_t compareOutputMode;
    uint16_t compareValue;
} Timer_A_initCompareModeParam;

/* Stub function prototypes */
static inline void Timer_A_initUpMode(uint16_t baseAddress, Timer_A_initUpModeParam *param) { /* Stub */ }
static inline void Timer_A_initContinuousMode(uint16_t baseAddress, Timer_A_initContinuousModeParam *param) { /* Stub */ }
static inline void Timer_A_initCompareMode(uint16_t baseAddress, Timer_A_initCompareModeParam *param) { /* Stub */ }
static inline void Timer_A_startCounter(uint16_t baseAddress, uint16_t timerMode) { /* Stub */ }
static inline void Timer_A_stop(uint16_t baseAddress) { /* Stub */ }
static inline void Timer_A_clear(uint16_t baseAddress) { /* Stub */ }
static inline void Timer_A_enableInterrupt(uint16_t baseAddress) { /* Stub */ }
static inline void Timer_A_disableInterrupt(uint16_t baseAddress) { /* Stub */ }
static inline void Timer_A_clearTimerInterrupt(uint16_t baseAddress) { /* Stub */ }
static inline void Timer_A_clearCaptureCompareInterrupt(uint16_t baseAddress, uint16_t captureCompareRegister) { /* Stub */ }
static inline void Timer_A_setCompareValue(uint16_t baseAddress, uint16_t compareRegister, uint16_t compareValue) { /* Stub */ }

#endif /* HAL_TIMER_A_H_ */
