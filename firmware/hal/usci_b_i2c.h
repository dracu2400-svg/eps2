/*
 * usci_b_i2c.h - MSP430 USCI_B I2C HAL Compatibility Stub
 * 
 * Copyright (C) 2019, SpaceLab.
 */

#ifndef HAL_USCI_B_I2C_H_
#define HAL_USCI_B_I2C_H_

#include <stdint.h>

/* Base addresses */
#define USCI_B0_BASE    0x0000
#define USCI_B1_BASE    0x0001
#define USCI_B2_BASE    0x0002

/* I2C modes */
#define USCI_B_I2C_TRANSMIT_MODE    0
#define USCI_B_I2C_RECEIVE_MODE     1

/* Data rates */
#define USCI_B_I2C_SET_DATA_RATE_100KBPS    100000
#define USCI_B_I2C_SET_DATA_RATE_400KBPS    400000

/* Interrupts */
#define USCI_B_I2C_RECEIVE_INTERRUPT0       0x01
#define USCI_B_I2C_TRANSMIT_INTERRUPT0      0x02
#define USCI_B_I2C_NAK_INTERRUPT            0x04
#define USCI_B_I2C_ARBITRATIONLOST_INTERRUPT 0x08
#define USCI_B_I2C_STOP_INTERRUPT           0x10
#define USCI_B_I2C_START_INTERRUPT          0x20

/* I2C init parameter structure */
typedef struct {
    uint8_t selectClockSource;
    uint32_t i2cClk;
    uint32_t dataRate;
    uint8_t byteCounterThreshold;
    uint8_t autoSTOPGeneration;
} USCI_B_I2C_initMasterParam;

/* Stub function prototypes */
static inline void USCI_B_I2C_initMaster(uint16_t baseAddress, USCI_B_I2C_initMasterParam *param) { /* Stub */ }
static inline void USCI_B_I2C_enable(uint16_t baseAddress) { /* Stub */ }
static inline void USCI_B_I2C_disable(uint16_t baseAddress) { /* Stub */ }
static inline void USCI_B_I2C_setSlaveAddress(uint16_t baseAddress, uint8_t slaveAddress) { /* Stub */ }
static inline void USCI_B_I2C_setMode(uint16_t baseAddress, uint8_t mode) { /* Stub */ }
static inline void USCI_B_I2C_enableInterrupt(uint16_t baseAddress, uint8_t mask) { /* Stub */ }
static inline void USCI_B_I2C_disableInterrupt(uint16_t baseAddress, uint8_t mask) { /* Stub */ }
static inline void USCI_B_I2C_masterSendStart(uint16_t baseAddress) { /* Stub */ }
static inline void USCI_B_I2C_masterSendMultiByteStart(uint16_t baseAddress, uint8_t txData) { /* Stub */ }
static inline void USCI_B_I2C_masterSendMultiByteNext(uint16_t baseAddress, uint8_t txData) { /* Stub */ }
static inline void USCI_B_I2C_masterSendMultiByteStop(uint16_t baseAddress) { /* Stub */ }
static inline uint8_t USCI_B_I2C_masterReceiveSingleByte(uint16_t baseAddress) { return 0; }
static inline uint8_t USCI_B_I2C_masterReceiveMultiByteNext(uint16_t baseAddress) { return 0; }
static inline void USCI_B_I2C_clearInterrupt(uint16_t baseAddress, uint8_t mask) { /* Stub */ }
static inline uint8_t USCI_B_I2C_getInterruptStatus(uint16_t baseAddress, uint8_t mask) { return 0; }

#endif /* HAL_USCI_B_I2C_H_ */
