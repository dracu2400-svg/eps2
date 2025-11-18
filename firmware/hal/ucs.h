/*
 * ucs.h - MSP430 UCS (Unified Clock System) HAL Compatibility Stub
 * 
 * Copyright (C) 2019, SpaceLab.
 */

#ifndef HAL_UCS_H_
#define HAL_UCS_H_

#include <stdint.h>

/* Base address */
#define UCS_BASE    0x0000

/* Clock sources */
#define UCS_XT1CLK_SELECT   0
#define UCS_VLOCLK_SELECT   1
#define UCS_REFOCLK_SELECT  2
#define UCS_DCOCLK_SELECT   3
#define UCS_DCOCLKDIV_SELECT    4
#define UCS_XT2CLK_SELECT   5

/* Dividers */
#define UCS_CLOCK_DIVIDER_1     0
#define UCS_CLOCK_DIVIDER_2     1
#define UCS_CLOCK_DIVIDER_4     2
#define UCS_CLOCK_DIVIDER_8     3
#define UCS_CLOCK_DIVIDER_16    4
#define UCS_CLOCK_DIVIDER_32    5

/* Clock signals */
#define UCS_ACLK    0
#define UCS_MCLK    1
#define UCS_SMCLK   2
#define UCS_FLLREF  3

/* Stub function prototypes */
static inline void UCS_initClockSignal(uint8_t selectedClockSignal, uint16_t clockSource, uint16_t clockSourceDivider) { /* Stub */ }
static inline void UCS_clockSignalInit(uint8_t selectedClockSignal, uint16_t clockSource, uint16_t clockSourceDivider) { /* Stub */ }
static inline uint32_t UCS_getMCLK(void) { return 80000000; }
static inline uint32_t UCS_getSMCLK(void) { return 80000000; }
static inline uint32_t UCS_getACLK(void) { return 32000; }

#endif /* HAL_UCS_H_ */
