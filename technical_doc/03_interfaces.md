# EPS2 Communication Interfaces

## Overview

The EPS2 provides multiple communication interfaces for inter-module communication, debugging, and sensor interfacing. The system implements 2 I2C buses, 2 UART interfaces, 1 SPI interface, and JTAG programming/debugging.

## I2C Interfaces

### I2C Bus 0 (USCI_B0) - Master Mode

**Purpose**: Communication with battery monitor IC

**Configuration**:
- Mode: Master
- Speed: 100 kbps
- Addressing: 7-bit
- USCI Module: USCI_B0

**Connected Devices**:
- **DS2777G Battery Monitor IC**
  - Function: Battery voltage, current, charge, and temperature monitoring
  - I2C Address: 0x34 (factory default for DS2777G)
  - Registers accessed: Voltage, Current, Temperature, Status, RAAC, RSAC, RARC, RSRC

**Data Transfer Pattern**:
```
Master (MCU) → Read Request → DS2777G
DS2777G → Register Data → Master (MCU)
```

**Register Read Example**:
1. MCU sends START condition
2. MCU sends device address (0x34) + Write bit
3. MCU sends register address
4. MCU sends REPEATED START
5. MCU sends device address (0x34) + Read bit
6. DS2777G sends register data
7. MCU sends STOP condition

**Update Rate**: 1-10 Hz (periodic sensor reading task)

**Error Handling**:
- Bus timeout detection
- NACK handling
- Data validation

---

### I2C Bus 2 (USCI_B2) - Slave Mode

**Purpose**: Communication with OBDH (On-Board Data Handling) module

**Configuration**:
- Mode: Slave
- Speed: 100 kbps
- Slave Address: 0x36
- USCI Module: USCI_B2
- Buffer IC: TCA4311A (I2C buffer with protection)

**Protocol**: FloripaSat Protocol (FSP)

**FSP Packet Structure**:
```
┌─────────┬─────────┬─────────┬─────────┬─────────┐
│  START  │ ADDRESS │ COMMAND │  DATA   │   CRC   │
├─────────┼─────────┼─────────┼─────────┼─────────┤
│ 1 byte  │ 1 byte  │ 1 byte  │ N bytes │ 1 byte  │
└─────────┴─────────┴─────────┴─────────┴─────────┘
```

**CRC Algorithm**: CRC8-CCITT
- Polynomial: 0x07
- Initial value: 0x00
- Used for error detection in packet transmission

**Control Pins**:
- **I2C Buffer Enable**: GPIO_PIN_66
  - Function: Enables TCA4311A I2C buffer
  - Active: HIGH

- **I2C Buffer Ready**: GPIO_PIN_69
  - Function: Indicates I2C buffer ready status
  - Read by MCU to check bus availability

**Supported Commands** (typical):
- Read parameter(s) from data buffer
- Write configuration parameters
- System reset command
- Status query

**Data Transfer Pattern**:
```
OBDH (Master) → Command Packet → EPS2 (Slave)
EPS2 → Process Command → Data Buffer
EPS2 (Slave) → Response Packet → OBDH (Master)
```

**Response Time**: Typically < 10 ms

**Error Handling**:
- CRC validation on received packets
- Invalid command handling
- Buffer overflow protection
- TCA4311A provides:
  - Live insertion capability
  - Rise-time acceleration
  - Bus-fault protection

**Interrupt-Driven Operation**:
- I2C slave interrupts trigger on:
  - Address match
  - Data received
  - Data requested
- Interrupts signal Device Response Task via FreeRTOS event groups

---

## UART Interfaces

### UART 0 (USCI_A0) - TTC Communication

**Purpose**: Communication with TTC (Telemetry, Tracking, and Command) beacon radio module

**Configuration**:
- Baud Rate: 9600 bps
- Data Bits: 8
- Stop Bits: 1
- Parity: None
- Flow Control: None
- USCI Module: USCI_A0

**Function**:
- Transmit beacon telemetry data
- Send critical system health information
- Periodic transmission of EPS status

**Data Format**:
- ASCII or binary telemetry packets
- Typically includes:
  - Battery voltage and charge
  - Solar panel status
  - System temperature
  - Critical fault indicators

**Transmission Pattern**:
```
EPS2 → Telemetry Packet → TTC Module → RF Transmission
```

**Update Rate**: As required by mission profile (typically every 30-60 seconds)

**Error Handling**:
- Transmit buffer overflow protection
- Timeout detection
- No acknowledgment (one-way communication)

---

### UART 1 (USCI_A2) - Debug Console

**Purpose**: Debug logging and system diagnostics

**Configuration**:
- Baud Rate: 115200 bps
- Data Bits: 8
- Stop Bits: 1
- Parity: None
- Flow Control: None
- USCI Module: USCI_A2

**Function**:
- System log output
- Debug messages
- Error reporting
- Task execution traces
- ANSI-formatted messages

**Message Format**:
- ANSI escape codes for color-coded output
- Timestamp prefixes
- Log level indicators (INFO, WARNING, ERROR, DEBUG)

**Example Output**:
```
[INFO] System initialized
[DEBUG] Battery voltage: 3850 mV
[WARNING] Temperature above threshold
[ERROR] I2C communication timeout
```

**Usage**:
- Development and testing
- System diagnostics
- Troubleshooting
- Not used in flight operations (can be disabled to save power)

**Implementation**:
- Non-blocking transmit (interrupt-driven)
- Circular buffer for log messages
- Optional logging levels for filtering

---

## SPI Interface

### SPI 0 (USCI_A1) - Temperature Sensor ADC

**Purpose**: Communication with ADS1248 24-bit ADC for RTD temperature sensors

**Configuration**:
- Mode: Master
- SPI Mode: Mode 1 (CPOL=0, CPHA=1)
- Clock Speed: 100 kHz
- Data Order: MSB first
- USCI Module: USCI_A1

**Connected Device**:
- **ADS1248 24-bit Sigma-Delta ADC** (Texas Instruments)
  - Function: High-precision temperature measurement via RTD sensors
  - Channels: 7 RTD inputs (MUX-based)
  - Resolution: 24-bit
  - Reference: 3.3V external reference

**Control Pins**:
- **Chip Select (CS)**: SPI_CS_0
  - Active LOW
  - Selects ADS1248 for communication

- **Start Pin**: GPIO_PIN_60
  - Function: Initiates ADC conversion
  - Pulse HIGH to start

- **Reset Pin**: GPIO_PIN_58
  - Function: Hardware reset of ADS1248
  - Active LOW

**RTD Channels**:
- Channel 0-3: Battery module temperatures
- Channel 4-6: Solar panel temperatures

**Communication Sequence**:
1. Assert CS (LOW)
2. Send command byte
3. Wait for conversion (if reading)
4. Read 24-bit result (3 bytes)
5. De-assert CS (HIGH)

**ADS1248 Commands**:
- RESET: 0x06
- START/SYNC: 0x08
- READ DATA: 0x12
- WREG: Write register
- RREG: Read register

**Data Conversion**:
```
Temperature (°C) = f(ADC_Value, RTD_Calibration_Table)
```

**Update Rate**: 0.1-1 Hz (slow due to thermal time constants)

**Error Handling**:
- SPI timeout detection
- Invalid data detection (out-of-range values)
- Sensor fault detection (open/short circuit)

---

## JTAG Interface

### Spy-Bi-Wire Programming and Debug

**Purpose**: Firmware programming and in-circuit debugging

**Configuration**:
- Protocol: Spy-Bi-Wire (2-wire JTAG)
- Pins: TEST and RST
- Compatible with MSP430 programming tools

**Functions**:
- Firmware download
- Flash memory programming
- Real-time debugging
- Breakpoint setting
- Register inspection
- Memory access

**Tools**:
- Code Composer Studio (CCS) debugger
- MSP-FET programmer
- MSP-FET430UIF (legacy)

**Usage**:
- Development and testing
- Firmware updates
- System debugging
- Typically disabled or protected in flight hardware

---

## GPIO Interfaces

### Hardware Version Detection

**Purpose**: Detect PCB hardware revision via resistor dividers

**Implementation**:
- Multiple GPIO pins configured as analog inputs
- Resistor divider networks on PCB
- ADC reads voltage levels to determine version

**Usage**:
- Read at startup
- Stored in data buffer
- Allows firmware to adapt to hardware changes

### Payload Power Control

**Purpose**: Enable/disable power to payload module

**Configuration**:
- Pin: GPIO_PIN_3
- Direction: Output
- Function: Controls payload power switch

**Control**:
- HIGH: Payload power enabled
- LOW: Payload power disabled

**Usage**:
- Commanded via OBDH
- Power-saving during non-operational periods

### LED Indicators

**System LED**: GPIO_PIN_36
- Function: Heartbeat / System alive indicator
- Pattern: Periodic blinking

**Fault LED**: GPIO_PIN_35
- Function: Error/fault indication
- Pattern: Solid or rapid blink during fault condition

---

## PC/104 Connector Interface

### Power Bus Connections

**Purpose**: Mechanical and electrical interface to other satellite modules

**Electrical Connections**:
- Main power bus distribution
- Ground connections
- GPIO signals for module communication
- Reserved pins for expansion

**Mechanical**:
- Standard PC/104 form factor
- Stackable connector system
- 90mm x 96mm mounting pattern (modified for 2U CubeSat: 90mm x 93mm)

---

## Internal Interfaces (Firmware)

### Data Buffer Interface

**Purpose**: Centralized data access for all tasks

**API Functions**:
- Read parameter(s)
- Write parameter(s)
- Lock/unlock for thread safety

**Access Pattern**:
```c
eps2_data_read(PARAM_ID, &value);
eps2_data_write(PARAM_ID, value);
```

### Event Group Interface

**Purpose**: Task synchronization and signaling

**Events** (typical):
- Sensor data ready
- I2C command received
- MPPT iteration complete
- Heater control update

**Usage**:
```c
xEventGroupSetBits(event_group, EVENT_BIT);
xEventGroupWaitBits(event_group, EVENT_BIT, ...);
```

---

## Interface Summary Table

| Interface | Type   | Speed      | Direction | Purpose                     | Module      |
|-----------|--------|------------|-----------|----------------------------|-------------|
| I2C0      | I2C    | 100 kbps   | Master    | Battery Monitor            | USCI_B0     |
| I2C2      | I2C    | 100 kbps   | Slave     | OBDH Communication         | USCI_B2     |
| UART0     | UART   | 9600 bps   | TX        | TTC Beacon                 | USCI_A0     |
| UART1     | UART   | 115200 bps | TX        | Debug Console              | USCI_A2     |
| SPI0      | SPI    | 100 kHz    | Master    | Temperature ADC (ADS1248)  | USCI_A1     |
| JTAG      | SBW    | Variable   | Bidirect. | Programming/Debug          | Native      |
| GPIO      | Digital| N/A        | Various   | Control, Status, LEDs      | Native      |

---

## Timing Considerations

**I2C Bus Timing**:
- Max clock frequency: 100 kHz (standard mode)
- Setup time: Per I2C specification
- Hold time: Per I2C specification

**SPI Timing**:
- Clock frequency: 100 kHz
- Setup/hold: Per ADS1248 datasheet

**UART Timing**:
- Bit time: Determined by baud rate
- 9600 bps: 104.2 µs per bit
- 115200 bps: 8.68 µs per bit

**Response Times**:
- I2C slave response: < 10 ms (typical)
- Command processing: < 100 ms (typical)
- Sensor reading: 1-100 ms (depends on sensor)

---

## Power Consumption

**Active Communication**:
- I2C: ~1-5 mA (transient)
- UART: ~2-10 mA (depending on baud rate)
- SPI: ~1-5 mA (transient)

**Idle State**:
- I2C: < 1 µA
- UART: ~100 µA (if enabled)
- SPI: < 1 µA
