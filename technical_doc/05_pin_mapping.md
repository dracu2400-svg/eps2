# EPS2 Pin Mapping

## Overview

The MSP430F6659IPZR microcontroller provides 74 GPIO pins organized in multiple ports. This document details the pin assignments for all peripherals, sensors, communication interfaces, and control signals on the EPS2.

## MCU Package

- **Device**: MSP430F6659IPZR
- **Package**: LQFP-100 (Low-profile Quad Flat Package, 100 pins)
- **Total GPIO**: 74 pins
- **Port Organization**: P1-P11, PJ

---

## Pin Mapping by Function

### Power Supply Pins

| Pin Name | Function | Description |
|----------|----------|-------------|
| DVCC     | Digital Power | 3.3V digital supply |
| DVSS     | Digital Ground | Digital ground |
| AVCC     | Analog Power | 3.3V analog supply |
| AVSS     | Analog Ground | Analog ground |

---

### Clock Pins

| Pin | Port.Pin | Function | Description |
|-----|----------|----------|-------------|
| 74  | PJ.4     | LFXIN | 32.768 kHz crystal input |
| 75  | PJ.5     | LFXOUT | 32.768 kHz crystal output |

**Crystal**: 32.768 kHz for ACLK (RTC and low-power timers)

---

### JTAG/Programming Interface

| Pin | Port.Pin | Function | Description |
|-----|----------|----------|-------------|
| 86  | TEST     | TEST | JTAG test mode |
| 87  | RST/NMI  | RST | Reset / Non-maskable interrupt |

**Programming Protocol**: Spy-Bi-Wire (2-wire JTAG)

---

## Communication Interfaces

### I2C Bus 0 (USCI_B0) - Master Mode

**Function**: Battery Monitor Communication

| Pin | Port.Pin | Signal | Direction | Connected To |
|-----|----------|--------|-----------|--------------|
| 50  | P3.0     | UCB0SDA | Bidirect. | DS2777G SDA |
| 51  | P3.1     | UCB0SCL | Output | DS2777G SCL |

**Pull-ups**: External 4.7kΩ on SDA and SCL

---

### I2C Bus 2 (USCI_B2) - Slave Mode

**Function**: OBDH Communication

| Pin | Port.Pin | Signal | Direction | Connected To |
|-----|----------|--------|-----------|--------------|
| 66  | P9.1     | UCB2SDA | Bidirect. | OBDH via TCA4311A |
| 67  | P9.2     | UCB2SCL | Input | OBDH via TCA4311A |

**I2C Buffer IC**: TCA4311A (hot-swap I2C buffer)

**Control Pins**:

| Pin | GPIO | Function | Description |
|-----|------|----------|-------------|
| TBD | GPIO_PIN_66 | I2C_BUF_EN | Enable TCA4311A buffer |
| TBD | GPIO_PIN_69 | I2C_BUF_RDY | Ready signal from TCA4311A |

---

### UART 0 (USCI_A0) - TTC Communication

**Function**: Beacon Radio Communication (9600 baud)

| Pin | Port.Pin | Signal | Direction | Connected To |
|-----|----------|--------|-----------|--------------|
| 52  | P3.3     | UCA0TXD | Output | TTC Module RX |
| 53  | P3.4     | UCA0RXD | Input | TTC Module TX |

---

### UART 1 (USCI_A2) - Debug Console

**Function**: Debug Serial Console (115200 baud)

| Pin | Port.Pin | Signal | Direction | Connected To |
|-----|----------|--------|-----------|--------------|
| 62  | P9.4     | UCA2TXD | Output | Debug connector |
| 63  | P9.5     | UCA2RXD | Input | Debug connector |

---

### SPI (USCI_A1) - Temperature ADC

**Function**: ADS1248 24-bit ADC for RTD sensors

| Pin | Port.Pin | Signal | Direction | Connected To |
|-----|----------|--------|-----------|--------------|
| 54  | P3.5     | UCA1CLK | Output | ADS1248 SCLK |
| 55  | P3.6     | UCA1SIMO | Output | ADS1248 DIN |
| 56  | P3.7     | UCA1SOMI | Input | ADS1248 DOUT |

**Chip Select and Control Pins**:

| Pin | GPIO | Function | Description |
|-----|------|----------|-------------|
| TBD | SPI_CS_0 | SPI_CS | ADS1248 chip select (active LOW) |
| TBD | GPIO_PIN_60 | ADS_START | Start conversion (pulse HIGH) |
| TBD | GPIO_PIN_58 | ADS_RESET | Hardware reset (active LOW) |

---

## Analog Inputs (ADC12_A)

### Voltage Sensing Channels

| ADC Ch | Pin | Port.Pin | Function | Measurement | Divider |
|--------|-----|----------|----------|-------------|---------|
| A0     | 2   | P6.0     | Solar -Y Voltage | Solar panel group | 4.0x |
| A1     | 3   | P6.1     | Solar +Y Voltage | Solar panel group | 4.0x |
| A2     | 4   | P6.2     | Solar -X Voltage | Solar panel group | 4.0x |
| A3     | 5   | P6.3     | Solar Total Voltage | All panels | 4.0x |
| A4     | 6   | P6.4     | Main Bus Voltage | Power bus | 1.93x |

**Reference Voltage**: 3.0V (REF5025AQDRQ1)

---

### Current Sensing Channels

**Current Sense Amplifier**: MAX9934 (gain: 25 µA/mV)

| ADC Ch | Pin | Port.Pin | Function | Measurement | Rsense |
|--------|-----|----------|----------|-------------|--------|
| A5     | 7   | P6.5     | Solar -Y Current | Solar panel | 20mΩ |
| A6     | 8   | P6.6     | Solar +Y Current | Solar panel | 20mΩ |
| A7     | 9   | P6.7     | Solar -X Current | Solar panel | 20mΩ |
| A8     | 10  | P7.0     | Solar +X Current | Solar panel | 20mΩ |
| A9     | 11  | P7.1     | Solar -Z Current | Solar panel | 20mΩ |
| A10    | 12  | P7.2     | Solar +Z Current | Solar panel | 20mΩ |
| A11    | 13  | P7.3     | EPS + Beacon Current | System current | 75mΩ |

---

### Internal Temperature Sensor

| ADC Ch | Function | Description |
|--------|----------|-------------|
| A10 (Internal) | MCU Temperature | Internal die temperature sensor |

---

## PWM Outputs

### MPPT PWM (Timer B0)

**Function**: Solar panel MPPT control

| Pin | Port.Pin | Timer | Function | Controls |
|-----|----------|-------|----------|----------|
| TBD | PWM_PORT_1 | TB0.1 | MPPT Ch 0 | -Y and +X panels |
| TBD | PWM_PORT_2 | TB0.2 | MPPT Ch 1 | -X and +Z panels |
| TBD | PWM_PORT_3 | TB0.3 | MPPT Ch 2 | -Z and +Y panels |

**PWM Frequency**: Configurable (typically 10-100 kHz)
**Duty Cycle Range**: 0-100%

---

### Heater PWM (Timer A1)

**Function**: Battery heater control

| Pin | Port.Pin | Timer | Function | Sensor |
|-----|----------|-------|----------|--------|
| TBD | PWM_PORT_1 | TA1.1 | Heater 0 | RTD Ch 6 |
| TBD | PWM_PORT_2 | TA1.2 | Heater 1 | RTD Ch 2 |

**PWM Frequency**: Configurable (typically 1-10 Hz)
**Duty Cycle Range**: 0-100%

---

## GPIO Pins

### LED Indicators

| Pin | GPIO | Function | Active | Description |
|-----|------|----------|--------|-------------|
| TBD | GPIO_PIN_36 | System LED | HIGH | Heartbeat indicator |
| TBD | GPIO_PIN_35 | Fault LED | HIGH | Error/fault indicator |

---

### Power Control

| Pin | GPIO | Function | Active | Description |
|-----|------|----------|--------|-------------|
| TBD | GPIO_PIN_3 | Payload Enable | HIGH | Payload power control |

---

### Hardware Version Detection

**Function**: Resistor divider networks for PCB version identification

| Pin | Port.Pin | ADC Ch | Function |
|-----|----------|--------|----------|
| TBD | Px.x     | Ax     | HW_VER_0 |
| TBD | Px.x     | Ax     | HW_VER_1 |

**Method**: ADC reads voltage levels set by resistor dividers
**Read**: At startup by Startup Task

---

### Kill Switches (Optional)

**Function**: Ground test and launch safety

| Pin | GPIO | Function | Description |
|-----|------|----------|-------------|
| TBD | GPIO_KILL_0 | Kill Switch 0 | Disable specific functions |
| TBD | GPIO_KILL_1 | Kill Switch 1 | Safety interlock |

---

## RTD Temperature Sensor Connections

**Interface**: Via ADS1248 24-bit ADC (SPI)

**Channels**:

| RTD Ch | Function | Location | Connector |
|--------|----------|----------|-----------|
| 0      | Battery Temp 1 | Battery module | RTD connector |
| 1      | Battery Temp 2 | Battery module | RTD connector |
| 2      | Battery Temp 3 | Battery module | RTD connector |
| 3      | Battery Temp 4 | Battery module | RTD connector |
| 4      | Solar Panel -Y Temp | Solar panel | RTD connector |
| 5      | Solar Panel +Y Temp | Solar panel | RTD connector |
| 6      | Solar Panel -Z Temp | Solar panel | RTD connector |

**RTD Type**: Platinum RTD (PT100 or PT1000)
**Connection**: 4-wire connection for accuracy

---

## PC/104 Connector Pinout

### Power Pins

| Pin | Function | Description |
|-----|----------|-------------|
| 1   | +5V (optional) | 5V power (if used) |
| 2   | +3.3V | 3.3V main power bus |
| 3-4 | GND | Ground |

### Communication Pins

| Pin | Function | Description |
|-----|----------|-------------|
| TBD | I2C_SDA | OBDH I2C data |
| TBD | I2C_SCL | OBDH I2C clock |
| TBD | UART_TX | TTC UART transmit |
| TBD | UART_RX | TTC UART receive |

### Control/Status Pins

| Pin | Function | Description |
|-----|----------|-------------|
| TBD | PAYLOAD_EN | Payload power enable |
| TBD | SYS_RESET | System reset signal |

---

## Solar Panel Connectors

### Panel Voltage Inputs

Each solar panel group connects via dedicated connectors:

| Connector | Panels | Voltage Pin | Current Pin |
|-----------|--------|-------------|-------------|
| SP_-Y     | -Y face | V_-Y | I_-Y |
| SP_+Y     | +Y face | V_+Y | I_+Y |
| SP_-X     | -X face | V_-X | I_-X |
| SP_+X     | +X face | V_+X | I_+X |
| SP_-Z     | -Z face | V_-Z | I_-Z |
| SP_+Z     | +Z face | V_+Z | I_+Z |

---

## Battery Connector

| Pin | Function | Description |
|-----|----------|-------------|
| 1   | VBAT+ | Battery positive |
| 2   | VBAT- | Battery negative |
| 3   | I2C_SDA | DS2777G data |
| 4   | I2C_SCL | DS2777G clock |
| 5-8 | RTD_x | RTD temperature sensors |

---

## Debug Connector

| Pin | Function | Description |
|-----|----------|-------------|
| 1   | UART_TX | Debug console transmit |
| 2   | UART_RX | Debug console receive |
| 3   | GND | Ground |
| 4   | VCC (optional) | 3.3V (for level shifters) |

---

## JTAG/Programming Connector

| Pin | Function | Description |
|-----|----------|-------------|
| 1   | VCC | 3.3V target power |
| 2   | TEST | Spy-Bi-Wire TEST |
| 3   | RST | Reset |
| 4   | GND | Ground |

**Compatible Programmers**:
- MSP-FET
- MSP-FET430UIF (legacy)

---

## Pin Configuration Summary

### Port 1 (P1.0 - P1.7)
- General purpose I/O
- Optional ADC inputs (if available)

### Port 2 (P2.0 - P2.7)
- General purpose I/O
- Timer outputs (if configured)

### Port 3 (P3.0 - P3.7)
- I2C0 (P3.0-P3.1): Battery Monitor
- UART0 (P3.3-P3.4): TTC Communication
- SPI (P3.5-P3.7): ADS1248 ADC

### Port 6 (P6.0 - P6.7)
- ADC inputs: Solar voltages and currents

### Port 7 (P7.0 - P7.3)
- ADC inputs: Solar currents and system current

### Port 9 (P9.1 - P9.5)
- I2C2 (P9.1-P9.2): OBDH Communication
- UART1 (P9.4-P9.5): Debug Console

### Port J (PJ.0 - PJ.5)
- PJ.4-PJ.5: Crystal oscillator (32.768 kHz)

---

## Electrical Specifications

### GPIO
- **Logic High**: > 2.0V (VCC = 3.3V)
- **Logic Low**: < 0.8V
- **Max Current**: 6 mA per pin (check datasheet for exact limits)

### ADC
- **Resolution**: 12-bit (4096 levels)
- **Reference**: 3.0V (external REF5025)
- **Input Range**: 0 - VREF (0 - 3.0V)

### PWM
- **Voltage**: 3.3V logic level
- **Drive**: May require external drivers for high-power loads

### I2C
- **Voltage**: 3.3V
- **Pull-ups**: 4.7kΩ typical
- **Max Speed**: 400 kHz (fast mode), configured for 100 kHz

### UART
- **Voltage**: 3.3V logic level
- **Level Shifting**: May be required for RS-232 compatibility

### SPI
- **Voltage**: 3.3V
- **Max Speed**: Limited by ADS1248 (several MHz possible)

---

## Notes

1. **Pin Numbers**: Some pin numbers are marked as "TBD" as they require reference to the actual PCB schematic for exact pin assignments. Refer to hardware schematics in `/home/user/eps2/hardware/outputs/board_schematics/`.

2. **GPIO_PIN_x Macros**: The firmware uses GPIO_PIN_x macros defined in driver headers. These map to specific port.pin combinations on the MSP430.

3. **Pull-ups/Pull-downs**: Check hardware schematics for external pull-up or pull-down resistors on specific pins.

4. **Multiplexed Functions**: Many MSP430 pins have multiple functions (GPIO, timer, ADC, etc.). The pin function is determined by register configuration in the firmware.

5. **Unused Pins**: Any unused pins should be configured as GPIO outputs driven LOW or inputs with pull-ups/pull-downs to prevent floating inputs.

6. **ESD Protection**: All external connector pins should have ESD protection (TVS diodes, etc.).

---

## Reference Documents

For complete and accurate pin mapping, refer to:

1. **MSP430F6659 Datasheet**: Texas Instruments document for pin definitions
2. **EPS2 Hardware Schematics**: `/home/user/eps2/hardware/outputs/board_schematics/`
3. **Firmware Driver Definitions**: `/home/user/eps2/firmware/drivers/` for GPIO macros
4. **Board Layout Files**: `/home/user/eps2/hardware/` for physical pin connections
