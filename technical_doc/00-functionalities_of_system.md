# EPS2 System Functionalities

## Overview

**EPS2 (Electrical Power System 2.0)** is a comprehensive power management module designed and developed by SpaceLab (UFSC) for the FloripaSat-2 CubeSat mission. It serves as the complete energy management solution for a 2U nanosatellite.

## Primary Functions

### 1. Energy Harvesting

- **Solar Panel Management**: Interfaces with 10 solar panels distributed around the satellite
- **Multi-Panel Configuration**: Manages 3 distinct solar panel groups:
  - Channel 0: -Y and +X panels
  - Channel 1: -X and +Z panels
  - Channel 2: -Z and +Y panels
- **Current Monitoring**: Real-time monitoring of 6 solar panel currents using MAX9934 current sense amplifiers
- **Voltage Monitoring**: Tracks voltage levels across 3 panel groups plus total solar output

### 2. Maximum Power Point Tracking (MPPT)

- **Adaptive Algorithm**: Implements MPPT algorithm to optimize solar energy conversion
- **PWM Control**: Uses pulse-width modulation to adjust power extraction from solar panels
- **Per-Channel Optimization**: Independent MPPT control for each of the 3 solar panel groups
- **Dynamic Adjustment**: Continuously adapts to changing solar irradiance and temperature conditions

### 3. Battery Charging and Management

- **Battery Configuration**: Manages 4 lithium-ion batteries in series/parallel configuration
- **Maximum Capacity**: 2450 mAh total charge capacity
- **Intelligent Charging**: DS2777G battery monitor IC provides:
  - Voltage monitoring
  - Charge/discharge current tracking
  - Accumulated charge (RAAC) and remaining capacity (RARC)
  - State of charge calculation
  - Over-voltage/under-voltage protection
- **Temperature Monitoring**: 4 RTD temperature sensors on battery module

### 4. Energy Distribution

- **DC-DC Conversion**: Multiple TPS54x0 buck converters for power distribution
- **Main Power Bus**: Regulated power bus for satellite subsystems
- **Payload Power Control**: Dedicated enable/disable control for payload (GPIO_PIN_3)
- **Current Monitoring**: Tracks EPS and Beacon current consumption

### 5. Thermal Management

- **Battery Heater Control**: 2 PWM-controlled heaters to maintain optimal battery temperature
- **PID Algorithm**: Proportional-Integral-Derivative control for precise temperature regulation
- **Target Temperature**: Maintains batteries within safe operating range
- **Heater Channels**:
  - Channel 0: Controlled by RTD Channel 6
  - Channel 1: Controlled by RTD Channel 2

### 6. System Monitoring

The EPS2 continuously monitors and reports 49 system parameters:

#### Voltage Measurements (5 channels)
- Solar panel group voltages (3 channels)
- Total solar panel voltage
- Main power bus voltage

#### Current Measurements (7 channels)
- Solar panel currents: -Y, +Y, -X, +X, -Z, +Z
- EPS + Beacon current

#### Temperature Measurements (8 channels)
- MCU internal temperature
- 4 battery module RTD sensors
- 3 solar panel RTD sensors

#### Battery Status
- Battery voltage
- Battery current (charge/discharge)
- Battery charge (mAh)
- Battery temperatures

#### System Status
- System time
- Reset cause tracking
- MCU temperature
- MPPT duty cycles
- Heater duty cycles
- Hardware/firmware versions

### 7. Communication Interface

- **OBDH Communication**: I2C slave interface for communication with On-Board Data Handling module
  - Slave address: 0x36
  - Speed: 100 kbps
  - Protocol: FloripaSat Protocol (FSP) with CRC8-CCITT error checking
  - TCA4311A I2C buffer for protection

- **TTC Communication**: UART interface for beacon radio communication
  - Baud rate: 9600 bps
  - Direct connection to TTC module

- **Debug Interface**: UART debug output
  - Baud rate: 115200 bps
  - ANSI-formatted system logs

### 8. System Health and Safety

- **Watchdog Timer**: Hardware watchdog with periodic reset task
- **Periodic System Reset**: Automatic system reset every 10 hours
- **Reset Cause Tracking**: Logs and reports system reset reasons
- **Kill Switches**: Ground testing and launch safety features
- **Fault Indication**: Dedicated fault LED (GPIO_PIN_35)
- **Heartbeat Indicator**: System alive LED (GPIO_PIN_36)

### 9. Data Management

- **Parameter Server**: Centralized data buffer management for all 49 system parameters
- **Flash Storage**: Internal flash memory for persistent data storage
- **Command Processing**: Handles external commands from OBDH/TTC modules
- **Data Buffering**: Efficient data structure for sensor readings and system state

## Key Specifications

- **Target Platform**: 2U CubeSat
- **PCB Dimensions**: 90mm x 93mm x 1.6mm (4-layer FR-4)
- **Firmware Version**: 0.4.0 (Development)
- **MCU**: MSP430F6659IPZR
  - 16-bit RISC architecture
  - 512KB Flash memory
  - 64KB RAM
  - Operating frequency: 32MHz
- **RTOS**: FreeRTOS v10.2.1
- **IDE**: Code Composer Studio (CCS) v9.0
- **Power Budget**: Optimized for low-power CubeSat operations

## Operational Modes

The EPS2 operates continuously, with all functionalities running concurrently through FreeRTOS task scheduling. The system is designed to:

1. Maximize energy harvesting from available solar irradiance
2. Maintain battery health through intelligent charging and thermal control
3. Provide reliable power distribution to all satellite subsystems
4. Monitor and report system health continuously
5. Respond to commands from OBDH for configuration and data retrieval
6. Ensure fail-safe operation through watchdog and reset mechanisms
