/*
 * adc12_a.h - MSP430 ADC12_A HAL Compatibility Stub
 * 
 * Copyright (C) 2019, SpaceLab.
 */

#ifndef HAL_ADC12_A_H_
#define HAL_ADC12_A_H_

#include <stdint.h>

/* Base address */
#define ADC12_A_BASE    0x0000

/* Sample/hold sources */
#define ADC12_A_SAMPLEHOLDSOURCE_SC     0
#define ADC12_A_SAMPLEHOLDSOURCE_1      1
#define ADC12_A_SAMPLEHOLDSOURCE_2      2
#define ADC12_A_SAMPLEHOLDSOURCE_3      3

/* Clock sources */
#define ADC12_A_CLOCKSOURCE_ADC12OSC    0
#define ADC12_A_CLOCKSOURCE_ACLK        1
#define ADC12_A_CLOCKSOURCE_MCLK        2
#define ADC12_A_CLOCKSOURCE_SMCLK       3

/* Clock dividers */
#define ADC12_A_CLOCKDIVIDER_1      0
#define ADC12_A_CLOCKDIVIDER_2      1
#define ADC12_A_CLOCKDIVIDER_3      2
#define ADC12_A_CLOCKDIVIDER_4      3
#define ADC12_A_CLOCKDIVIDER_5      4
#define ADC12_A_CLOCKDIVIDER_6      5
#define ADC12_A_CLOCKDIVIDER_7      6
#define ADC12_A_CLOCKDIVIDER_8      7

/* Resolution */
#define ADC12_A_RESOLUTION_8BIT     0
#define ADC12_A_RESOLUTION_10BIT    1
#define ADC12_A_RESOLUTION_12BIT    2

/* Sample/hold time */
#define ADC12_A_CYCLEHOLD_4_CYCLES      0
#define ADC12_A_CYCLEHOLD_8_CYCLES      1
#define ADC12_A_CYCLEHOLD_16_CYCLES     2
#define ADC12_A_CYCLEHOLD_32_CYCLES     3
#define ADC12_A_CYCLEHOLD_64_CYCLES     4
#define ADC12_A_CYCLEHOLD_96_CYCLES     5
#define ADC12_A_CYCLEHOLD_128_CYCLES    6
#define ADC12_A_CYCLEHOLD_192_CYCLES    7
#define ADC12_A_CYCLEHOLD_256_CYCLES    8
#define ADC12_A_CYCLEHOLD_384_CYCLES    9
#define ADC12_A_CYCLEHOLD_512_CYCLES    10
#define ADC12_A_CYCLEHOLD_768_CYCLES    11
#define ADC12_A_CYCLEHOLD_1024_CYCLES   12

/* Input channels */
#define ADC12_A_INPUT_A0    0
#define ADC12_A_INPUT_A1    1
#define ADC12_A_INPUT_A2    2
#define ADC12_A_INPUT_A3    3
#define ADC12_A_INPUT_A4    4
#define ADC12_A_INPUT_A5    5
#define ADC12_A_INPUT_A6    6
#define ADC12_A_INPUT_A7    7
#define ADC12_A_INPUT_VEREF_PLUS    14
#define ADC12_A_INPUT_VEREF_MINUS   15

/* Reference voltages */
#define ADC12_A_VREFPOS_AVCC    0
#define ADC12_A_VREFPOS_EXT     1
#define ADC12_A_VREFPOS_INT     2
#define ADC12_A_VREFNEG_AVSS    0
#define ADC12_A_VREFNEG_EXT     1

/* Memory buffer control */
#define ADC12_A_NOTENDOFSEQUENCE    0
#define ADC12_A_ENDOFSEQUENCE       1

/* Interrupts */
#define ADC12_A_MEMORY_0    0x01
#define ADC12_A_MEMORY_1    0x02
#define ADC12_A_MEMORY_2    0x04
#define ADC12_A_MEMORY_3    0x08

/* ADC init parameter structure */
typedef struct {
    uint8_t sampleHoldSignalSourceSelect;
    uint8_t clockSourceSelect;
    uint8_t clockSourceDivider;
    uint8_t clockSourcePredivider;
    uint16_t internalChannelMap;
} ADC12_A_initParam;

/* Memory configure parameter structure */
typedef struct {
    uint8_t memoryBufferControlIndex;
    uint8_t inputSourceSelect;
    uint8_t positiveRefVoltageSourceSelect;
    uint8_t negativeRefVoltageSourceSelect;
    uint8_t endOfSequence;
} ADC12_A_configureMemoryParam;

/* Stub function prototypes */
static inline uint8_t ADC12_A_init(uint16_t baseAddress, ADC12_A_initParam *param) { return 1; }
static inline void ADC12_A_enable(uint16_t baseAddress) { /* Stub */ }
static inline void ADC12_A_disable(uint16_t baseAddress) { /* Stub */ }
static inline void ADC12_A_setupSamplingTimer(uint16_t baseAddress, uint16_t clockCycleHoldCountLowMem, uint16_t clockCycleHoldCountHighMem, uint16_t multipleSamplesEnabled) { /* Stub */ }
static inline void ADC12_A_disableSamplingTimer(uint16_t baseAddress) { /* Stub */ }
static inline void ADC12_A_configureMemory(uint16_t baseAddress, ADC12_A_configureMemoryParam *param) { /* Stub */ }
static inline void ADC12_A_enableInterrupt(uint16_t baseAddress, uint16_t interruptMask) { /* Stub */ }
static inline void ADC12_A_disableInterrupt(uint16_t baseAddress, uint16_t interruptMask) { /* Stub */ }
static inline void ADC12_A_clearInterrupt(uint16_t baseAddress, uint16_t interruptMask) { /* Stub */ }
static inline void ADC12_A_startConversion(uint16_t baseAddress, uint8_t startingMemoryBufferIndex, uint8_t conversionSequenceModeSelect) { /* Stub */ }
static inline uint16_t ADC12_A_getResults(uint16_t baseAddress, uint8_t memoryBufferIndex) { return 0; }
static inline void ADC12_A_setResolution(uint16_t baseAddress, uint8_t resolutionSelect) { /* Stub */ }

#endif /* HAL_ADC12_A_H_ */
