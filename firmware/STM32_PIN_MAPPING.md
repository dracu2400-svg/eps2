# STM32L476RG Pin Mapping for EPS2

## Overview
This document defines the pin mapping from **MSP430F6659** to **STM32L476RG** for the EPS2 (Electrical Power System 2.0) firmware port.

**Date Created:** 2025-11-18
**Version:** 1.0
**Status:** Draft - Requires hardware verification

---

## Pin Mapping Strategy

### MSP430F6659 vs STM32L476RG GPIO Differences

| Feature | MSP430F6659 | STM32L476RG | Notes |
|---------|-------------|-------------|-------|
| **GPIO Ports** | P1-P11 (11 ports) | PA-PH (8 ports) | STM32 has fewer ports but more pins per port |
| **Pins per Port** | 8 pins | 16 pins | STM32 ports are wider |
| **Pin Naming** | P1.0, P2.3, etc. | PA0, PB3, etc. | Different nomenclature |
| **Configuration** | DIR, OUT, IN, REN, SEL | MODER, OTYPER, PUPDR, OSPEEDR, AFR | STM32 more granular |
| **Alternate Functions** | SEL0-SEL2 (3 bits) | AFR[0:1] (4 bits/pin = 16 AF) | STM32 has more AF options |
| **Max Frequency** | 25 MHz | 80 MHz | STM32 supports higher speeds |

---

## GPIO Pin Mapping Table

### Abstract GPIO Pins (from `drivers/gpio/gpio.h`)

The firmware uses abstract GPIO pin numbers (GPIO_PIN_0 through GPIO_PIN_63) which are mapped to physical pins.

| Abstract Pin | MSP430 Port.Pin | STM32 Port.Pin | Function/Signal | Direction | Notes |
|--------------|-----------------|----------------|-----------------|-----------|-------|
| **GPIO_PIN_0** | P1.0 | PA0 | LED_STATUS | Output | Status LED |
| **GPIO_PIN_1** | P1.1 | PA1 | LED_FAULT | Output | Fault indicator LED |
| **GPIO_PIN_2** | P1.2 | PA2 | HEATER_CTRL | Output | Heater control signal |
| **GPIO_PIN_3** | P1.3 | PA3 | MPPT_EN | Output | MPPT enable signal |
| **GPIO_PIN_4** | P1.4 | PA4 | CS_BAT_MON | Output | Battery monitor chip select |
| **GPIO_PIN_5** | P1.5 | PA5 | SCK_SPI1 | AF (SPI1) | SPI1 clock (AF5) |
| **GPIO_PIN_6** | P1.6 | PA6 | MISO_SPI1 | AF (SPI1) | SPI1 MISO (AF5) |
| **GPIO_PIN_7** | P1.7 | PA7 | MOSI_SPI1 | AF (SPI1) | SPI1 MOSI (AF5) |
| **GPIO_PIN_8** | P2.0 | PB0 | ADC_VBAT | Analog (ADC1_IN15) | Battery voltage sense |
| **GPIO_PIN_9** | P2.1 | PB1 | ADC_ISOLAR | Analog (ADC1_IN16) | Solar current sense |
| **GPIO_PIN_10** | P2.2 | PB2 | WDT_KICK | Output | External watchdog kick |
| **GPIO_PIN_11** | P2.3 | PB3 | CS_ADC_EXT | Output | External ADC chip select |
| **GPIO_PIN_12** | P2.4 | PB4 | I2C1_SDA_ALT | AF (I2C1) | I2C1 SDA (AF4) - alternate |
| **GPIO_PIN_13** | P2.5 | PB5 | I2C1_SCL_ALT | AF (I2C1) | I2C1 SCL (AF4) - alternate |
| **GPIO_PIN_14** | P2.6 | PB6 | UART1_TX | AF (USART1) | USART1 TX (AF7) |
| **GPIO_PIN_15** | P2.7 | PB7 | UART1_RX | AF (USART1) | USART1 RX (AF7) |
| **GPIO_PIN_16** | P3.0 | PB8 | I2C1_SCL | AF (I2C1) | I2C1 SCL (AF4) |
| **GPIO_PIN_17** | P3.1 | PB9 | I2C1_SDA | AF (I2C1) | I2C1 SDA (AF4) |
| **GPIO_PIN_18** | P3.2 | PB10 | UART3_TX | AF (USART3) | USART3 TX (AF7) |
| **GPIO_PIN_19** | P3.3 | PB11 | UART3_RX | AF (USART3) | USART3 RX (AF7) |
| **GPIO_PIN_20** | P3.4 | PB12 | CS_FLASH | Output | External flash chip select |
| **GPIO_PIN_21** | P3.5 | PB13 | SCK_SPI2 | AF (SPI2) | SPI2 clock (AF5) |
| **GPIO_PIN_22** | P3.6 | PB14 | MISO_SPI2 | AF (SPI2) | SPI2 MISO (AF5) |
| **GPIO_PIN_23** | P3.7 | PB15 | MOSI_SPI2 | AF (SPI2) | SPI2 MOSI (AF5) |
| **GPIO_PIN_24** | P4.0 | PC0 | ADC_TEMP1 | Analog (ADC1_IN1) | Temperature sensor 1 |
| **GPIO_PIN_25** | P4.1 | PC1 | ADC_TEMP2 | Analog (ADC1_IN2) | Temperature sensor 2 |
| **GPIO_PIN_26** | P4.2 | PC2 | ADC_VSOLAR | Analog (ADC1_IN3) | Solar voltage sense |
| **GPIO_PIN_27** | P4.3 | PC3 | ADC_ILOAD | Analog (ADC1_IN4) | Load current sense |
| **GPIO_PIN_28** | P4.4 | PC4 | PWM_CH1 | AF (TIM2_CH4) | PWM channel 1 (AF1) |
| **GPIO_PIN_29** | P4.5 | PC5 | PWM_CH2 | AF (TIM2_CH1) | PWM channel 2 (AF1) |
| **GPIO_PIN_30** | P4.6 | PC6 | UART6_TX | AF (USART6) | USART6 TX (AF8) - debug |
| **GPIO_PIN_31** | P4.7 | PC7 | UART6_RX | AF (USART6) | USART6 RX (AF8) - debug |

**Note:** GPIO_PIN_32 through GPIO_PIN_63 are currently unused but reserved for future expansion.

---

## Peripheral Alternate Function Assignments

### USART/UART Pins

| USART | TX Pin | AF | RX Pin | AF | Baud Rate | Usage |
|-------|--------|----|---------|----|-----------|-------|
| USART1 | PB6 | AF7 | PB7 | AF7 | 115200 | System log / Debug console |
| USART2 | PA2 | AF7 | PA3 | AF7 | 115200 | OBDH (On-Board Data Handling) |
| USART3 | PB10 | AF7 | PB11 | AF7 | 115200 | TTC (Telemetry, Tracking & Command) |

**Configuration:**
- **Mode:** 8N1 (8 data bits, no parity, 1 stop bit)
- **Flow Control:** None
- **Speed:** OSPEEDR = Very High (80 MHz capable)

### SPI Pins

| SPI | SCK Pin | AF | MISO Pin | AF | MOSI Pin | AF | Speed | Usage |
|-----|---------|----|-----------|----|----------|-------|-------|-------|
| SPI1 | PA5 | AF5 | PA6 | AF5 | PA7 | AF5 | 10 MHz | Battery monitors (DS2775G, DS277Xg) |
| SPI2 | PB13 | AF5 | PB14 | AF5 | PB15 | AF5 | 10 MHz | External ADC (ADS1248), Flash |

**Configuration:**
- **Mode:** Master
- **Clock Polarity:** CPOL = 0 (idle low)
- **Clock Phase:** CPHA = 0 (sample on first edge)
- **Data Size:** 8-bit
- **NSS:** Software managed (GPIO chip selects)
- **Prescaler:** fPCLK/8 = 10 MHz @ 80 MHz CPU

**Chip Select Mapping:**
- **PA4** - Battery monitor chip select
- **PB3** - External ADC (ADS1248) chip select
- **PB12** - External flash chip select

### I2C Pins

| I2C | SCL Pin | AF | SDA Pin | AF | Speed | Usage |
|-----|---------|----|---------|----|-------|-------|
| I2C1 | PB8 | AF4 | PB9 | AF4 | 100 kHz | Sensors (current, voltage, temperature) |
| I2C1_ALT | PB6 | AF4 | PB7 | AF4 | 100 kHz | Alternative pins (if needed) |

**Configuration:**
- **Mode:** Master
- **Speed:** Standard mode (100 kHz)
- **Addressing:** 7-bit
- **Pull-ups:** External 4.7kΩ on SCL and SDA

**I2C Device Addresses:**
- **0x48** - MAX9934 current sense amplifier
- **0x4A** - TCA4311A I2C bus buffer
- **0x50-0x57** - EEPROM / parameter storage (future)

### ADC Pins

| ADC | Channel | Pin | Signal | Range | Resolution |
|-----|---------|-----|--------|-------|------------|
| ADC1 | IN1 | PC0 | Temperature sensor 1 | 0-3.3V | 12-bit |
| ADC1 | IN2 | PC1 | Temperature sensor 2 | 0-3.3V | 12-bit |
| ADC1 | IN3 | PC2 | Solar panel voltage | 0-3.3V (scaled) | 12-bit |
| ADC1 | IN4 | PC3 | Load current | 0-3.3V (shunt voltage) | 12-bit |
| ADC1 | IN15 | PB0 | Battery voltage | 0-3.3V (scaled) | 12-bit |
| ADC1 | IN16 | PB1 | Solar panel current | 0-3.3V (shunt voltage) | 12-bit |

**Configuration:**
- **Resolution:** 12-bit (0-4095)
- **Sampling Time:** 47.5 cycles (for good accuracy with high impedance sources)
- **Conversion Mode:** Single conversion or scan mode
- **Trigger:** Software trigger
- **Reference:** VDDA = 3.3V

**Voltage Dividers:**
- **Battery voltage:** R1=10kΩ, R2=2.2kΩ → Max 15V → 3.3V
- **Solar voltage:** R1=10kΩ, R2=2.2kΩ → Max 15V → 3.3V

### PWM/Timer Pins

| Timer | Channel | Pin | AF | Signal | Frequency | Duty Cycle | Usage |
|-------|---------|-----|----|--------|-----------|------------|-------|
| TIM2 | CH1 | PC5 | AF1 | PWM_CH1 | 1 kHz | 0-100% | Heater control |
| TIM2 | CH4 | PC4 | AF1 | PWM_CH2 | 10 kHz | 0-100% | MPPT control |

**Configuration:**
- **Mode:** PWM mode 1 (active high)
- **Prescaler:** Calculate for desired frequency from 80 MHz
- **Auto-reload:** Calculate for desired frequency
- **Output:** Push-pull

---

## GPIO Configuration Details

### Output Pins

| Pin | Function | Type | Speed | Pull | Initial State |
|-----|----------|------|-------|------|---------------|
| PA0 | LED_STATUS | Push-pull | Low | None | Low (LED off) |
| PA1 | LED_FAULT | Push-pull | Low | None | Low (LED off) |
| PA2 | HEATER_CTRL | Push-pull | Low | None | Low (heater off) |
| PA3 | MPPT_EN | Push-pull | Low | None | Low (MPPT disabled) |
| PA4 | CS_BAT_MON | Push-pull | Very High | Pull-up | High (deselected) |
| PB2 | WDT_KICK | Push-pull | Medium | None | Low |
| PB3 | CS_ADC_EXT | Push-pull | Very High | Pull-up | High (deselected) |
| PB12 | CS_FLASH | Push-pull | Very High | Pull-up | High (deselected) |

**Note:** All chip select pins are active-low and should default to HIGH (deselected).

### Input Pins

| Pin | Function | Type | Pull | Usage |
|-----|----------|------|------|-------|
| PA10 | FAULT_IN | Digital input | Pull-up | External fault signal |
| PC13 | USER_BUTTON | Digital input | Pull-up | User button (for testing) |

### Alternate Function Pins

All AF pins (USART, SPI, I2C, TIM) should be configured as:
- **Mode:** Alternate Function
- **Type:** Push-pull (except I2C which needs open-drain)
- **Speed:** Very High (for USART, SPI) or High (for I2C)
- **Pull:** According to peripheral (USART: pull-up, SPI: no pull, I2C: external pull-up)

### Analog Pins

All ADC pins should be configured as:
- **Mode:** Analog
- **Pull:** None
- **Speed:** N/A (not applicable for analog)

---

## Special Considerations

### 1. I2C Pull-ups
STM32 I2C requires external pull-up resistors on SCL and SDA lines:
- **Value:** 4.7 kΩ typical (2.2 kΩ - 10 kΩ range)
- **Connected to:** 3.3V rail
- **Pins:** PB8 (SCL), PB9 (SDA)

### 2. SPI Chip Selects
All SPI chip select pins must be controlled manually via GPIO:
```c
// Example: Select device
GPIO->BSRR = GPIO_BSRR_BR4;  // Reset PA4 (chip select active low)

// SPI transaction here...

// Deselect device
GPIO->BSRR = GPIO_BSRR_BS4;  // Set PA4 (chip select inactive high)
```

### 3. ADC Voltage Scaling
ADC readings must be scaled according to voltage dividers:
```c
// Battery voltage (15V max → 3.3V)
float battery_voltage = (adc_reading * 3.3 / 4095.0) * (10.0 + 2.2) / 2.2;

// Solar voltage (15V max → 3.3V)
float solar_voltage = (adc_reading * 3.3 / 4095.0) * (10.0 + 2.2) / 2.2;
```

### 4. PWM Frequencies
- **Heater:** 1 kHz (to avoid EMI, allow thermal mass averaging)
- **MPPT:** 10 kHz (standard for DC-DC converters)

Calculate timer parameters:
```c
// For 1 kHz PWM from 80 MHz:
// Prescaler = 79 (divide by 80) → 1 MHz timer clock
// ARR = 999 (count 0-999) → 1 kHz PWM
TIM2->PSC = 79;
TIM2->ARR = 999;

// For 10 kHz PWM:
// Prescaler = 7 (divide by 8) → 10 MHz timer clock
// ARR = 999 (count 0-999) → 10 kHz PWM
TIM2->PSC = 7;
TIM2->ARR = 999;
```

### 5. Watchdog Kick Signal
External watchdog requires periodic toggling of PB2:
- **Frequency:** At least once per 1 second
- **Pattern:** Toggle high-low or use pulse
- **Configuration:** Output, push-pull, medium speed

---

## Pin Mapping Implementation

### GPIO Driver Pin Translation Function

The GPIO driver should translate abstract pin numbers to STM32 ports and pins:

```c
typedef struct {
    GPIO_TypeDef *port;
    uint16_t pin;
} stm32_pin_t;

static const stm32_pin_t pin_map[32] = {
    [GPIO_PIN_0]  = {GPIOA, 0},   // PA0 - LED_STATUS
    [GPIO_PIN_1]  = {GPIOA, 1},   // PA1 - LED_FAULT
    [GPIO_PIN_2]  = {GPIOA, 2},   // PA2 - HEATER_CTRL
    [GPIO_PIN_3]  = {GPIOA, 3},   // PA3 - MPPT_EN
    [GPIO_PIN_4]  = {GPIOA, 4},   // PA4 - CS_BAT_MON
    [GPIO_PIN_5]  = {GPIOA, 5},   // PA5 - SCK_SPI1
    [GPIO_PIN_6]  = {GPIOA, 6},   // PA6 - MISO_SPI1
    [GPIO_PIN_7]  = {GPIOA, 7},   // PA7 - MOSI_SPI1
    [GPIO_PIN_8]  = {GPIOB, 0},   // PB0 - ADC_VBAT
    [GPIO_PIN_9]  = {GPIOB, 1},   // PB1 - ADC_ISOLAR
    [GPIO_PIN_10] = {GPIOB, 2},   // PB2 - WDT_KICK
    [GPIO_PIN_11] = {GPIOB, 3},   // PB3 - CS_ADC_EXT
    [GPIO_PIN_12] = {GPIOB, 4},   // PB4 - I2C1_SDA_ALT
    [GPIO_PIN_13] = {GPIOB, 5},   // PB5 - I2C1_SCL_ALT
    [GPIO_PIN_14] = {GPIOB, 6},   // PB6 - UART1_TX
    [GPIO_PIN_15] = {GPIOB, 7},   // PB7 - UART1_RX
    [GPIO_PIN_16] = {GPIOB, 8},   // PB8 - I2C1_SCL
    [GPIO_PIN_17] = {GPIOB, 9},   // PB9 - I2C1_SDA
    [GPIO_PIN_18] = {GPIOB, 10},  // PB10 - UART3_TX
    [GPIO_PIN_19] = {GPIOB, 11},  // PB11 - UART3_RX
    [GPIO_PIN_20] = {GPIOB, 12},  // PB12 - CS_FLASH
    [GPIO_PIN_21] = {GPIOB, 13},  // PB13 - SCK_SPI2
    [GPIO_PIN_22] = {GPIOB, 14},  // PB14 - MISO_SPI2
    [GPIO_PIN_23] = {GPIOB, 15},  // PB15 - MOSI_SPI2
    [GPIO_PIN_24] = {GPIOC, 0},   // PC0 - ADC_TEMP1
    [GPIO_PIN_25] = {GPIOC, 1},   // PC1 - ADC_TEMP2
    [GPIO_PIN_26] = {GPIOC, 2},   // PC2 - ADC_VSOLAR
    [GPIO_PIN_27] = {GPIOC, 3},   // PC3 - ADC_ILOAD
    [GPIO_PIN_28] = {GPIOC, 4},   // PC4 - PWM_CH1
    [GPIO_PIN_29] = {GPIOC, 5},   // PC5 - PWM_CH2
    [GPIO_PIN_30] = {GPIOC, 6},   // PC6 - UART6_TX
    [GPIO_PIN_31] = {GPIOC, 7},   // PC7 - UART6_RX
};
```

---

## Hardware Validation Checklist

Before using this pin mapping in production, verify:

- [ ] Pin assignment matches PCB schematic
- [ ] Voltage levels are compatible (all 3.3V tolerant)
- [ ] Current drive capability is sufficient for loads
- [ ] Pull-up/pull-down resistors match design
- [ ] SPI/I2C timing meets device requirements
- [ ] ADC voltage dividers are correctly calculated
- [ ] PWM frequencies are appropriate for loads
- [ ] Watchdog kick signal is correctly routed
- [ ] All chip select signals are correctly inverted (active low)
- [ ] UART baud rates are achievable with 80 MHz clock

---

## References

- **STM32L476RG Datasheet:** [DS10198](https://www.st.com/resource/en/datasheet/stm32l476rg.pdf)
- **STM32L4 Reference Manual:** [RM0351](https://www.st.com/resource/en/reference_manual/rm0351-stm32l47xxx-stm32l48xxx-stm32l49xxx-and-stm32l4axxx-advanced-armbased-32bit-mcus-stmicroelectronics.pdf)
- **MSP430F6659 Datasheet:** [SLAS704](https://www.ti.com/lit/ds/symlink/msp430f6659.pdf)
- **EPS2 Hardware Schematic:** v2.1 (2021-05-15)

---

**Last Updated:** 2025-11-18
**Author:** Claude (EPS2 Port)
**Status:** Draft - Requires hardware team verification before use

**IMPORTANT:** This pin mapping is a **draft** and must be verified against the actual PCB hardware before flashing to the target. Incorrect pin assignments can damage hardware!
