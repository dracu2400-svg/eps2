/*
 * usci_a_spi.h - MSP430 USCI_A SPI HAL Compatibility Stub
 * 
 * Copyright (C) 2019, SpaceLab.
 */

#ifndef HAL_USCI_A_SPI_H_
#define HAL_USCI_A_SPI_H_

#include <stdint.h>

/* Base addresses */
#define USCI_A0_BASE    0x0000
#define USCI_A1_BASE    0x0001

/* SPI modes */
#define USCI_A_SPI_MSB_FIRST    0
#define USCI_A_SPI_LSB_FIRST    1

/* Clock phase/polarity */
#define USCI_A_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT     0
#define USCI_A_SPI_PHASE_DATA_CAPTURED_ONFIRST_CHANGED_ON_NEXT     1
#define USCI_A_SPI_CLOCKPOLARITY_INACTIVITY_HIGH    1
#define USCI_A_SPI_CLOCKPOLARITY_INACTIVITY_LOW     0

/* 3-pin/4-pin */
#define USCI_A_SPI_3PIN     0
#define USCI_A_SPI_4PIN_UCxSTE_ACTIVE_HIGH  1
#define USCI_A_SPI_4PIN_UCxSTE_ACTIVE_LOW   2

/* Interrupt flags */
#define USCI_A_SPI_RECEIVE_INTERRUPT    0x01
#define USCI_A_SPI_TRANSMIT_INTERRUPT   0x02

/* SPI init parameter structure */
typedef struct {
    uint8_t selectClockSource;
    uint32_t clockSourceFrequency;
    uint32_t desiredSpiClock;
    uint8_t msbFirst;
    uint8_t clockPhase;
    uint8_t clockPolarity;
    uint8_t spiMode;
} USCI_A_SPI_initMasterParam;

/* Stub function prototypes */
static inline uint8_t USCI_A_SPI_masterInit(uint16_t baseAddress, USCI_A_SPI_initMasterParam *param) { return 1; }
static inline void USCI_A_SPI_enable(uint16_t baseAddress) { /* Stub */ }
static inline void USCI_A_SPI_disable(uint16_t baseAddress) { /* Stub */ }
static inline void USCI_A_SPI_enableInterrupt(uint16_t baseAddress, uint8_t mask) { /* Stub */ }
static inline void USCI_A_SPI_disableInterrupt(uint16_t baseAddress, uint8_t mask) { /* Stub */ }
static inline void USCI_A_SPI_transmitData(uint16_t baseAddress, uint8_t data) { /* Stub */ }
static inline uint8_t USCI_A_SPI_receiveData(uint16_t baseAddress) { return 0; }
static inline uint8_t USCI_A_SPI_isBusy(uint16_t baseAddress) { return 0; }
static inline void USCI_A_SPI_clearInterrupt(uint16_t baseAddress, uint8_t mask) { /* Stub */ }

#endif /* HAL_USCI_A_SPI_H_ */
