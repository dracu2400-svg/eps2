# EPS2 Hardware Components and Architecture

## Overview

The EPS2 (Electrical Power System 2.0) is a complete power management solution for the FloripaSat-2 CubeSat mission. The hardware consists of a 4-layer PCB integrating solar panel management, battery charging, DC-DC conversion, thermal control, and system monitoring.

## PCB Specifications

- **Dimensions**: 90mm x 93mm x 1.6mm
- **Form Factor**: Modified PC/104 for 2U CubeSat
- **Layers**: 4-layer FR-4
- **Copper Thickness**: TBD (standard 1 oz or 2 oz)
- **Finish**: ENIG (Electroless Nickel Immersion Gold) or HASL
- **Design Tool**: KiCad or Altium Designer
- **Manufacturer**: TBD (suitable for space-grade or high-reliability PCBs)

## System Architecture Block Diagram

```
┌─────────────────────────────────────────────────────────────────┐
│                         EPS2 System                              │
├──────────────────┬──────────────────────┬──────────────────────┤
│  Solar Panels    │                      │    Power Bus         │
│  (10 panels)     │                      │   Distribution       │
│                  │                      │                      │
│  ┌────┬────┬────┐│    ┌──────────┐     │  ┌────────────────┐  │
│  │-Y │+Y │-X  ││    │          │     │  │ DC-DC Buck     │  │
│  │ │  ││+X │-Z  ││───→│   MPPT   │     │  │ Converters     │  │
│  │ │  ││+Z │    ││    │  Control │     │  │ (TPS54x0)      │  │
│  └────┴────┴────┘│    │  (PWM)   │     │  └────┬───────────┘  │
│                  │    └──────────┘     │       │              │
│  Current/Voltage │                      │       ├→ 3.3V Bus   │
│  Sensors         │                      │       ├→ 5.0V Bus   │
│  (MAX9934, ADC)  │                      │       └→ Payload    │
└──────────────────┴──────────────────────┴──────────────────────┘
         │                                            │
         v                                            v
┌──────────────────┐                        ┌─────────────────┐
│  Battery Module  │                        │   Satellite     │
│  (4x Li-ion)     │                        │   Subsystems    │
│                  │                        │                 │
│  ┌────────────┐  │                        │  - OBDH         │
│  │  DS2777G   │  │                        │  - TTC          │
│  │  Battery   │  │                        │  - Payload      │
│  │  Monitor   │  │                        │  - ADCS         │
│  └────────────┘  │                        └─────────────────┘
│                  │
│  ┌────────────┐  │
│  │  Heaters   │  │
│  │  (2 ch)    │  │
│  │  PWM ctrl  │  │
│  └────────────┘  │
│                  │
│  RTD Temp        │
│  Sensors (4)     │
└──────────────────┘
         │
         v
┌──────────────────────────────────────────────────────────────┐
│                  MSP430F6659 Microcontroller                  │
│  ┌──────────┬──────────┬──────────┬──────────┬─────────────┐ │
│  │ FreeRTOS │   MPPT   │  Heater  │  Sensor  │ Comm I2C/   │ │
│  │ Kernel   │Algorithm │    PID   │  Reading │ UART/SPI    │ │
│  └──────────┴──────────┴──────────┴──────────┴─────────────┘ │
└──────────────────────────────────────────────────────────────┘
```

---

## Major Hardware Components

### 1. Microcontroller

**Part Number**: MSP430F6659IPZR

**Manufacturer**: Texas Instruments

**Package**: LQFP-100

**Key Specifications**:
- **Architecture**: 16-bit RISC
- **Flash Memory**: 512 KB
- **SRAM**: 64 KB
- **Clock Speed**: Up to 25 MHz (configured for 32 MHz with limitations)
- **Operating Voltage**: 1.8V - 3.6V (3.3V in EPS2)
- **Operating Temperature**: -40°C to +85°C (industrial)
- **Power Consumption**:
  - Active mode: ~200 µA/MHz
  - Low-power modes available (LPM0-LPM4)

**Peripherals**:
- 6x USCI modules (I2C/SPI/UART)
- 12-bit ADC (12 channels)
- 2x 12-bit DAC
- Multiple timers (Timer_A, Timer_B)
- 74 GPIO pins
- DMA controller
- Real-time clock (RTC)

**Rationale**: Low power consumption, rich peripheral set, extensive I/O, flight heritage in space applications.

---

### 2. Voltage Reference

**Part Number**: REF5025AQDRQ1

**Manufacturer**: Texas Instruments

**Package**: VSSOP-8

**Specifications**:
- **Output Voltage**: 2.5V (configurable to 3.0V via resistor network)
- **Accuracy**: ±0.05% (max)
- **Temperature Coefficient**: 3 ppm/°C (typical)
- **Automotive Grade**: AEC-Q100 qualified
- **Low Noise**: 4 µVpp (0.1 Hz to 10 Hz)

**Function**: Precision voltage reference for ADC measurements (voltage and current sensors).

---

### 3. Solar Panel MPPT Components

#### Power Stage

**DC-DC Converters**: TBD (buck or boost converters based on panel voltage and battery voltage)

**Switching Frequency**: 10-100 kHz

**Efficiency**: > 90% typical

#### MPPT Control

**Method**: PWM control via MSP430 Timer B0

**Channels**: 3 independent MPPT channels

**Algorithm**: Perturb and Observe (P&O) implemented in firmware

**Power Switching Devices**:
- MOSFETs: TBD (N-channel or P-channel depending on topology)
- Gate drivers: TBD

#### Solar Panel Connectors

**Type**: TBD (header, terminal block, or specialized space connector)

**Pinout**: Each connector provides:
- V+ (panel positive)
- V- (panel negative)
- Optional current sense lines

---

### 4. Battery Interface

#### Battery Monitor IC

**Part Number**: DS2777G

**Manufacturer**: Maxim Integrated (now part of Analog Devices)

**Package**: 8-pin µSOP

**Function**: Battery fuel gauge and protection

**Features**:
- Voltage measurement: ±10 mV accuracy
- Current measurement: ±3% accuracy
- Accumulated charge tracking (RAAC, RSAC)
- Remaining capacity estimation (RARC, RSRC)
- Temperature measurement
- I2C interface
- Overvoltage/undervoltage protection

**I2C Address**: 0x34 (typical for DS2777G family)

**Connection**: I2C bus 0 (USCI_B0) in master mode

#### Battery Configuration

**Battery Type**: Lithium-ion (Li-ion)

**Configuration**: 4 cells in series/parallel (2S2P or similar)

**Nominal Voltage**: ~7.4V (for 2S configuration)

**Maximum Capacity**: 2450 mAh

**Connector**: TBD (JST, Molex, or custom)

**Protection**:
- Overcurrent protection (hardware)
- Overvoltage protection (DS2777G + firmware)
- Undervoltage protection (DS2777G + firmware)
- Temperature monitoring and cutoff

---

### 5. Current Sensors

#### Current Sense Amplifier

**Part Number**: MAX9934

**Manufacturer**: Maxim Integrated

**Package**: SOT23-5

**Specifications**:
- **Gain**: 25 µA/mV (fixed)
- **Bandwidth**: 1 MHz
- **Input Offset**: < 50 µV
- **Supply Voltage**: 2.7V - 5.5V
- **Operating Temperature**: -40°C to +125°C

**Function**: Amplifies voltage drop across current sense resistors for ADC measurement.

**Quantity**: 7 amplifiers (6 for solar panels, 1 for EPS current)

#### Current Sense Resistors

**Solar Panel Sensing**:
- **Value**: 20 mΩ (0.020 Ω)
- **Power Rating**: 1W or higher
- **Tolerance**: 1%
- **Type**: Low-inductance metal foil or current sense resistor

**EPS Current Sensing**:
- **Value**: 75 mΩ (0.075 Ω)
- **Power Rating**: 1W or higher
- **Tolerance**: 1%

**Placement**: High-side or low-side depending on circuit topology

**Current Calculation**:
```
I (mA) = V_sense / R_sense
V_sense = V_ADC / Gain
```

---

### 6. Voltage Sensors

#### Resistor Divider Networks

**Function**: Scale down high voltages (solar panels, battery) to ADC input range (0-3V).

**Solar Panel Dividers**:
- **Ratio**: 4.0x (e.g., R1=30kΩ, R2=10kΩ)
- **Input Range**: 0-12V → 0-3V at ADC
- **Tolerance**: 1% or better for accuracy

**Main Bus Divider**:
- **Ratio**: 1.93x
- **Input Range**: 0-6V → 0-3.1V at ADC

**Component Selection**:
- Precision resistors (1% tolerance, low temperature coefficient)
- Power rating: 1/4W typical

---

### 7. Temperature Sensors

#### RTD Sensors

**Type**: Platinum RTD (PT100 or PT1000)

**Quantity**: 7 sensors
- 4 on battery module
- 3 on solar panels

**Connection**: 4-wire connection for high accuracy

**Temperature Range**: -40°C to +85°C (typical for space)

**Interface**: Via ADS1248 ADC

#### Temperature ADC

**Part Number**: ADS1248

**Manufacturer**: Texas Instruments

**Package**: TSSOP-24

**Specifications**:
- **Resolution**: 24-bit
- **Channels**: 8 differential inputs (MUX-based)
- **Interface**: SPI
- **Sample Rate**: Up to 2 kSPS
- **PGA**: Programmable gain amplifier (1 to 128)
- **Reference**: External 3.3V or internal

**Function**: High-precision ADC for RTD temperature measurement.

**SPI Configuration**:
- Mode: 1 (CPOL=0, CPHA=1)
- Clock speed: 100 kHz
- Chip select: Active LOW

#### MCU Internal Temperature Sensor

**Location**: Inside MSP430F6659

**Interface**: Internal ADC channel

**Accuracy**: ±2°C typical

**Function**: Monitor MCU die temperature for thermal management.

---

### 8. Battery Heaters

#### Heating Elements

**Type**: Resistive heater elements (thin-film or wire-wound)

**Quantity**: 2 heaters

**Power Rating**: TBD (typically 1-5W each)

**Location**: Attached to battery module for thermal control

**Target**: Maintain battery temperature above 0°C during charging

#### Heater Drivers

**Switching Devices**: N-channel MOSFETs (TBD)

**Control**: PWM from MSP430 Timer A1

**Frequency**: 1-10 Hz (low frequency to reduce EMI)

**Duty Cycle**: 0-100% (controlled by PID algorithm)

**Protection**:
- Thermal fuse (optional)
- Over-temperature cutoff in firmware
- Current limiting

---

### 9. DC-DC Buck Converters

#### Main Bus Converters

**Part Number**: TPS54x0 series (e.g., TPS54360, TPS54202)

**Manufacturer**: Texas Instruments

**Function**: Convert battery voltage to regulated output voltages

**Outputs**:
- **3.3V Bus**: Main system power (OBDH, TTC, EPS MCU)
  - Current capability: 2-3A
- **5.0V Bus**: Payloads or specific subsystems (if required)
  - Current capability: 1-2A
- **Payload Bus**: Switchable output for payload
  - Enable/disable via GPIO_PIN_3

**Specifications** (typical for TPS54360):
- **Input Voltage**: 4.5V - 60V (covers battery voltage range)
- **Output Voltage**: Adjustable via resistor divider
- **Switching Frequency**: 100 kHz - 2.5 MHz (configurable)
- **Efficiency**: > 90%
- **Current Limit**: Internal current limiting
- **Thermal Shutdown**: Built-in protection

**External Components**:
- Input/output capacitors (ceramic and electrolytic)
- Inductor (10-47 µH depending on design)
- Feedback resistors (set output voltage)
- Bootstrap capacitor

---

### 10. I2C Buffer and Protection

#### I2C Buffer IC

**Part Number**: TCA4311A

**Manufacturer**: Texas Instruments

**Package**: VSSOP-8

**Function**: I2C bus buffer with hot-swap capability

**Features**:
- Live insertion (hot-swap) on I2C bus
- Rise-time acceleration
- Bus-fault detection and isolation
- Compatible with 1.8V - 5V systems
- Enable pin control (GPIO_PIN_66)
- Ready output (GPIO_PIN_69)

**Location**: Between MCU I2C2 (USCI_B2) and OBDH module

**Rationale**: Protects EPS2 I2C bus from faults on OBDH bus, allows safe connection/disconnection.

---

### 11. Oscillators and Clocks

#### Low-Frequency Crystal

**Part Number**: TBD (standard 32.768 kHz watch crystal)

**Frequency**: 32.768 kHz

**Accuracy**: ±20 ppm typical

**Load Capacitance**: 12.5 pF typical

**Function**: Provides ACLK for RTC and low-power timers

**Connections**: PJ.4 (LFXIN), PJ.5 (LFXOUT)

#### Main Clock

**Source**: Internal DCO (Digitally Controlled Oscillator)

**Frequency**: 32 MHz (configured)

**Function**: Main system clock (MCLK, SMCLK)

**Accuracy**: Calibrated using DCO calibration constants

---

### 12. Power Supply Filtering

#### Input Capacitors

**Location**: Battery input, solar panel inputs

**Type**: Ceramic (X7R or X5R) + electrolytic (low ESR aluminum or tantalum)

**Values**: TBD (e.g., 10 µF ceramic + 100 µF electrolytic)

**Function**: Filter input voltage, provide decoupling

#### Output Capacitors

**Location**: DC-DC converter outputs (3.3V, 5V buses)

**Type**: Ceramic (X7R) + electrolytic

**Values**: TBD (e.g., 22 µF ceramic + 100 µF electrolytic)

**Function**: Smooth output voltage, reduce ripple

#### Decoupling Capacitors

**Location**: Near every IC (MCU, ADCs, amplifiers, etc.)

**Type**: Ceramic (X7R or X5R)

**Values**: 100 nF typical, plus 10 µF for high-current ICs

**Function**: Local high-frequency noise filtering

---

### 13. Protection Components

#### TVS Diodes

**Function**: Transient voltage suppression, ESD protection

**Locations**:
- External connectors (solar panels, battery, I2C, UART)
- Critical signal lines

**Type**: Unidirectional or bidirectional depending on signal

#### Fuses

**Location**: Battery input (optional), critical power lines

**Type**: Resettable PTC (Positive Temperature Coefficient) or traditional fuses

**Rating**: Based on expected maximum current

#### Schottky Diodes

**Function**: Reverse polarity protection, OR-ing for redundant supplies

**Locations**: Power inputs

---

### 14. Connectors

#### PC/104 Connector

**Type**: PC/104 stackable connector (modified for 2U CubeSat: 90x93mm)

**Pins**: TBD (standard PC/104 has 104 pins, but may be reduced for CubeSat)

**Function**: Mechanical and electrical interface to other satellite modules

**Connections**:
- Power distribution (3.3V, 5V, battery)
- I2C bus (OBDH)
- UART (TTC)
- Control signals

#### Solar Panel Connectors

**Quantity**: 6-10 connectors (depending on panel grouping)

**Type**: TBD (header, JST, Molex, or space-rated connector)

**Pins per connector**: 2-4 (V+, V-, optional sense lines)

#### Battery Connector

**Type**: TBD (JST, Molex, or locking connector)

**Pins**: 6-8
- VBAT+ / VBAT- (power)
- I2C SDA / SCL (DS2777G)
- RTD connections (4x for 4-wire RTD)

#### Debug/Programming Connectors

**JTAG Connector**:
- Type: 4-pin or 6-pin header
- Pins: VCC, TEST, RST, GND (+ optional)

**Debug UART Connector**:
- Type: 4-pin header
- Pins: TX, RX, GND, VCC (optional)

---

## Power Budget

### Power Consumption

#### MCU (MSP430F6659)
- Active mode @ 32 MHz: ~20 mA
- Low-power mode: < 1 mA

#### Peripherals
- DS2777G (battery monitor): ~50 µA
- ADS1248 (temp ADC): ~1 mA
- MAX9934 amplifiers (7x): ~0.5 mA each
- TCA4311A (I2C buffer): ~10 µA

#### DC-DC Converters
- Quiescent current: ~100 µA per converter

#### LEDs
- System LED: ~2 mA (when ON)
- Fault LED: ~2 mA (when ON)

#### Total EPS Power Consumption
- Normal operation: ~30-50 mA @ 3.3V ≈ 100-165 mW
- Peak (with heaters ON): Up to several watts

### Power Generation

#### Solar Panels
- Total area: ~0.02 m² (for 2U CubeSat)
- Efficiency: ~28% (triple-junction GaAs)
- Irradiance: 1367 W/m² (LEO, worst case)
- Power generation: ~5-10W (depending on orientation)

#### Battery
- Capacity: 2450 mAh @ 7.4V ≈ 18 Wh
- Charge/discharge rate: 1C typical

---

## Thermal Design

### Heat Sources
- DC-DC converters (TPS54x0): ~0.5-1W loss each
- Battery during charging: ~1-2W
- MCU: ~0.1W
- Heaters: Up to 5-10W (when active)

### Thermal Management
- **Passive Cooling**: Conduction to CubeSat structure
- **Active Heating**: Battery heaters controlled by PID algorithm
- **Temperature Monitoring**: 8 temperature sensors (RTD + MCU)

### Operating Temperature Range
- **Specification**: -40°C to +85°C (standard for space-grade components)
- **Battery Target**: 0°C to +45°C (optimal for Li-ion charging)

---

## Mechanical Design

### PCB Mounting
- **Mounting Holes**: 4 corners, M3 standoffs
- **Spacing**: PC/104 standard (modified to 90x93mm)
- **Stackup**: 4-layer PCB within CubeSat rails

### Component Placement
- **Top Side**: Main components (MCU, DC-DC converters, sensors)
- **Bottom Side**: Connectors, passives, less critical components

### Vibration and Shock Resistance
- **Launch Loads**: Designed to withstand launch vibration per CubeSat standards
- **Component Attachment**: SMD components with appropriate pad sizes, through-hole for connectors

---

## EMI/EMC Considerations

### Switching Noise
- DC-DC converters: Switching frequency selected to avoid interference with communication bands
- Ferrite beads on power lines
- Ground plane continuity

### Radiated Emissions
- Shielding: Metal CubeSat structure acts as Faraday cage
- Filtered connectors for external interfaces

### Conducted Emissions
- Input/output filtering capacitors
- LC filters on power lines

---

## Reliability and Redundancy

### Single Point Failures
- **Critical Components**: MCU, DC-DC converters, battery monitor
- **Mitigation**: Component selection (space-grade or automotive-grade), derating, periodic system reset

### Watchdog Timer
- Hardware watchdog ensures system recovery from software hangs

### Redundancy
- **Dual Power Paths**: Solar panels grouped for redundancy
- **Multiple MPPT Channels**: Independent control reduces impact of single channel failure

---

## Design Files and Documentation

### Hardware Design Files
- **Schematics**: `/home/user/eps2/hardware/outputs/board_schematics/`
- **PCB Layout**: KiCad or Altium project files
- **Gerber Files**: `/home/user/eps2/hardware/outputs/board_gerber/`
- **3D Models**: `/home/user/eps2/hardware/outputs/board_3dmodels/`

### Bill of Materials
- **BOM**: `/home/user/eps2/hardware/outputs/board_bom/board_bom.xlsx`

### Assembly Drawings
- Component placement diagrams
- Assembly instructions

### Test and Verification
- Test procedures
- Acceptance test plans
- Qualification test results

---

## Manufacturing Considerations

### PCB Fabrication
- **Impedance Control**: For high-speed signals (if required)
- **Via Types**: Through-hole, blind, buried (if needed for 4-layer design)
- **Soldermask**: LPI (Liquid Photo-Imageable) soldermask
- **Silkscreen**: Component designators, polarity marks

### Assembly
- **SMT Assembly**: Reflow soldering for SMD components
- **Through-Hole**: Hand soldering or wave soldering for connectors
- **Inspection**: AOI (Automated Optical Inspection), X-ray for BGA/QFN (if used)

### Testing
- **Functional Test**: Power-on, firmware loading, basic functionality
- **Environmental Test**: Thermal cycling, vibration, thermal vacuum (TVAC)
- **Burn-in**: Extended operation test to screen for infant mortality

---

## Summary

The EPS2 hardware combines a powerful MSP430 microcontroller with precision analog components, efficient power conversion, and robust thermal management to provide a complete power solution for CubeSat missions. Key features include:

- **3-channel MPPT** for optimized solar energy harvesting
- **High-precision sensing** with 24-bit ADC and low-noise amplifiers
- **Intelligent battery management** with DS2777G fuel gauge
- **Dual heater control** using PID algorithm
- **Robust communication** via I2C, UART, and SPI with protection
- **Modular design** for easy integration into 2U CubeSat

The system is designed for reliability, efficiency, and long-term operation in the challenging space environment.
