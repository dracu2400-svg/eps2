/*
 * usci_a_uart.h - MSP430 USCI_A UART HAL Compatibility Stub
 * 
 * Copyright (C) 2019, SpaceLab.
 */

#ifndef HAL_USCI_A_UART_H_
#define HAL_USCI_A_UART_H_

#include <stdint.h>

/* Base address definitions (stub) */
#define USCI_A0_BASE    0x0000
#define USCI_A1_BASE    0x0001
#define USCI_A2_BASE    0x0002

/* Parity */
#define USCI_A_UART_NO_PARITY       0
#define USCI_A_UART_ODD_PARITY      1
#define USCI_A_UART_EVEN_PARITY     2

/* Stop bits */
#define USCI_A_UART_ONE_STOP_BIT    0
#define USCI_A_UART_TWO_STOP_BITS   1

/* Mode */
#define USCI_A_UART_MODE            0
#define USCI_A_UART_IDLE_LINE_MULTI_PROCESSOR_MODE  1
#define USCI_A_UART_ADDRESS_BIT_MULTI_PROCESSOR_MODE 2

/* MSB/LSB first */
#define USCI_A_UART_LSB_FIRST       0
#define USCI_A_UART_MSB_FIRST       1

/* Interrupt flags */
#define USCI_A_UART_RECEIVE_INTERRUPT_FLAG      0x01
#define USCI_A_UART_TRANSMIT_INTERRUPT_FLAG     0x02
#define USCI_A_UART_RECEIVE_ERRONEUOUS_CHAR_INTERRUPT   0x04
#define USCI_A_UART_BREAKCHAR_INTERRUPT         0x08

/* UART init parameter structure */
typedef struct {
    uint8_t selectClockSource;
    uint16_t clockPrescalar;
    uint8_t firstModReg;
    uint8_t secondModReg;
    uint8_t parity;
    uint8_t msborLsbFirst;
    uint8_t numberofStopBits;
    uint8_t uartMode;
    uint8_t overSampling;
} USCI_A_UART_initParam;

/* Stub function prototypes */
static inline uint8_t USCI_A_UART_init(uint16_t baseAddress, USCI_A_UART_initParam *param) { return 1; }
static inline void USCI_A_UART_enable(uint16_t baseAddress) { /* Stub */ }
static inline void USCI_A_UART_disable(uint16_t baseAddress) { /* Stub */ }
static inline void USCI_A_UART_enableInterrupt(uint16_t baseAddress, uint8_t mask) { /* Stub */ }
static inline void USCI_A_UART_disableInterrupt(uint16_t baseAddress, uint8_t mask) { /* Stub */ }
static inline void USCI_A_UART_transmitData(uint16_t baseAddress, uint8_t data) { /* Stub */ }
static inline uint8_t USCI_A_UART_receiveData(uint16_t baseAddress) { return 0; }
static inline uint8_t USCI_A_UART_queryStatusFlags(uint16_t baseAddress, uint8_t mask) { return 0; }
static inline void USCI_A_UART_clearInterrupt(uint16_t baseAddress, uint8_t mask) { /* Stub */ }

#endif /* HAL_USCI_A_UART_H_ */
