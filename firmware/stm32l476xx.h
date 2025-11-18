/**
  ******************************************************************************
  * @file    stm32l476xx.h
  * @author  EPS2 Port
  * @brief   CMSIS STM32L476xx Device Peripheral Access Layer Header File.
  *
  *          This file contains:
  *           - Data structures and the address mapping for all peripherals
  *           - Peripheral registers declarations and bits definition
  *           - Macros to access peripheral registers hardware
  *
  ******************************************************************************
  */

#ifndef __STM32L476xx_H
#define __STM32L476xx_H

#ifdef __cplusplus
 extern "C" {
#endif

/** @addtogroup Library_configuration_section
  * @{
  */

/**
  * @brief STM32 Family
  */
#if !defined (STM32L4)
#define STM32L4
#endif /* STM32L4 */

/* Uncomment the line below according to the target STM32L4 device used in your
   application
  */
#if !defined (STM32L476xx)
  #define STM32L476xx
#endif /* STM32L476xx */

/**
  * @brief CMSIS Device version number
  */
#define __STM32L4_CMSIS_VERSION_MAIN   (0x01U) /*!< [31:24] main version */
#define __STM32L4_CMSIS_VERSION_SUB1   (0x00U) /*!< [23:16] sub1 version */
#define __STM32L4_CMSIS_VERSION_SUB2   (0x00U) /*!< [15:8]  sub2 version */
#define __STM32L4_CMSIS_VERSION_RC     (0x00U) /*!< [7:0]  release candidate */
#define __STM32L4_CMSIS_VERSION        ((__STM32L4_CMSIS_VERSION_MAIN << 24U)\
                                       |(__STM32L4_CMSIS_VERSION_SUB1 << 16U)\
                                       |(__STM32L4_CMSIS_VERSION_SUB2 << 8U )\
                                       |(__STM32L4_CMSIS_VERSION_RC))

/**
  * @}
  */

/** @addtogroup Configuration_section_for_CMSIS
  * @{
  */

/**
  * @brief Configuration of the Cortex-M4 Processor and Core Peripherals
   */
#define __CM4_REV                 0x0001U  /*!< Cortex-M4 revision r0p1                       */
#define __MPU_PRESENT             1U       /*!< STM32L4XX provides an MPU                     */
#define __NVIC_PRIO_BITS          4U       /*!< STM32L4XX uses 4 Bits for the Priority Levels */
#define __Vendor_SysTickConfig    0U       /*!< Set to 1 if different SysTick Config is used  */
#define __FPU_PRESENT             1U       /*!< FPU present                                   */

/**
  * @}
  */

/** @addtogroup Peripheral_interrupt_number_definition
  * @{
  */

/**
 * @brief STM32L4XX Interrupt Number Definition, according to the selected device
 *        in @ref Library_configuration_section
 */
typedef enum
{
/******  Cortex-M4 Processor Exceptions Numbers ****************************************************************/
  NonMaskableInt_IRQn         = -14,    /*!< 2 Non Maskable Interrupt                                          */
  HardFault_IRQn              = -13,    /*!< 3 Cortex-M4 Hard Fault Interrupt                                  */
  MemoryManagement_IRQn       = -12,    /*!< 4 Cortex-M4 Memory Management Interrupt                           */
  BusFault_IRQn               = -11,    /*!< 5 Cortex-M4 Bus Fault Interrupt                                   */
  UsageFault_IRQn             = -10,    /*!< 6 Cortex-M4 Usage Fault Interrupt                                 */
  SVCall_IRQn                 = -5,     /*!< 11 Cortex-M4 SV Call Interrupt                                    */
  DebugMonitor_IRQn           = -4,     /*!< 12 Cortex-M4 Debug Monitor Interrupt                              */
  PendSV_IRQn                 = -2,     /*!< 14 Cortex-M4 Pend SV Interrupt                                    */
  SysTick_IRQn                = -1,     /*!< 15 Cortex-M4 System Tick Interrupt                                */
/******  STM32 specific Interrupt Numbers **********************************************************************/
  WWDG_IRQn                   = 0,      /*!< Window WatchDog Interrupt                                         */
  PVD_PVM_IRQn                = 1,      /*!< PVD/PVM1/PVM2/PVM3/PVM4 through EXTI Line detection Interrupts   */
  TAMP_STAMP_IRQn             = 2,      /*!< Tamper and TimeStamp interrupts through the EXTI line             */
  RTC_WKUP_IRQn               = 3,      /*!< RTC Wakeup interrupt through the EXTI line                        */
  FLASH_IRQn                  = 4,      /*!< FLASH global Interrupt                                            */
  RCC_IRQn                    = 5,      /*!< RCC global Interrupt                                              */
  EXTI0_IRQn                  = 6,      /*!< EXTI Line0 Interrupt                                              */
  EXTI1_IRQn                  = 7,      /*!< EXTI Line1 Interrupt                                              */
  EXTI2_IRQn                  = 8,      /*!< EXTI Line2 Interrupt                                              */
  EXTI3_IRQn                  = 9,      /*!< EXTI Line3 Interrupt                                              */
  EXTI4_IRQn                  = 10,     /*!< EXTI Line4 Interrupt                                              */
  DMA1_Channel1_IRQn          = 11,     /*!< DMA1 Channel 1 global Interrupt                                   */
  DMA1_Channel2_IRQn          = 12,     /*!< DMA1 Channel 2 global Interrupt                                   */
  DMA1_Channel3_IRQn          = 13,     /*!< DMA1 Channel 3 global Interrupt                                   */
  DMA1_Channel4_IRQn          = 14,     /*!< DMA1 Channel 4 global Interrupt                                   */
  DMA1_Channel5_IRQn          = 15,     /*!< DMA1 Channel 5 global Interrupt                                   */
  DMA1_Channel6_IRQn          = 16,     /*!< DMA1 Channel 6 global Interrupt                                   */
  DMA1_Channel7_IRQn          = 17,     /*!< DMA1 Channel 7 global Interrupt                                   */
  ADC1_2_IRQn                 = 18,     /*!< ADC1, ADC2 SAR global Interrupts                                  */
  CAN1_TX_IRQn                = 19,     /*!< CAN1 TX Interrupt                                                 */
  CAN1_RX0_IRQn               = 20,     /*!< CAN1 RX0 Interrupt                                                */
  CAN1_RX1_IRQn               = 21,     /*!< CAN1 RX1 Interrupt                                                */
  CAN1_SCE_IRQn               = 22,     /*!< CAN1 SCE Interrupt                                                */
  EXTI9_5_IRQn                = 23,     /*!< External Line[9:5] Interrupts                                     */
  TIM1_BRK_TIM15_IRQn         = 24,     /*!< TIM1 Break interrupt and TIM15 global interrupt                   */
  TIM1_UP_TIM16_IRQn          = 25,     /*!< TIM1 Update Interrupt and TIM16 global interrupt                  */
  TIM1_TRG_COM_TIM17_IRQn     = 26,     /*!< TIM1 Trigger and Commutation Interrupt and TIM17 global interrupt */
  TIM1_CC_IRQn                = 27,     /*!< TIM1 Capture Compare Interrupt                                    */
  TIM2_IRQn                   = 28,     /*!< TIM2 global Interrupt                                             */
  TIM3_IRQn                   = 29,     /*!< TIM3 global Interrupt                                             */
  TIM4_IRQn                   = 30,     /*!< TIM4 global Interrupt                                             */
  I2C1_EV_IRQn                = 31,     /*!< I2C1 Event Interrupt                                              */
  I2C1_ER_IRQn                = 32,     /*!< I2C1 Error Interrupt                                              */
  I2C2_EV_IRQn                = 33,     /*!< I2C2 Event Interrupt                                              */
  I2C2_ER_IRQn                = 34,     /*!< I2C2 Error Interrupt                                              */
  SPI1_IRQn                   = 35,     /*!< SPI1 global Interrupt                                             */
  SPI2_IRQn                   = 36,     /*!< SPI2 global Interrupt                                             */
  USART1_IRQn                 = 37,     /*!< USART1 global Interrupt                                           */
  USART2_IRQn                 = 38,     /*!< USART2 global Interrupt                                           */
  USART3_IRQn                 = 39,     /*!< USART3 global Interrupt                                           */
  EXTI15_10_IRQn              = 40,     /*!< External Line[15:10] Interrupts                                   */
  RTC_Alarm_IRQn              = 41,     /*!< RTC Alarm (A and B) through EXTI Line Interrupt                   */
  DFSDM1_FLT3_IRQn            = 42,     /*!< DFSDM1 Filter 3 global Interrupt                                  */
  TIM8_BRK_IRQn               = 43,     /*!< TIM8 Break Interrupt                                              */
  TIM8_UP_IRQn                = 44,     /*!< TIM8 Update Interrupt                                             */
  TIM8_TRG_COM_IRQn           = 45,     /*!< TIM8 Trigger and Commutation Interrupt                            */
  TIM8_CC_IRQn                = 46,     /*!< TIM8 Capture Compare Interrupt                                    */
  ADC3_IRQn                   = 47,     /*!< ADC3 global  Interrupt                                            */
  FMC_IRQn                    = 48,     /*!< FMC global Interrupt                                              */
  SDMMC1_IRQn                 = 49,     /*!< SDMMC1 global Interrupt                                           */
  TIM5_IRQn                   = 50,     /*!< TIM5 global Interrupt                                             */
  SPI3_IRQn                   = 51,     /*!< SPI3 global Interrupt                                             */
  UART4_IRQn                  = 52,     /*!< UART4 global Interrupt                                            */
  UART5_IRQn                  = 53,     /*!< UART5 global Interrupt                                            */
  TIM6_DAC_IRQn               = 54,     /*!< TIM6 global and DAC1&2 underrun error  interrupts                 */
  TIM7_IRQn                   = 55,     /*!< TIM7 global interrupt                                             */
  DMA2_Channel1_IRQn          = 56,     /*!< DMA2 Channel 1 global Interrupt                                   */
  DMA2_Channel2_IRQn          = 57,     /*!< DMA2 Channel 2 global Interrupt                                   */
  DMA2_Channel3_IRQn          = 58,     /*!< DMA2 Channel 3 global Interrupt                                   */
  DMA2_Channel4_IRQn          = 59,     /*!< DMA2 Channel 4 global Interrupt                                   */
  DMA2_Channel5_IRQn          = 60,     /*!< DMA2 Channel 5 global Interrupt                                   */
  DFSDM1_FLT0_IRQn            = 61,     /*!< DFSDM1 Filter 0 global Interrupt                                  */
  DFSDM1_FLT1_IRQn            = 62,     /*!< DFSDM1 Filter 1 global Interrupt                                  */
  DFSDM1_FLT2_IRQn            = 63,     /*!< DFSDM1 Filter 2 global Interrupt                                  */
  COMP_IRQn                   = 64,     /*!< COMP1 and COMP2 Interrupts                                        */
  LPTIM1_IRQn                 = 65,     /*!< LP TIM1 interrupt                                                 */
  LPTIM2_IRQn                 = 66,     /*!< LP TIM2 interrupt                                                 */
  OTG_FS_IRQn                 = 67,     /*!< USB OTG FS global Interrupt                                       */
  DMA2_Channel6_IRQn          = 68,     /*!< DMA2 Channel 6 global interrupt                                   */
  DMA2_Channel7_IRQn          = 69,     /*!< DMA2 Channel 7 global interrupt                                   */
  LPUART1_IRQn                = 70,     /*!< LP UART1 interrupt                                                */
  QUADSPI_IRQn                = 71,     /*!< Quad SPI global interrupt                                         */
  I2C3_EV_IRQn                = 72,     /*!< I2C3 event interrupt                                              */
  I2C3_ER_IRQn                = 73,     /*!< I2C3 error interrupt                                              */
  SAI1_IRQn                   = 74,     /*!< Serial Audio Interface 1 global interrupt                         */
  SAI2_IRQn                   = 75,     /*!< Serial Audio Interface 2 global interrupt                         */
  SWPMI1_IRQn                 = 76,     /*!< Serial Wire Interface 1 global interrupt                          */
  TSC_IRQn                    = 77,     /*!< Touch Sense Controller global interrupt                           */
  LCD_IRQn                    = 78,     /*!< LCD global interrupt                                              */
  RNG_IRQn                    = 80,     /*!< RNG global interrupt                                              */
  FPU_IRQn                    = 81      /*!< FPU global interrupt                                              */
} IRQn_Type;

/**
  * @}
  */

#include "core_cm4.h"             /* Cortex-M4 processor and core peripherals */
#include <stdint.h>

/** @addtogroup Peripheral_registers_structures
  * @{
  */

/**
  * @brief General Purpose I/O
  */
typedef struct
{
  volatile uint32_t MODER;    /*!< GPIO port mode register,               Address offset: 0x00      */
  volatile uint32_t OTYPER;   /*!< GPIO port output type register,        Address offset: 0x04      */
  volatile uint32_t OSPEEDR;  /*!< GPIO port output speed register,       Address offset: 0x08      */
  volatile uint32_t PUPDR;    /*!< GPIO port pull-up/pull-down register,  Address offset: 0x0C      */
  volatile uint32_t IDR;      /*!< GPIO port input data register,         Address offset: 0x10      */
  volatile uint32_t ODR;      /*!< GPIO port output data register,        Address offset: 0x14      */
  volatile uint32_t BSRR;     /*!< GPIO port bit set/reset register,      Address offset: 0x18      */
  volatile uint32_t LCKR;     /*!< GPIO port configuration lock register, Address offset: 0x1C      */
  volatile uint32_t AFR[2];   /*!< GPIO alternate function registers,     Address offset: 0x20-0x24 */
  volatile uint32_t BRR;      /*!< GPIO Bit Reset register,               Address offset: 0x28      */
  volatile uint32_t ASCR;     /*!< GPIO analog switch control register,   Address offset: 0x2C     */
} GPIO_TypeDef;

/**
  * @brief Reset and Clock Control
  */
typedef struct
{
  volatile uint32_t CR;           /*!< RCC clock control register,                                              Address offset: 0x00 */
  volatile uint32_t ICSCR;        /*!< RCC internal clock sources calibration register,                         Address offset: 0x04 */
  volatile uint32_t CFGR;         /*!< RCC clock configuration register,                                        Address offset: 0x08 */
  volatile uint32_t PLLCFGR;      /*!< RCC system PLL configuration register,                                   Address offset: 0x0C */
  volatile uint32_t PLLSAI1CFGR;  /*!< RCC PLL SAI1 configuration register,                                     Address offset: 0x10 */
  volatile uint32_t PLLSAI2CFGR;  /*!< RCC PLL SAI2 configuration register,                                     Address offset: 0x14 */
  volatile uint32_t CIER;         /*!< RCC clock interrupt enable register,                                     Address offset: 0x18 */
  volatile uint32_t CIFR;         /*!< RCC clock interrupt flag register,                                       Address offset: 0x1C */
  volatile uint32_t CICR;         /*!< RCC clock interrupt clear register,                                      Address offset: 0x20 */
  uint32_t      RESERVED0;        /*!< Reserved,                                                                Address offset: 0x24 */
  volatile uint32_t AHB1RSTR;     /*!< RCC AHB1 peripheral reset register,                                      Address offset: 0x28 */
  volatile uint32_t AHB2RSTR;     /*!< RCC AHB2 peripheral reset register,                                      Address offset: 0x2C */
  volatile uint32_t AHB3RSTR;     /*!< RCC AHB3 peripheral reset register,                                      Address offset: 0x30 */
  uint32_t      RESERVED1;        /*!< Reserved,                                                                Address offset: 0x34 */
  volatile uint32_t APB1RSTR1;    /*!< RCC APB1 peripheral reset register 1,                                    Address offset: 0x38 */
  volatile uint32_t APB1RSTR2;    /*!< RCC APB1 peripheral reset register 2,                                    Address offset: 0x3C */
  volatile uint32_t APB2RSTR;     /*!< RCC APB2 peripheral reset register,                                      Address offset: 0x40 */
  uint32_t      RESERVED2;        /*!< Reserved,                                                                Address offset: 0x44 */
  volatile uint32_t AHB1ENR;      /*!< RCC AHB1 peripheral clocks enable register,                              Address offset: 0x48 */
  volatile uint32_t AHB2ENR;      /*!< RCC AHB2 peripheral clocks enable register,                              Address offset: 0x4C */
  volatile uint32_t AHB3ENR;      /*!< RCC AHB3 peripheral clocks enable register,                              Address offset: 0x50 */
  uint32_t      RESERVED3;        /*!< Reserved,                                                                Address offset: 0x54 */
  volatile uint32_t APB1ENR1;     /*!< RCC APB1 peripheral clocks enable register 1,                            Address offset: 0x58 */
  volatile uint32_t APB1ENR2;     /*!< RCC APB1 peripheral clocks enable register 2,                            Address offset: 0x5C */
  volatile uint32_t APB2ENR;      /*!< RCC APB2 peripheral clocks enable register,                              Address offset: 0x60 */
  uint32_t      RESERVED4;        /*!< Reserved,                                                                Address offset: 0x64 */
  volatile uint32_t AHB1SMENR;    /*!< RCC AHB1 peripheral clocks enable in sleep and stop modes register,      Address offset: 0x68 */
  volatile uint32_t AHB2SMENR;    /*!< RCC AHB2 peripheral clocks enable in sleep and stop modes register,      Address offset: 0x6C */
  volatile uint32_t AHB3SMENR;    /*!< RCC AHB3 peripheral clocks enable in sleep and stop modes register,      Address offset: 0x70 */
  uint32_t      RESERVED5;        /*!< Reserved,                                                                Address offset: 0x74 */
  volatile uint32_t APB1SMENR1;   /*!< RCC APB1 peripheral clocks enable in sleep mode and stop modes register 1, Address offset: 0x78 */
  volatile uint32_t APB1SMENR2;   /*!< RCC APB1 peripheral clocks enable in sleep mode and stop modes register 2, Address offset: 0x7C */
  volatile uint32_t APB2SMENR;    /*!< RCC APB2 peripheral clocks enable in sleep mode and stop modes register, Address offset: 0x80 */
  uint32_t      RESERVED6;        /*!< Reserved,                                                                Address offset: 0x84 */
  volatile uint32_t CCIPR;        /*!< RCC peripherals independent clock configuration register,                Address offset: 0x88 */
  uint32_t      RESERVED7;        /*!< Reserved,                                                                Address offset: 0x8C */
  volatile uint32_t BDCR;         /*!< RCC backup domain control register,                                      Address offset: 0x90 */
  volatile uint32_t CSR;          /*!< RCC clock control & status register,                                     Address offset: 0x94 */
} RCC_TypeDef;

/**
  * @brief Universal Synchronous Asynchronous Receiver Transmitter
  */
typedef struct
{
  volatile uint32_t CR1;    /*!< USART Control register 1,                 Address offset: 0x00 */
  volatile uint32_t CR2;    /*!< USART Control register 2,                 Address offset: 0x04 */
  volatile uint32_t CR3;    /*!< USART Control register 3,                 Address offset: 0x08 */
  volatile uint32_t BRR;    /*!< USART Baud rate register,                 Address offset: 0x0C */
  volatile uint32_t GTPR;   /*!< USART Guard time and prescaler register,  Address offset: 0x10 */
  volatile uint32_t RTOR;   /*!< USART Receiver Time Out register,         Address offset: 0x14 */
  volatile uint32_t RQR;    /*!< USART Request register,                   Address offset: 0x18 */
  volatile uint32_t ISR;    /*!< USART Interrupt and status register,      Address offset: 0x1C */
  volatile uint32_t ICR;    /*!< USART Interrupt flag Clear register,      Address offset: 0x20 */
  volatile uint32_t RDR;    /*!< USART Receive Data register,              Address offset: 0x24 */
  volatile uint32_t TDR;    /*!< USART Transmit Data register,             Address offset: 0x28 */
} USART_TypeDef;

/**
  * @brief Serial Peripheral Interface
  */
typedef struct
{
  volatile uint32_t CR1;      /*!< SPI Control register 1,      Address offset: 0x00 */
  volatile uint32_t CR2;      /*!< SPI Control register 2,      Address offset: 0x04 */
  volatile uint32_t SR;       /*!< SPI Status register,         Address offset: 0x08 */
  volatile uint32_t DR;       /*!< SPI data register,           Address offset: 0x0C */
  volatile uint32_t CRCPR;    /*!< SPI CRC polynomial register, Address offset: 0x10 */
  volatile uint32_t RXCRCR;   /*!< SPI Rx CRC register,         Address offset: 0x14 */
  volatile uint32_t TXCRCR;   /*!< SPI Tx CRC register,         Address offset: 0x18 */
} SPI_TypeDef;

/**
  * @brief Inter-integrated Circuit Interface
  */
typedef struct
{
  volatile uint32_t CR1;      /*!< I2C Control register 1,            Address offset: 0x00 */
  volatile uint32_t CR2;      /*!< I2C Control register 2,            Address offset: 0x04 */
  volatile uint32_t OAR1;     /*!< I2C Own address 1 register,        Address offset: 0x08 */
  volatile uint32_t OAR2;     /*!< I2C Own address 2 register,        Address offset: 0x0C */
  volatile uint32_t TIMINGR;  /*!< I2C Timing register,               Address offset: 0x10 */
  volatile uint32_t TIMEOUTR; /*!< I2C Timeout register,              Address offset: 0x14 */
  volatile uint32_t ISR;      /*!< I2C Interrupt and status register, Address offset: 0x18 */
  volatile uint32_t ICR;      /*!< I2C Interrupt clear register,      Address offset: 0x1C */
  volatile uint32_t PECR;     /*!< I2C PEC register,                  Address offset: 0x20 */
  volatile uint32_t RXDR;     /*!< I2C Receive data register,         Address offset: 0x24 */
  volatile uint32_t TXDR;     /*!< I2C Transmit data register,        Address offset: 0x28 */
} I2C_TypeDef;

/**
  * @brief Analog to Digital Converter
  */
typedef struct
{
  volatile uint32_t ISR;          /*!< ADC interrupt and status register,             Address offset: 0x00 */
  volatile uint32_t IER;          /*!< ADC interrupt enable register,                 Address offset: 0x04 */
  volatile uint32_t CR;           /*!< ADC control register,                          Address offset: 0x08 */
  volatile uint32_t CFGR;         /*!< ADC configuration register 1,                  Address offset: 0x0C */
  volatile uint32_t CFGR2;        /*!< ADC configuration register 2,                  Address offset: 0x10 */
  volatile uint32_t SMPR1;        /*!< ADC sampling time register 1,                  Address offset: 0x14 */
  volatile uint32_t SMPR2;        /*!< ADC sampling time register 2,                  Address offset: 0x18 */
  uint32_t RESERVED1;             /*!< Reserved,                                                      0x1C */
  volatile uint32_t TR1;          /*!< ADC analog watchdog 1 threshold register,      Address offset: 0x20 */
  volatile uint32_t TR2;          /*!< ADC analog watchdog 2 threshold register,      Address offset: 0x24 */
  volatile uint32_t TR3;          /*!< ADC analog watchdog 3 threshold register,      Address offset: 0x28 */
  uint32_t RESERVED2;             /*!< Reserved,                                                      0x2C */
  volatile uint32_t SQR1;         /*!< ADC group regular sequencer register 1,        Address offset: 0x30 */
  volatile uint32_t SQR2;         /*!< ADC group regular sequencer register 2,        Address offset: 0x34 */
  volatile uint32_t SQR3;         /*!< ADC group regular sequencer register 3,        Address offset: 0x38 */
  volatile uint32_t SQR4;         /*!< ADC group regular sequencer register 4,        Address offset: 0x3C */
  volatile uint32_t DR;           /*!< ADC group regular data register,               Address offset: 0x40 */
  uint32_t RESERVED3;             /*!< Reserved,                                                      0x44 */
  uint32_t RESERVED4;             /*!< Reserved,                                                      0x48 */
  volatile uint32_t JSQR;         /*!< ADC group injected sequencer register,         Address offset: 0x4C */
  uint32_t RESERVED5[4];          /*!< Reserved,                                               0x50 - 0x5C */
  volatile uint32_t OFR1;         /*!< ADC offset register 1,                         Address offset: 0x60 */
  volatile uint32_t OFR2;         /*!< ADC offset register 2,                         Address offset: 0x64 */
  volatile uint32_t OFR3;         /*!< ADC offset register 3,                         Address offset: 0x68 */
  volatile uint32_t OFR4;         /*!< ADC offset register 4,                         Address offset: 0x6C */
  uint32_t RESERVED6[4];          /*!< Reserved,                                               0x70 - 0x7C */
  volatile uint32_t JDR1;         /*!< ADC group injected rank 1 data register,       Address offset: 0x80 */
  volatile uint32_t JDR2;         /*!< ADC group injected rank 2 data register,       Address offset: 0x84 */
  volatile uint32_t JDR3;         /*!< ADC group injected rank 3 data register,       Address offset: 0x88 */
  volatile uint32_t JDR4;         /*!< ADC group injected rank 4 data register,       Address offset: 0x8C */
  uint32_t RESERVED7[4];          /*!< Reserved,                                             0x090 - 0x09C */
  volatile uint32_t AWD2CR;       /*!< ADC analog watchdog 2 configuration register,  Address offset: 0xA0 */
  volatile uint32_t AWD3CR;       /*!< ADC analog watchdog 3 Configuration Register,  Address offset: 0xA4 */
  uint32_t RESERVED8;             /*!< Reserved,                                                     0x0A8 */
  uint32_t RESERVED9;             /*!< Reserved,                                                     0x0AC */
  volatile uint32_t DIFSEL;       /*!< ADC differential mode selection register,      Address offset: 0xB0 */
  volatile uint32_t CALFACT;      /*!< ADC calibration factors,                       Address offset: 0xB4 */
} ADC_TypeDef;

/**
  * @brief TIM
  */
typedef struct
{
  volatile uint32_t CR1;         /*!< TIM control register 1,                   Address offset: 0x00 */
  volatile uint32_t CR2;         /*!< TIM control register 2,                   Address offset: 0x04 */
  volatile uint32_t SMCR;        /*!< TIM slave mode control register,          Address offset: 0x08 */
  volatile uint32_t DIER;        /*!< TIM DMA/interrupt enable register,        Address offset: 0x0C */
  volatile uint32_t SR;          /*!< TIM status register,                      Address offset: 0x10 */
  volatile uint32_t EGR;         /*!< TIM event generation register,            Address offset: 0x14 */
  volatile uint32_t CCMR1;       /*!< TIM capture/compare mode register 1,      Address offset: 0x18 */
  volatile uint32_t CCMR2;       /*!< TIM capture/compare mode register 2,      Address offset: 0x1C */
  volatile uint32_t CCER;        /*!< TIM capture/compare enable register,      Address offset: 0x20 */
  volatile uint32_t CNT;         /*!< TIM counter register,                     Address offset: 0x24 */
  volatile uint32_t PSC;         /*!< TIM prescaler register,                   Address offset: 0x28 */
  volatile uint32_t ARR;         /*!< TIM auto-reload register,                 Address offset: 0x2C */
  volatile uint32_t RCR;         /*!< TIM repetition counter register,          Address offset: 0x30 */
  volatile uint32_t CCR1;        /*!< TIM capture/compare register 1,           Address offset: 0x34 */
  volatile uint32_t CCR2;        /*!< TIM capture/compare register 2,           Address offset: 0x38 */
  volatile uint32_t CCR3;        /*!< TIM capture/compare register 3,           Address offset: 0x3C */
  volatile uint32_t CCR4;        /*!< TIM capture/compare register 4,           Address offset: 0x40 */
  volatile uint32_t BDTR;        /*!< TIM break and dead-time register,         Address offset: 0x44 */
  volatile uint32_t DCR;         /*!< TIM DMA control register,                 Address offset: 0x48 */
  volatile uint32_t DMAR;        /*!< TIM DMA address for full transfer,        Address offset: 0x4C */
  volatile uint32_t OR1;         /*!< TIM option register 1,                    Address offset: 0x50 */
  volatile uint32_t CCMR3;       /*!< TIM capture/compare mode register 3,      Address offset: 0x54 */
  volatile uint32_t CCR5;        /*!< TIM capture/compare register 5,           Address offset: 0x58 */
  volatile uint32_t CCR6;        /*!< TIM capture/compare register 6,           Address offset: 0x5C */
  volatile uint32_t OR2;         /*!< TIM option register 2,                    Address offset: 0x60 */
  volatile uint32_t OR3;         /*!< TIM option register 3,                    Address offset: 0x64 */
} TIM_TypeDef;

/**
  * @brief Independent WATCHDOG
  */
typedef struct
{
  volatile uint32_t KR;   /*!< IWDG Key register,       Address offset: 0x00 */
  volatile uint32_t PR;   /*!< IWDG Prescaler register, Address offset: 0x04 */
  volatile uint32_t RLR;  /*!< IWDG Reload register,    Address offset: 0x08 */
  volatile uint32_t SR;   /*!< IWDG Status register,    Address offset: 0x0C */
  volatile uint32_t WINR; /*!< IWDG Window register,    Address offset: 0x10 */
} IWDG_TypeDef;

/**
  * @brief FLASH Registers
  */
typedef struct
{
  volatile uint32_t ACR;      /*!< FLASH access control register,            Address offset: 0x00 */
  volatile uint32_t PDKEYR;   /*!< FLASH power down key register,            Address offset: 0x04 */
  volatile uint32_t KEYR;     /*!< FLASH key register,                       Address offset: 0x08 */
  volatile uint32_t OPTKEYR;  /*!< FLASH option key register,                Address offset: 0x0C */
  volatile uint32_t SR;       /*!< FLASH status register,                    Address offset: 0x10 */
  volatile uint32_t CR;       /*!< FLASH control register,                   Address offset: 0x14 */
  volatile uint32_t ECCR;     /*!< FLASH ECC register,                       Address offset: 0x18 */
  uint32_t RESERVED1;         /*!< Reserved1,                                Address offset: 0x1C */
  volatile uint32_t OPTR;     /*!< FLASH option register,                    Address offset: 0x20 */
  volatile uint32_t PCROP1SR; /*!< FLASH bank1 PCROP start address register, Address offset: 0x24 */
  volatile uint32_t PCROP1ER; /*!< FLASH bank1 PCROP end address register,   Address offset: 0x28 */
  volatile uint32_t WRP1AR;   /*!< FLASH bank1 WRP area A address register,  Address offset: 0x2C */
  volatile uint32_t WRP1BR;   /*!< FLASH bank1 WRP area B address register,  Address offset: 0x30 */
} FLASH_TypeDef;

/**
  * @}
  */

/** @addtogroup Peripheral_memory_map
  * @{
  */
#define FLASH_BASE            0x08000000UL /*!< FLASH base address */
#define SRAM1_BASE            0x20000000UL /*!< SRAM1(96 KB) base address*/
#define SRAM2_BASE            0x10000000UL /*!< SRAM2(32 KB) base address*/
#define PERIPH_BASE           0x40000000UL /*!< Peripheral base address */

#define SRAM1_SIZE_MAX        0x00018000UL /*!< maximum SRAM1 size (up to 96 KBytes) */
#define SRAM2_SIZE            0x00008000UL /*!< SRAM2 size (32 KBytes) */

/*!< Peripheral memory map */
#define APB1PERIPH_BASE       PERIPH_BASE
#define APB2PERIPH_BASE       (PERIPH_BASE + 0x00010000UL)
#define AHB1PERIPH_BASE       (PERIPH_BASE + 0x00020000UL)
#define AHB2PERIPH_BASE       (PERIPH_BASE + 0x08000000UL)

/*!< APB1 peripherals */
#define TIM2_BASE             (APB1PERIPH_BASE + 0x0000UL)
#define TIM3_BASE             (APB1PERIPH_BASE + 0x0400UL)
#define TIM4_BASE             (APB1PERIPH_BASE + 0x0800UL)
#define TIM5_BASE             (APB1PERIPH_BASE + 0x0C00UL)
#define TIM6_BASE             (APB1PERIPH_BASE + 0x1000UL)
#define TIM7_BASE             (APB1PERIPH_BASE + 0x1400UL)
#define USART2_BASE           (APB1PERIPH_BASE + 0x4400UL)
#define USART3_BASE           (APB1PERIPH_BASE + 0x4800UL)
#define UART4_BASE            (APB1PERIPH_BASE + 0x4C00UL)
#define UART5_BASE            (APB1PERIPH_BASE + 0x5000UL)
#define I2C1_BASE             (APB1PERIPH_BASE + 0x5400UL)
#define I2C2_BASE             (APB1PERIPH_BASE + 0x5800UL)
#define I2C3_BASE             (APB1PERIPH_BASE + 0x5C00UL)
#define PWR_BASE              (APB1PERIPH_BASE + 0x7000UL)

/*!< APB2 peripherals */
#define TIM1_BASE             (APB2PERIPH_BASE + 0x2C00UL)
#define SPI1_BASE             (APB2PERIPH_BASE + 0x3000UL)
#define TIM8_BASE             (APB2PERIPH_BASE + 0x3400UL)
#define USART1_BASE           (APB2PERIPH_BASE + 0x3800UL)
#define TIM15_BASE            (APB2PERIPH_BASE + 0x4000UL)
#define TIM16_BASE            (APB2PERIPH_BASE + 0x4400UL)
#define TIM17_BASE            (APB2PERIPH_BASE + 0x4800UL)

/*!< AHB1 peripherals */
#define DMA1_BASE             (AHB1PERIPH_BASE + 0x0000UL)
#define DMA2_BASE             (AHB1PERIPH_BASE + 0x0400UL)
#define RCC_BASE              (AHB1PERIPH_BASE + 0x1000UL)
#define FLASH_R_BASE          (AHB1PERIPH_BASE + 0x2000UL)

/*!< AHB2 peripherals */
#define GPIOA_BASE            (AHB2PERIPH_BASE + 0x0000UL)
#define GPIOB_BASE            (AHB2PERIPH_BASE + 0x0400UL)
#define GPIOC_BASE            (AHB2PERIPH_BASE + 0x0800UL)
#define GPIOD_BASE            (AHB2PERIPH_BASE + 0x0C00UL)
#define GPIOE_BASE            (AHB2PERIPH_BASE + 0x1000UL)
#define GPIOF_BASE            (AHB2PERIPH_BASE + 0x1400UL)
#define GPIOG_BASE            (AHB2PERIPH_BASE + 0x1800UL)
#define GPIOH_BASE            (AHB2PERIPH_BASE + 0x1C00UL)
#define ADC1_BASE             (AHB2PERIPH_BASE + 0x08040000UL)
#define ADC2_BASE             (AHB2PERIPH_BASE + 0x08040100UL)
#define ADC3_BASE             (AHB2PERIPH_BASE + 0x08040200UL)

#define SPI2_BASE             (APB1PERIPH_BASE + 0x3800UL)
#define SPI3_BASE             (APB1PERIPH_BASE + 0x3C00UL)

#define IWDG_BASE             (APB1PERIPH_BASE + 0x3000UL)

/**
  * @}
  */

/** @addtogroup Peripheral_declaration
  * @{
  */
#define TIM2                ((TIM_TypeDef *) TIM2_BASE)
#define TIM3                ((TIM_TypeDef *) TIM3_BASE)
#define TIM4                ((TIM_TypeDef *) TIM4_BASE)
#define TIM5                ((TIM_TypeDef *) TIM5_BASE)
#define TIM6                ((TIM_TypeDef *) TIM6_BASE)
#define TIM7                ((TIM_TypeDef *) TIM7_BASE)
#define RCC                 ((RCC_TypeDef *) RCC_BASE)
#define FLASH               ((FLASH_TypeDef *) FLASH_R_BASE)
#define GPIOA               ((GPIO_TypeDef *) GPIOA_BASE)
#define GPIOB               ((GPIO_TypeDef *) GPIOB_BASE)
#define GPIOC               ((GPIO_TypeDef *) GPIOC_BASE)
#define GPIOD               ((GPIO_TypeDef *) GPIOD_BASE)
#define GPIOE               ((GPIO_TypeDef *) GPIOE_BASE)
#define GPIOF               ((GPIO_TypeDef *) GPIOF_BASE)
#define GPIOG               ((GPIO_TypeDef *) GPIOG_BASE)
#define GPIOH               ((GPIO_TypeDef *) GPIOH_BASE)
#define ADC1                ((ADC_TypeDef *) ADC1_BASE)
#define ADC2                ((ADC_TypeDef *) ADC2_BASE)
#define ADC3                ((ADC_TypeDef *) ADC3_BASE)
#define USART1              ((USART_TypeDef *) USART1_BASE)
#define USART2              ((USART_TypeDef *) USART2_BASE)
#define USART3              ((USART_TypeDef *) USART3_BASE)
#define UART4               ((USART_TypeDef *) UART4_BASE)
#define UART5               ((USART_TypeDef *) UART5_BASE)
#define SPI1                ((SPI_TypeDef *) SPI1_BASE)
#define SPI2                ((SPI_TypeDef *) SPI2_BASE)
#define SPI3                ((SPI_TypeDef *) SPI3_BASE)
#define I2C1                ((I2C_TypeDef *) I2C1_BASE)
#define I2C2                ((I2C_TypeDef *) I2C2_BASE)
#define I2C3                ((I2C_TypeDef *) I2C3_BASE)
#define TIM1                ((TIM_TypeDef *) TIM1_BASE)
#define TIM8                ((TIM_TypeDef *) TIM8_BASE)
#define IWDG                ((IWDG_TypeDef *) IWDG_BASE)

/**
  * @}
  */

/** @addtogroup Exported_constants
  * @{
  */

  /** @addtogroup Peripheral_Registers_Bits_Definition
  * @{
  */

/******************************************************************************/
/*                         Peripheral Registers Bits Definition              */
/******************************************************************************/

/******************************************************************************/
/*                                                                            */
/*                        Reset and Clock Control (RCC)                       */
/*                                                                            */
/******************************************************************************/
/********************  Bit definition for RCC_CR register  ********************/
#define RCC_CR_MSION_Pos                 (0U)
#define RCC_CR_MSION_Msk                 (0x1UL << RCC_CR_MSION_Pos)           /*!< 0x00000001 */
#define RCC_CR_MSION                     RCC_CR_MSION_Msk                      /*!< MSI clock enable */
#define RCC_CR_MSIRDY_Pos                (1U)
#define RCC_CR_MSIRDY_Msk                (0x1UL << RCC_CR_MSIRDY_Pos)          /*!< 0x00000002 */
#define RCC_CR_MSIRDY                    RCC_CR_MSIRDY_Msk                     /*!< MSI clock ready flag */
#define RCC_CR_HSION_Pos                 (8U)
#define RCC_CR_HSION_Msk                 (0x1UL << RCC_CR_HSION_Pos)           /*!< 0x00000100 */
#define RCC_CR_HSION                     RCC_CR_HSION_Msk                      /*!< HSI clock enable */
#define RCC_CR_HSIRDY_Pos                (10U)
#define RCC_CR_HSIRDY_Msk                (0x1UL << RCC_CR_HSIRDY_Pos)          /*!< 0x00000400 */
#define RCC_CR_HSIRDY                    RCC_CR_HSIRDY_Msk                     /*!< HSI clock ready flag */
#define RCC_CR_HSEON_Pos                 (16U)
#define RCC_CR_HSEON_Msk                 (0x1UL << RCC_CR_HSEON_Pos)           /*!< 0x00010000 */
#define RCC_CR_HSEON                     RCC_CR_HSEON_Msk                      /*!< HSE clock enable */
#define RCC_CR_HSERDY_Pos                (17U)
#define RCC_CR_HSERDY_Msk                (0x1UL << RCC_CR_HSERDY_Pos)          /*!< 0x00020000 */
#define RCC_CR_HSERDY                    RCC_CR_HSERDY_Msk                     /*!< HSE clock ready flag */
#define RCC_CR_PLLON_Pos                 (24U)
#define RCC_CR_PLLON_Msk                 (0x1UL << RCC_CR_PLLON_Pos)           /*!< 0x01000000 */
#define RCC_CR_PLLON                     RCC_CR_PLLON_Msk                      /*!< System PLL enable */
#define RCC_CR_PLLRDY_Pos                (25U)
#define RCC_CR_PLLRDY_Msk                (0x1UL << RCC_CR_PLLRDY_Pos)          /*!< 0x02000000 */
#define RCC_CR_PLLRDY                    RCC_CR_PLLRDY_Msk                     /*!< System PLL clock ready flag */

/********************  Bit definition for RCC_CFGR register  ******************/
#define RCC_CFGR_SW_Pos                  (0U)
#define RCC_CFGR_SW_Msk                  (0x3UL << RCC_CFGR_SW_Pos)            /*!< 0x00000003 */
#define RCC_CFGR_SW                      RCC_CFGR_SW_Msk                       /*!< SW[1:0] bits (System clock Switch) */
#define RCC_CFGR_SW_0                    (0x1UL << RCC_CFGR_SW_Pos)            /*!< 0x00000001 */
#define RCC_CFGR_SW_1                    (0x2UL << RCC_CFGR_SW_Pos)            /*!< 0x00000002 */
#define RCC_CFGR_SW_MSI                  (0x00000000UL)                        /*!< MSI oscillator selection as system clock */
#define RCC_CFGR_SW_HSI                  (0x00000001UL)                        /*!< HSI16 oscillator selection as system clock */
#define RCC_CFGR_SW_HSE                  (0x00000002UL)                        /*!< HSE oscillator selection as system clock */
#define RCC_CFGR_SW_PLL                  (0x00000003UL)                        /*!< PLL selection as system clock */

#define RCC_CFGR_SWS_Pos                 (2U)
#define RCC_CFGR_SWS_Msk                 (0x3UL << RCC_CFGR_SWS_Pos)           /*!< 0x0000000C */
#define RCC_CFGR_SWS                     RCC_CFGR_SWS_Msk                      /*!< SWS[1:0] bits (System Clock Switch Status) */
#define RCC_CFGR_SWS_0                   (0x1UL << RCC_CFGR_SWS_Pos)           /*!< 0x00000004 */
#define RCC_CFGR_SWS_1                   (0x2UL << RCC_CFGR_SWS_Pos)           /*!< 0x00000008 */
#define RCC_CFGR_SWS_MSI                 (0x00000000UL)                        /*!< MSI oscillator used as system clock */
#define RCC_CFGR_SWS_HSI                 (0x00000004UL)                        /*!< HSI16 oscillator used as system clock */
#define RCC_CFGR_SWS_HSE                 (0x00000008UL)                        /*!< HSE oscillator used as system clock */
#define RCC_CFGR_SWS_PLL                 (0x0000000CUL)                        /*!< PLL used as system clock */

/********************  Bit definition for RCC_AHB2ENR register  ***************/
#define RCC_AHB2ENR_GPIOAEN_Pos          (0U)
#define RCC_AHB2ENR_GPIOAEN_Msk          (0x1UL << RCC_AHB2ENR_GPIOAEN_Pos)    /*!< 0x00000001 */
#define RCC_AHB2ENR_GPIOAEN              RCC_AHB2ENR_GPIOAEN_Msk               /*!< IO port A clock enable */
#define RCC_AHB2ENR_GPIOBEN_Pos          (1U)
#define RCC_AHB2ENR_GPIOBEN_Msk          (0x1UL << RCC_AHB2ENR_GPIOBEN_Pos)    /*!< 0x00000002 */
#define RCC_AHB2ENR_GPIOBEN              RCC_AHB2ENR_GPIOBEN_Msk               /*!< IO port B clock enable */
#define RCC_AHB2ENR_GPIOCEN_Pos          (2U)
#define RCC_AHB2ENR_GPIOCEN_Msk          (0x1UL << RCC_AHB2ENR_GPIOCEN_Pos)    /*!< 0x00000004 */
#define RCC_AHB2ENR_GPIOCEN              RCC_AHB2ENR_GPIOCEN_Msk               /*!< IO port C clock enable */
#define RCC_AHB2ENR_GPIODEN_Pos          (3U)
#define RCC_AHB2ENR_GPIODEN_Msk          (0x1UL << RCC_AHB2ENR_GPIODEN_Pos)    /*!< 0x00000008 */
#define RCC_AHB2ENR_GPIODEN              RCC_AHB2ENR_GPIODEN_Msk               /*!< IO port D clock enable */
#define RCC_AHB2ENR_GPIOEEN_Pos          (4U)
#define RCC_AHB2ENR_GPIOEEN_Msk          (0x1UL << RCC_AHB2ENR_GPIOEEN_Pos)    /*!< 0x00000010 */
#define RCC_AHB2ENR_GPIOEEN              RCC_AHB2ENR_GPIOEEN_Msk               /*!< IO port E clock enable */
#define RCC_AHB2ENR_GPIOFEN_Pos          (5U)
#define RCC_AHB2ENR_GPIOFEN_Msk          (0x1UL << RCC_AHB2ENR_GPIOFEN_Pos)    /*!< 0x00000020 */
#define RCC_AHB2ENR_GPIOFEN              RCC_AHB2ENR_GPIOFEN_Msk               /*!< IO port F clock enable */
#define RCC_AHB2ENR_GPIOGEN_Pos          (6U)
#define RCC_AHB2ENR_GPIOGEN_Msk          (0x1UL << RCC_AHB2ENR_GPIOGEN_Pos)    /*!< 0x00000040 */
#define RCC_AHB2ENR_GPIOGEN              RCC_AHB2ENR_GPIOGEN_Msk               /*!< IO port G clock enable */
#define RCC_AHB2ENR_GPIOHEN_Pos          (7U)
#define RCC_AHB2ENR_GPIOHEN_Msk          (0x1UL << RCC_AHB2ENR_GPIOHEN_Pos)    /*!< 0x00000080 */
#define RCC_AHB2ENR_GPIOHEN              RCC_AHB2ENR_GPIOHEN_Msk               /*!< IO port H clock enable */
#define RCC_AHB2ENR_ADCEN_Pos            (13U)
#define RCC_AHB2ENR_ADCEN_Msk            (0x1UL << RCC_AHB2ENR_ADCEN_Pos)      /*!< 0x00002000 */
#define RCC_AHB2ENR_ADCEN                RCC_AHB2ENR_ADCEN_Msk                 /*!< ADC 1 clock enable */

/********************  Bit definition for RCC_APB1ENR1 register  **************/
#define RCC_APB1ENR1_TIM2EN_Pos          (0U)
#define RCC_APB1ENR1_TIM2EN_Msk          (0x1UL << RCC_APB1ENR1_TIM2EN_Pos)    /*!< 0x00000001 */
#define RCC_APB1ENR1_TIM2EN              RCC_APB1ENR1_TIM2EN_Msk               /*!< TIM2 clock enable */
#define RCC_APB1ENR1_TIM3EN_Pos          (1U)
#define RCC_APB1ENR1_TIM3EN_Msk          (0x1UL << RCC_APB1ENR1_TIM3EN_Pos)    /*!< 0x00000002 */
#define RCC_APB1ENR1_TIM3EN              RCC_APB1ENR1_TIM3EN_Msk               /*!< TIM3 clock enable */
#define RCC_APB1ENR1_TIM4EN_Pos          (2U)
#define RCC_APB1ENR1_TIM4EN_Msk          (0x1UL << RCC_APB1ENR1_TIM4EN_Pos)    /*!< 0x00000004 */
#define RCC_APB1ENR1_TIM4EN              RCC_APB1ENR1_TIM4EN_Msk               /*!< TIM4 clock enable */
#define RCC_APB1ENR1_TIM5EN_Pos          (3U)
#define RCC_APB1ENR1_TIM5EN_Msk          (0x1UL << RCC_APB1ENR1_TIM5EN_Pos)    /*!< 0x00000008 */
#define RCC_APB1ENR1_TIM5EN              RCC_APB1ENR1_TIM5EN_Msk               /*!< TIM5 clock enable */
#define RCC_APB1ENR1_TIM6EN_Pos          (4U)
#define RCC_APB1ENR1_TIM6EN_Msk          (0x1UL << RCC_APB1ENR1_TIM6EN_Pos)    /*!< 0x00000010 */
#define RCC_APB1ENR1_TIM6EN              RCC_APB1ENR1_TIM6EN_Msk               /*!< TIM6 clock enable */
#define RCC_APB1ENR1_TIM7EN_Pos          (5U)
#define RCC_APB1ENR1_TIM7EN_Msk          (0x1UL << RCC_APB1ENR1_TIM7EN_Pos)    /*!< 0x00000020 */
#define RCC_APB1ENR1_TIM7EN              RCC_APB1ENR1_TIM7EN_Msk               /*!< TIM7 clock enable */
#define RCC_APB1ENR1_WWDGEN_Pos          (11U)
#define RCC_APB1ENR1_WWDGEN_Msk          (0x1UL << RCC_APB1ENR1_WWDGEN_Pos)    /*!< 0x00000800 */
#define RCC_APB1ENR1_WWDGEN              RCC_APB1ENR1_WWDGEN_Msk               /*!< Window Watchdog clock enable */
#define RCC_APB1ENR1_SPI2EN_Pos          (14U)
#define RCC_APB1ENR1_SPI2EN_Msk          (0x1UL << RCC_APB1ENR1_SPI2EN_Pos)    /*!< 0x00004000 */
#define RCC_APB1ENR1_SPI2EN              RCC_APB1ENR1_SPI2EN_Msk               /*!< SPI2 clock enable */
#define RCC_APB1ENR1_SPI3EN_Pos          (15U)
#define RCC_APB1ENR1_SPI3EN_Msk          (0x1UL << RCC_APB1ENR1_SPI3EN_Pos)    /*!< 0x00008000 */
#define RCC_APB1ENR1_SPI3EN              RCC_APB1ENR1_SPI3EN_Msk               /*!< SPI3 clock enable */
#define RCC_APB1ENR1_USART2EN_Pos        (17U)
#define RCC_APB1ENR1_USART2EN_Msk        (0x1UL << RCC_APB1ENR1_USART2EN_Pos)  /*!< 0x00020000 */
#define RCC_APB1ENR1_USART2EN            RCC_APB1ENR1_USART2EN_Msk             /*!< USART2 clock enable */
#define RCC_APB1ENR1_USART3EN_Pos        (18U)
#define RCC_APB1ENR1_USART3EN_Msk        (0x1UL << RCC_APB1ENR1_USART3EN_Pos)  /*!< 0x00040000 */
#define RCC_APB1ENR1_USART3EN            RCC_APB1ENR1_USART3EN_Msk             /*!< USART3 clock enable */
#define RCC_APB1ENR1_UART4EN_Pos         (19U)
#define RCC_APB1ENR1_UART4EN_Msk         (0x1UL << RCC_APB1ENR1_UART4EN_Pos)   /*!< 0x00080000 */
#define RCC_APB1ENR1_UART4EN             RCC_APB1ENR1_UART4EN_Msk              /*!< UART4 clock enable */
#define RCC_APB1ENR1_UART5EN_Pos         (20U)
#define RCC_APB1ENR1_UART5EN_Msk         (0x1UL << RCC_APB1ENR1_UART5EN_Pos)   /*!< 0x00100000 */
#define RCC_APB1ENR1_UART5EN             RCC_APB1ENR1_UART5EN_Msk              /*!< UART5 clock enable */
#define RCC_APB1ENR1_I2C1EN_Pos          (21U)
#define RCC_APB1ENR1_I2C1EN_Msk          (0x1UL << RCC_APB1ENR1_I2C1EN_Pos)    /*!< 0x00200000 */
#define RCC_APB1ENR1_I2C1EN              RCC_APB1ENR1_I2C1EN_Msk               /*!< I2C1 clock enable */
#define RCC_APB1ENR1_I2C2EN_Pos          (22U)
#define RCC_APB1ENR1_I2C2EN_Msk          (0x1UL << RCC_APB1ENR1_I2C2EN_Pos)    /*!< 0x00400000 */
#define RCC_APB1ENR1_I2C2EN              RCC_APB1ENR1_I2C2EN_Msk               /*!< I2C2 clock enable */
#define RCC_APB1ENR1_I2C3EN_Pos          (23U)
#define RCC_APB1ENR1_I2C3EN_Msk          (0x1UL << RCC_APB1ENR1_I2C3EN_Pos)    /*!< 0x00800000 */
#define RCC_APB1ENR1_I2C3EN              RCC_APB1ENR1_I2C3EN_Msk               /*!< I2C3 clock enable */
#define RCC_APB1ENR1_PWREN_Pos           (28U)
#define RCC_APB1ENR1_PWREN_Msk           (0x1UL << RCC_APB1ENR1_PWREN_Pos)     /*!< 0x10000000 */
#define RCC_APB1ENR1_PWREN               RCC_APB1ENR1_PWREN_Msk                /*!< Power interface clock enable */

/********************  Bit definition for RCC_APB2ENR register  ***************/
#define RCC_APB2ENR_TIM1EN_Pos           (11U)
#define RCC_APB2ENR_TIM1EN_Msk           (0x1UL << RCC_APB2ENR_TIM1EN_Pos)     /*!< 0x00000800 */
#define RCC_APB2ENR_TIM1EN               RCC_APB2ENR_TIM1EN_Msk                /*!< TIM1 clock enable */
#define RCC_APB2ENR_TIM8EN_Pos           (13U)
#define RCC_APB2ENR_TIM8EN_Msk           (0x1UL << RCC_APB2ENR_TIM8EN_Pos)     /*!< 0x00002000 */
#define RCC_APB2ENR_TIM8EN               RCC_APB2ENR_TIM8EN_Msk                /*!< TIM8 clock enable */
#define RCC_APB2ENR_USART1EN_Pos         (14U)
#define RCC_APB2ENR_USART1EN_Msk         (0x1UL << RCC_APB2ENR_USART1EN_Pos)   /*!< 0x00004000 */
#define RCC_APB2ENR_USART1EN             RCC_APB2ENR_USART1EN_Msk              /*!< USART1 clock enable */
#define RCC_APB2ENR_SPI1EN_Pos           (12U)
#define RCC_APB2ENR_SPI1EN_Msk           (0x1UL << RCC_APB2ENR_SPI1EN_Pos)     /*!< 0x00001000 */
#define RCC_APB2ENR_SPI1EN               RCC_APB2ENR_SPI1EN_Msk                /*!< SPI1 clock enable */

/******************************************************************************/
/*                                                                            */
/*                       General Purpose IOs (GPIO)                           */
/*                                                                            */
/******************************************************************************/
/******************  Bits definition for GPIO_MODER register  *****************/
#define GPIO_MODER_MODE0_Pos           (0U)
#define GPIO_MODER_MODE0_Msk           (0x3UL << GPIO_MODER_MODE0_Pos)          /*!< 0x00000003 */
#define GPIO_MODER_MODE0               GPIO_MODER_MODE0_Msk
#define GPIO_MODER_MODE0_0             (0x1UL << GPIO_MODER_MODE0_Pos)          /*!< 0x00000001 */
#define GPIO_MODER_MODE0_1             (0x2UL << GPIO_MODER_MODE0_Pos)          /*!< 0x00000002 */

/* GPIO Mode definitions */
#define GPIO_MODE_INPUT                ((uint32_t)0x00000000U)   /*!< Input Mode */
#define GPIO_MODE_OUTPUT               ((uint32_t)0x00000001U)   /*!< Output Mode */
#define GPIO_MODE_AF                   ((uint32_t)0x00000002U)   /*!< Alternate Function Mode */
#define GPIO_MODE_ANALOG               ((uint32_t)0x00000003U)   /*!< Analog Mode  */

/* GPIO Pull-Up Pull-Down definitions */
#define GPIO_NOPULL                    ((uint32_t)0x00000000U)   /*!< No Pull-up or Pull-down activation  */
#define GPIO_PULLUP                    ((uint32_t)0x00000001U)   /*!< Pull-up activation                  */
#define GPIO_PULLDOWN                  ((uint32_t)0x00000002U)   /*!< Pull-down activation                */

/* GPIO Output type definitions */
#define GPIO_OTYPE_PP                  ((uint32_t)0x00000000U)   /*!< Push-Pull */
#define GPIO_OTYPE_OD                  ((uint32_t)0x00000001U)   /*!< Open Drain */

/* GPIO Speed definitions */
#define GPIO_SPEED_FREQ_LOW            ((uint32_t)0x00000000U)   /*!< Low speed     */
#define GPIO_SPEED_FREQ_MEDIUM         ((uint32_t)0x00000001U)   /*!< Medium speed  */
#define GPIO_SPEED_FREQ_HIGH           ((uint32_t)0x00000002U)   /*!< High speed    */
#define GPIO_SPEED_FREQ_VERY_HIGH      ((uint32_t)0x00000003U)   /*!< Very high speed */

/******************  Bits definition for GPIO_IDR register  *******************/
#define GPIO_IDR_ID0_Pos               (0U)
#define GPIO_IDR_ID0_Msk               (0x1UL << GPIO_IDR_ID0_Pos)              /*!< 0x00000001 */
#define GPIO_IDR_ID0                   GPIO_IDR_ID0_Msk

/******************  Bits definition for GPIO_ODR register  *******************/
#define GPIO_ODR_OD0_Pos               (0U)
#define GPIO_ODR_OD0_Msk               (0x1UL << GPIO_ODR_OD0_Pos)              /*!< 0x00000001 */
#define GPIO_ODR_OD0                   GPIO_ODR_OD0_Msk

/******************  Bits definition for GPIO_BSRR register  ******************/
#define GPIO_BSRR_BS0_Pos              (0U)
#define GPIO_BSRR_BS0_Msk              (0x1UL << GPIO_BSRR_BS0_Pos)             /*!< 0x00000001 */
#define GPIO_BSRR_BS0                  GPIO_BSRR_BS0_Msk
#define GPIO_BSRR_BR0_Pos              (16U)
#define GPIO_BSRR_BR0_Msk              (0x1UL << GPIO_BSRR_BR0_Pos)             /*!< 0x00010000 */
#define GPIO_BSRR_BR0                  GPIO_BSRR_BR0_Msk

/******************************************************************************/
/*                                                                            */
/*                      Inter-integrated Circuit Interface (I2C)              */
/*                                                                            */
/******************************************************************************/
/*******************  Bit definition for I2C_CR1 register  *******************/
#define I2C_CR1_PE_Pos                  (0U)
#define I2C_CR1_PE_Msk                  (0x1UL << I2C_CR1_PE_Pos)                /*!< 0x00000001 */
#define I2C_CR1_PE                      I2C_CR1_PE_Msk                           /*!< Peripheral enable */

/*******************  Bit definition for I2C_CR2 register  *******************/
#define I2C_CR2_SADD_Pos                (0U)
#define I2C_CR2_SADD_Msk                (0x3FFUL << I2C_CR2_SADD_Pos)            /*!< 0x000003FF */
#define I2C_CR2_SADD                    I2C_CR2_SADD_Msk                         /*!< Slave address (master mode) */
#define I2C_CR2_RD_WRN_Pos              (10U)
#define I2C_CR2_RD_WRN_Msk              (0x1UL << I2C_CR2_RD_WRN_Pos)            /*!< 0x00000400 */
#define I2C_CR2_RD_WRN                  I2C_CR2_RD_WRN_Msk                       /*!< Transfer direction (master mode) */
#define I2C_CR2_NBYTES_Pos              (16U)
#define I2C_CR2_NBYTES_Msk              (0xFFUL << I2C_CR2_NBYTES_Pos)           /*!< 0x00FF0000 */
#define I2C_CR2_NBYTES                  I2C_CR2_NBYTES_Msk                       /*!< Number of bytes */
#define I2C_CR2_START_Pos               (13U)
#define I2C_CR2_START_Msk               (0x1UL << I2C_CR2_START_Pos)             /*!< 0x00002000 */
#define I2C_CR2_START                   I2C_CR2_START_Msk                        /*!< START generation */
#define I2C_CR2_STOP_Pos                (14U)
#define I2C_CR2_STOP_Msk                (0x1UL << I2C_CR2_STOP_Pos)              /*!< 0x00004000 */
#define I2C_CR2_STOP                    I2C_CR2_STOP_Msk                         /*!< STOP generation (master mode) */

/*******************  Bit definition for I2C_ISR register  *******************/
#define I2C_ISR_TXE_Pos                 (0U)
#define I2C_ISR_TXE_Msk                 (0x1UL << I2C_ISR_TXE_Pos)               /*!< 0x00000001 */
#define I2C_ISR_TXE                     I2C_ISR_TXE_Msk                          /*!< Transmit data register empty */
#define I2C_ISR_TXIS_Pos                (1U)
#define I2C_ISR_TXIS_Msk                (0x1UL << I2C_ISR_TXIS_Pos)              /*!< 0x00000002 */
#define I2C_ISR_TXIS                    I2C_ISR_TXIS_Msk                         /*!< Transmit interrupt status */
#define I2C_ISR_RXNE_Pos                (2U)
#define I2C_ISR_RXNE_Msk                (0x1UL << I2C_ISR_RXNE_Pos)              /*!< 0x00000004 */
#define I2C_ISR_RXNE                    I2C_ISR_RXNE_Msk                         /*!< Receive data register not empty */
#define I2C_ISR_TC_Pos                  (6U)
#define I2C_ISR_TC_Msk                  (0x1UL << I2C_ISR_TC_Pos)                /*!< 0x00000040 */
#define I2C_ISR_TC                      I2C_ISR_TC_Msk                           /*!< Transfer complete */
#define I2C_ISR_BUSY_Pos                (15U)
#define I2C_ISR_BUSY_Msk                (0x1UL << I2C_ISR_BUSY_Pos)              /*!< 0x00008000 */
#define I2C_ISR_BUSY                    I2C_ISR_BUSY_Msk                         /*!< Bus busy */

/******************************************************************************/
/*                                                                            */
/*                          Serial Peripheral Interface (SPI)                 */
/*                                                                            */
/******************************************************************************/
/*******************  Bit definition for SPI_CR1 register  ********************/
#define SPI_CR1_SPE_Pos                 (6U)
#define SPI_CR1_SPE_Msk                 (0x1UL << SPI_CR1_SPE_Pos)               /*!< 0x00000040 */
#define SPI_CR1_SPE                     SPI_CR1_SPE_Msk                          /*!< SPI Enable */
#define SPI_CR1_BR_Pos                  (3U)
#define SPI_CR1_BR_Msk                  (0x7UL << SPI_CR1_BR_Pos)                /*!< 0x00000038 */
#define SPI_CR1_BR                      SPI_CR1_BR_Msk                           /*!< BR[2:0] bits (Baud Rate Control) */
#define SPI_CR1_MSTR_Pos                (2U)
#define SPI_CR1_MSTR_Msk                (0x1UL << SPI_CR1_MSTR_Pos)              /*!< 0x00000004 */
#define SPI_CR1_MSTR                    SPI_CR1_MSTR_Msk                         /*!< Master Selection */
#define SPI_CR1_CPOL_Pos                (1U)
#define SPI_CR1_CPOL_Msk                (0x1UL << SPI_CR1_CPOL_Pos)              /*!< 0x00000002 */
#define SPI_CR1_CPOL                    SPI_CR1_CPOL_Msk                         /*!< Clock Polarity */
#define SPI_CR1_CPHA_Pos                (0U)
#define SPI_CR1_CPHA_Msk                (0x1UL << SPI_CR1_CPHA_Pos)              /*!< 0x00000001 */
#define SPI_CR1_CPHA                    SPI_CR1_CPHA_Msk                         /*!< Clock Phase */

/*******************  Bit definition for SPI_SR register  *********************/
#define SPI_SR_RXNE_Pos                 (0U)
#define SPI_SR_RXNE_Msk                 (0x1UL << SPI_SR_RXNE_Pos)               /*!< 0x00000001 */
#define SPI_SR_RXNE                     SPI_SR_RXNE_Msk                          /*!< Receive buffer Not Empty */
#define SPI_SR_TXE_Pos                  (1U)
#define SPI_SR_TXE_Msk                  (0x1UL << SPI_SR_TXE_Pos)                /*!< 0x00000002 */
#define SPI_SR_TXE                      SPI_SR_TXE_Msk                           /*!< Transmit buffer Empty */
#define SPI_SR_BSY_Pos                  (7U)
#define SPI_SR_BSY_Msk                  (0x1UL << SPI_SR_BSY_Pos)                /*!< 0x00000080 */
#define SPI_SR_BSY                      SPI_SR_BSY_Msk                           /*!< Busy flag */

/******************************************************************************/
/*                                                                            */
/*                       Universal Synchronous Asynchronous Receiver Transmitter (USART) */
/*                                                                            */
/******************************************************************************/
/******************  Bit definition for USART_CR1 register  *******************/
#define USART_CR1_UE_Pos                (0U)
#define USART_CR1_UE_Msk                (0x1UL << USART_CR1_UE_Pos)              /*!< 0x00000001 */
#define USART_CR1_UE                    USART_CR1_UE_Msk                         /*!< USART Enable */
#define USART_CR1_RE_Pos                (2U)
#define USART_CR1_RE_Msk                (0x1UL << USART_CR1_RE_Pos)              /*!< 0x00000004 */
#define USART_CR1_RE                    USART_CR1_RE_Msk                         /*!< Receiver Enable */
#define USART_CR1_TE_Pos                (3U)
#define USART_CR1_TE_Msk                (0x1UL << USART_CR1_TE_Pos)              /*!< 0x00000008 */
#define USART_CR1_TE                    USART_CR1_TE_Msk                         /*!< Transmitter Enable */
#define USART_CR1_RXNEIE_Pos            (5U)
#define USART_CR1_RXNEIE_Msk            (0x1UL << USART_CR1_RXNEIE_Pos)          /*!< 0x00000020 */
#define USART_CR1_RXNEIE                USART_CR1_RXNEIE_Msk                     /*!< RXNE Interrupt Enable */
#define USART_CR1_TXEIE_Pos             (7U)
#define USART_CR1_TXEIE_Msk             (0x1UL << USART_CR1_TXEIE_Pos)           /*!< 0x00000080 */
#define USART_CR1_TXEIE                 USART_CR1_TXEIE_Msk                      /*!< TXE Interrupt Enable */
#define USART_CR1_M0_Pos                (12U)
#define USART_CR1_M0_Msk                (0x1UL << USART_CR1_M0_Pos)              /*!< 0x00001000 */
#define USART_CR1_M0                    USART_CR1_M0_Msk                         /*!< Word length bit 0 */
#define USART_CR1_OVER8_Pos             (15U)
#define USART_CR1_OVER8_Msk             (0x1UL << USART_CR1_OVER8_Pos)           /*!< 0x00008000 */
#define USART_CR1_OVER8                 USART_CR1_OVER8_Msk                      /*!< Oversampling by 8-bit or 16-bit mode */

/******************  Bit definition for USART_ISR register  *******************/
#define USART_ISR_TXE_Pos               (7U)
#define USART_ISR_TXE_Msk               (0x1UL << USART_ISR_TXE_Pos)             /*!< 0x00000080 */
#define USART_ISR_TXE                   USART_ISR_TXE_Msk                        /*!< Transmit Data Register Empty */
#define USART_ISR_RXNE_Pos              (5U)
#define USART_ISR_RXNE_Msk              (0x1UL << USART_ISR_RXNE_Pos)            /*!< 0x00000020 */
#define USART_ISR_RXNE                  USART_ISR_RXNE_Msk                       /*!< Read Data Register Not Empty */
#define USART_ISR_TC_Pos                (6U)
#define USART_ISR_TC_Msk                (0x1UL << USART_ISR_TC_Pos)              /*!< 0x00000040 */
#define USART_ISR_TC                    USART_ISR_TC_Msk                         /*!< Transmission Complete */

/******************************************************************************/
/*                                                                            */
/*                        Analog to Digital Converter (ADC)                   */
/*                                                                            */
/******************************************************************************/
/********************  Bit definition for ADC_CR register  ********************/
#define ADC_CR_ADEN_Pos                 (0U)
#define ADC_CR_ADEN_Msk                 (0x1UL << ADC_CR_ADEN_Pos)               /*!< 0x00000001 */
#define ADC_CR_ADEN                     ADC_CR_ADEN_Msk                          /*!< ADC enable */
#define ADC_CR_ADDIS_Pos                (1U)
#define ADC_CR_ADDIS_Msk                (0x1UL << ADC_CR_ADDIS_Pos)              /*!< 0x00000002 */
#define ADC_CR_ADDIS                    ADC_CR_ADDIS_Msk                         /*!< ADC disable */
#define ADC_CR_ADSTART_Pos              (2U)
#define ADC_CR_ADSTART_Msk              (0x1UL << ADC_CR_ADSTART_Pos)            /*!< 0x00000004 */
#define ADC_CR_ADSTART                  ADC_CR_ADSTART_Msk                       /*!< ADC group regular conversion start */
#define ADC_CR_ADSTP_Pos                (4U)
#define ADC_CR_ADSTP_Msk                (0x1UL << ADC_CR_ADSTP_Pos)              /*!< 0x00000010 */
#define ADC_CR_ADSTP                    ADC_CR_ADSTP_Msk                         /*!< ADC group regular conversion stop */

/********************  Bit definition for ADC_ISR register  *******************/
#define ADC_ISR_ADRDY_Pos               (0U)
#define ADC_ISR_ADRDY_Msk               (0x1UL << ADC_ISR_ADRDY_Pos)             /*!< 0x00000001 */
#define ADC_ISR_ADRDY                   ADC_ISR_ADRDY_Msk                        /*!< ADC ready flag */
#define ADC_ISR_EOC_Pos                 (2U)
#define ADC_ISR_EOC_Msk                 (0x1UL << ADC_ISR_EOC_Pos)               /*!< 0x00000004 */
#define ADC_ISR_EOC                     ADC_ISR_EOC_Msk                          /*!< ADC group regular end of conversion flag */
#define ADC_ISR_EOS_Pos                 (3U)
#define ADC_ISR_EOS_Msk                 (0x1UL << ADC_ISR_EOS_Pos)               /*!< 0x00000008 */
#define ADC_ISR_EOS                     ADC_ISR_EOS_Msk                          /*!< ADC group regular end of sequence conversions flag */

/******************************************************************************/
/*                                                                            */
/*                                Timers (TIM)                                */
/*                                                                            */
/******************************************************************************/
/*******************  Bit definition for TIM_CR1 register  ********************/
#define TIM_CR1_CEN_Pos                 (0U)
#define TIM_CR1_CEN_Msk                 (0x1UL << TIM_CR1_CEN_Pos)               /*!< 0x00000001 */
#define TIM_CR1_CEN                     TIM_CR1_CEN_Msk                          /*!<Counter enable */
#define TIM_CR1_ARPE_Pos                (7U)
#define TIM_CR1_ARPE_Msk                (0x1UL << TIM_CR1_ARPE_Pos)              /*!< 0x00000080 */
#define TIM_CR1_ARPE                    TIM_CR1_ARPE_Msk                         /*!<Auto-reload preload enable */

/*******************  Bit definition for TIM_DIER register  *******************/
#define TIM_DIER_UIE_Pos                (0U)
#define TIM_DIER_UIE_Msk                (0x1UL << TIM_DIER_UIE_Pos)              /*!< 0x00000001 */
#define TIM_DIER_UIE                    TIM_DIER_UIE_Msk                         /*!<Update interrupt enable */

/*******************  Bit definition for TIM_SR register  *********************/
#define TIM_SR_UIF_Pos                  (0U)
#define TIM_SR_UIF_Msk                  (0x1UL << TIM_SR_UIF_Pos)                /*!< 0x00000001 */
#define TIM_SR_UIF                      TIM_SR_UIF_Msk                           /*!<Update interrupt Flag */

/*******************  Bit definition for TIM_CCMR1 register  ******************/
#define TIM_CCMR1_OC1M_Pos              (4U)
#define TIM_CCMR1_OC1M_Msk              (0x7UL << TIM_CCMR1_OC1M_Pos)            /*!< 0x00000070 */
#define TIM_CCMR1_OC1M                  TIM_CCMR1_OC1M_Msk                       /*!<OC1M[2:0] bits (Output Compare 1 Mode) */
#define TIM_CCMR1_OC1M_1                (0x2UL << TIM_CCMR1_OC1M_Pos)            /*!< 0x00000020 - PWM mode 1 */
#define TIM_CCMR1_OC1M_2                (0x3UL << TIM_CCMR1_OC1M_Pos)            /*!< 0x00000030 - PWM mode 2 */
#define TIM_CCMR1_OC1PE_Pos             (3U)
#define TIM_CCMR1_OC1PE_Msk             (0x1UL << TIM_CCMR1_OC1PE_Pos)           /*!< 0x00000008 */
#define TIM_CCMR1_OC1PE                 TIM_CCMR1_OC1PE_Msk                      /*!<Output Compare 1 Preload enable */

/*******************  Bit definition for TIM_CCER register  *******************/
#define TIM_CCER_CC1E_Pos               (0U)
#define TIM_CCER_CC1E_Msk               (0x1UL << TIM_CCER_CC1E_Pos)             /*!< 0x00000001 */
#define TIM_CCER_CC1E                   TIM_CCER_CC1E_Msk                        /*!<Capture/Compare 1 output enable */

/******************************************************************************/
/*                                                                            */
/*                      Independent WATCHDOG (IWDG)                           */
/*                                                                            */
/******************************************************************************/
/*******************  Bit definition for IWDG_KR register  ********************/
#define IWDG_KR_KEY_Pos                 (0U)
#define IWDG_KR_KEY_Msk                 (0xFFFFUL << IWDG_KR_KEY_Pos)            /*!< 0x0000FFFF */
#define IWDG_KR_KEY                     IWDG_KR_KEY_Msk                          /*!<Key value (write only, read 0000h) */

/*******************  Bit definition for IWDG_PR register  ********************/
#define IWDG_PR_PR_Pos                  (0U)
#define IWDG_PR_PR_Msk                  (0x7UL << IWDG_PR_PR_Pos)                /*!< 0x00000007 */
#define IWDG_PR_PR                      IWDG_PR_PR_Msk                           /*!<PR[2:0] (Prescaler divider) */

/*******************  Bit definition for IWDG_RLR register  *******************/
#define IWDG_RLR_RL_Pos                 (0U)
#define IWDG_RLR_RL_Msk                 (0xFFFUL << IWDG_RLR_RL_Pos)             /*!< 0x00000FFF */
#define IWDG_RLR_RL                     IWDG_RLR_RL_Msk                          /*!<Watchdog counter reload value */

/*******************  Bit definition for IWDG_SR register  ********************/
#define IWDG_SR_PVU_Pos                 (0U)
#define IWDG_SR_PVU_Msk                 (0x1UL << IWDG_SR_PVU_Pos)               /*!< 0x00000001 */
#define IWDG_SR_PVU                     IWDG_SR_PVU_Msk                          /*!<Watchdog prescaler value update */
#define IWDG_SR_RVU_Pos                 (1U)
#define IWDG_SR_RVU_Msk                 (0x1UL << IWDG_SR_RVU_Pos)               /*!< 0x00000002 */
#define IWDG_SR_RVU                     IWDG_SR_RVU_Msk                          /*!<Watchdog counter reload value update */

/* IWDG Key values */
#define IWDG_KEY_RELOAD                 0xAAAAU  /*!< IWDG Reload Counter Enable   */
#define IWDG_KEY_ENABLE                 0xCCCCU  /*!< IWDG Peripheral Enable        */
#define IWDG_KEY_WRITE_ACCESS_ENABLE    0x5555U  /*!< IWDG KR Write Access Enable   */
#define IWDG_KEY_WRITE_ACCESS_DISABLE   0x0000U  /*!< IWDG KR Write Access Disable  */

/******************************************************************************/
/*                                                                            */
/*                                    FLASH                                   */
/*                                                                            */
/******************************************************************************/
/*******************  Bits definition for FLASH_ACR register  *****************/
#define FLASH_ACR_LATENCY_Pos           (0U)
#define FLASH_ACR_LATENCY_Msk           (0x7UL << FLASH_ACR_LATENCY_Pos)        /*!< 0x00000007 */
#define FLASH_ACR_LATENCY               FLASH_ACR_LATENCY_Msk
#define FLASH_ACR_PRFTEN_Pos            (8U)
#define FLASH_ACR_PRFTEN_Msk            (0x1UL << FLASH_ACR_PRFTEN_Pos)          /*!< 0x00000100 */
#define FLASH_ACR_PRFTEN                FLASH_ACR_PRFTEN_Msk                     /*!< Prefetch enable */
#define FLASH_ACR_ICEN_Pos              (9U)
#define FLASH_ACR_ICEN_Msk              (0x1UL << FLASH_ACR_ICEN_Pos)            /*!< 0x00000200 */
#define FLASH_ACR_ICEN                  FLASH_ACR_ICEN_Msk                       /*!< Instruction cache enable */
#define FLASH_ACR_DCEN_Pos              (10U)
#define FLASH_ACR_DCEN_Msk              (0x1UL << FLASH_ACR_DCEN_Pos)            /*!< 0x00000400 */
#define FLASH_ACR_DCEN                  FLASH_ACR_DCEN_Msk                       /*!< Data cache enable */

/**
  * @}
  */

/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __STM32L476xx_H */
