/*
 * ref.h - MSP430 REF (Voltage Reference) HAL Compatibility Stub
 * 
 * Copyright (C) 2019, SpaceLab.
 */

#ifndef HAL_REF_H_
#define HAL_REF_H_

#include <stdint.h>

/* Base address */
#define REF_BASE    0x0000

/* Reference voltages */
#define REF_VREF1_2V    0
#define REF_VREF1_5V    1
#define REF_VREF2_0V    2
#define REF_VREF2_5V    3

/* Stub function prototypes */
static inline void REF_setReferenceVoltage(uint16_t baseAddress, uint8_t referenceVoltageSelect) { /* Stub */ }
static inline void REF_enableReferenceVoltage(uint16_t baseAddress) { /* Stub */ }
static inline void REF_disableReferenceVoltage(uint16_t baseAddress) { /* Stub */ }
static inline void REF_enableTempSensor(uint16_t baseAddress) { /* Stub */ }
static inline void REF_disableTempSensor(uint16_t baseAddress) { /* Stub */ }
static inline uint8_t REF_isRefGenBusy(uint16_t baseAddress) { return 0; }

#endif /* HAL_REF_H_ */
