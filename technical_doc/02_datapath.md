# EPS2 Data Path

## Overview

The EPS2 data path describes how information flows through the system, from sensor acquisition to data storage and communication. The system manages 49 distinct parameters organized in a centralized data buffer structure.

## Data Buffer Structure

**Location**: `/home/user/eps2/firmware/app/structs/eps2_data.h`

The EPS2 maintains a comprehensive data structure containing all system parameters. This structure is managed by the Parameter Server task and provides thread-safe access to all system data.

### Complete Parameter List (49 parameters)

#### System Parameters
1. **System Time** - Current system time in seconds
2. **Reset Cause** - Last system reset reason
3. **MCU Temperature** - Internal MCU temperature (°C)
4. **Reset Counter** - Number of system resets

#### Solar Panel Voltages (4 parameters)
5. **Solar Panel -Y Voltage** - Voltage from -Y panel group (mV)
6. **Solar Panel +Y Voltage** - Voltage from +Y panel group (mV)
7. **Solar Panel -X Voltage** - Voltage from -X panel group (mV)
8. **Solar Panel +X Voltage** - Voltage from +X panel group (mV)
9. **Solar Panel -Z Voltage** - Voltage from -Z panel group (mV)
10. **Solar Panel +Z Voltage** - Voltage from +Z panel group (mV)
11. **Solar Panel Total Voltage** - Combined solar panel voltage (mV)

#### Solar Panel Currents (6 parameters)
12. **Solar Panel -Y Current** - Current from -Y panel (mA)
13. **Solar Panel +Y Current** - Current from +Y panel (mA)
14. **Solar Panel -X Current** - Current from -X panel (mA)
15. **Solar Panel +X Current** - Current from +X panel (mA)
16. **Solar Panel -Z Current** - Current from -Z panel (mA)
17. **Solar Panel +Z Current** - Current from +Z panel (mA)

#### Solar Panel Temperatures (3 parameters)
18. **Solar Panel -Y Temperature** - RTD temperature sensor (°C)
19. **Solar Panel +Y Temperature** - RTD temperature sensor (°C)
20. **Solar Panel -Z Temperature** - RTD temperature sensor (°C)

#### Battery Parameters (5 parameters)
21. **Battery Voltage** - Main battery voltage (mV)
22. **Battery Current** - Charge/discharge current (mA, signed)
23. **Battery Charge** - Remaining capacity (mAh)
24. **Battery Monitor Temperature** - DS2777G internal temp (°C)
25. **Battery Average Current** - Average current over time (mA)

#### Battery Temperatures (4 parameters)
26. **Battery Temperature 1** - RTD channel 0 (°C)
27. **Battery Temperature 2** - RTD channel 1 (°C)
28. **Battery Temperature 3** - RTD channel 2 (°C)
29. **Battery Temperature 4** - RTD channel 3 (°C)

#### Power Bus Parameters
30. **Main Bus Voltage** - System power bus voltage (mV)
31. **EPS + Beacon Current** - Combined current consumption (mA)

#### MPPT Parameters (3 parameters)
32. **MPPT Channel 0 Duty Cycle** - PWM duty cycle (%)
33. **MPPT Channel 1 Duty Cycle** - PWM duty cycle (%)
34. **MPPT Channel 2 Duty Cycle** - PWM duty cycle (%)

#### Heater Parameters (2 parameters)
35. **Heater 0 Duty Cycle** - PWM duty cycle (%)
36. **Heater 1 Duty Cycle** - PWM duty cycle (%)

#### Version Information
37. **Hardware Version** - PCB hardware version
38. **Firmware Version Major** - Firmware major version
39. **Firmware Version Minor** - Firmware minor version
40. **Firmware Version Patch** - Firmware patch version

#### Additional Battery Monitor Data
41. **RAAC** - Remaining Active Absolute Capacity (mAh)
42. **RSAC** - Remaining Standby Absolute Capacity (mAh)
43. **RARC** - Remaining Active Relative Capacity (%)
44. **RSRC** - Remaining Standby Relative Capacity (%)
45. **Battery Status** - DS2777G status register
46. **Battery Protection** - DS2777G protection register

#### RTD Temperatures (additional)
47. **RTD Temperature 4** - Additional RTD channel (°C)
48. **RTD Temperature 5** - Additional RTD channel (°C)
49. **RTD Temperature 6** - Additional RTD channel (°C)

## Data Flow Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                    Sensor Acquisition                        │
├──────────────┬──────────────┬──────────────┬────────────────┤
│   Internal   │     ADC      │     I2C      │      SPI       │
│   MCU Temp   │   Sensors    │   Battery    │   ADS1248      │
│              │              │   Monitor    │   (RTD)        │
└──────┬───────┴──────┬───────┴──────┬───────┴────────┬───────┘
       │              │              │                │
       v              v              v                v
┌─────────────────────────────────────────────────────────────┐
│              Read Sensors Task (Periodic)                    │
│  - Voltage measurements (5 channels)                         │
│  - Current measurements (7 channels)                         │
│  - Temperature measurements (8 channels)                     │
│  - Battery monitor data (DS2777G)                            │
└──────────────────────────┬──────────────────────────────────┘
                           │
                           v
┌─────────────────────────────────────────────────────────────┐
│              Parameter Server (Data Buffer)                  │
│  - Thread-safe read/write operations                         │
│  - 49 parameters stored in unified structure                 │
│  - Provides data to all tasks                                │
└──┬──────────┬──────────┬──────────────┬───────────────┬─────┘
   │          │          │              │               │
   v          v          v              v               v
┌──────┐  ┌──────┐  ┌─────────┐  ┌──────────┐  ┌────────────┐
│ MPPT │  │Heater│  │  OBDH   │  │   TTC    │  │   Flash    │
│ Task │  │ Task │  │Command  │  │ Beacon   │  │  Storage   │
│      │  │      │  │Response │  │          │  │            │
└──────┘  └──────┘  └─────────┘  └──────────┘  └────────────┘
```

## Data Acquisition Paths

### 1. Voltage Measurements (ADC-based)

**Path**: Hardware → Internal ADC → Driver → Read Sensors Task → Data Buffer

**Channels** (5 total):
- Solar panel group voltages (3 channels via resistor dividers)
- Total solar panel voltage (1 channel)
- Main bus voltage (1 channel)

**Resistor Divider Ratios**:
- Solar panels: 4.0x division
- Main bus: 1.93x division

**Conversion**:
```
Actual Voltage = ADC_Reading × (VREF / 4096) × Division_Factor
```

**Update Rate**: Periodic (configurable, typically 1-10 Hz)

### 2. Current Measurements (ADC-based)

**Path**: Hardware → MAX9934 Amplifier → Internal ADC → Driver → Read Sensors Task → Data Buffer

**Channels** (7 total):
- 6 solar panel currents
- 1 EPS + Beacon current

**Sensing Parameters**:
- Current sense amplifier: MAX9934
- Amplifier gain: 25 µA/mV
- Sense resistors:
  - Solar panels: 20 mΩ
  - EPS current: 75 mΩ

**Conversion**:
```
Current (mA) = ADC_Reading × (VREF / 4096) / Rsense / Gain
```

**Update Rate**: Periodic (configurable, typically 1-10 Hz)

### 3. Temperature Measurements (Multiple Paths)

#### Path A: Internal MCU Temperature
**Path**: MCU Temp Sensor → Internal ADC → Driver → Read Sensors Task → Data Buffer

**Conversion**: MCU-specific temperature calibration formula

#### Path B: RTD Temperature Sensors (7 channels)
**Path**: RTD Sensors → ADS1248 ADC (SPI) → Driver → Read Sensors Task → Data Buffer

**RTD Channels**:
- RTD 0-3: Battery module temperatures
- RTD 4-6: Solar panel temperatures

**ADS1248 Configuration**:
- Resolution: 24-bit
- Reference voltage: 3.3V
- SPI interface
- Conversion: RTD resistance → Temperature via calibration table

**Update Rate**: Periodic (configurable, typically 0.1-1 Hz due to thermal time constants)

### 4. Battery Monitor Data (I2C-based)

**Path**: DS2777G Battery Monitor (I2C) → I2C Driver → Read Sensors Task → Data Buffer

**DS2777G Registers Read**:
- Voltage register (0x0C-0x0D)
- Current register (0x0E-0x0F)
- Accumulated current (0x10-0x11)
- Temperature register (0x0A-0x0B)
- Status register (0x01)
- RAAC, RSAC, RARC, RSRC registers

**I2C Configuration**:
- Bus: I2C0 (USCI_B0)
- Speed: 100 kbps
- 7-bit addressing

**Update Rate**: Periodic (configurable, typically 1-10 Hz)

### 5. Control Output Data (Generated)

**Path**: Algorithm Tasks → Data Buffer

**MPPT Duty Cycles**:
- Generated by MPPT Algorithm Task
- Based on solar panel voltage/current measurements
- Updated continuously during MPPT operation

**Heater Duty Cycles**:
- Generated by Heater Controller Task
- Based on PID algorithm using battery temperatures
- Updated continuously during heater operation

## Data Consumption Paths

### 1. MPPT Algorithm Task

**Input Data** (from Data Buffer):
- Solar panel voltages (3 channels)
- Solar panel currents (6 channels)

**Processing**:
- Perturb and Observe (P&O) algorithm
- Power calculation: P = V × I
- Duty cycle adjustment

**Output Data** (to Data Buffer):
- MPPT duty cycles (3 channels)

**Output Hardware**:
- PWM signals to power converters (Timer B0)

### 2. Heater Controller Task

**Input Data** (from Data Buffer):
- Battery temperatures (RTD channels)
- Target temperature setpoint

**Processing**:
- PID control algorithm
- Error calculation: e = Setpoint - Temperature
- PID terms: P, I, D

**Output Data** (to Data Buffer):
- Heater duty cycles (2 channels)

**Output Hardware**:
- PWM signals to heaters (Timer A1)

### 3. Device Response Task (OBDH Communication)

**Input**:
- Commands from OBDH via I2C slave interface
- FloripaSat Protocol (FSP) packets

**Processing**:
- Command parsing
- CRC8-CCITT validation
- Parameter read/write operations

**Data Access**:
- Reads any of 49 parameters from Data Buffer
- Writes configuration parameters (if applicable)

**Output**:
- Response packets to OBDH via I2C
- Updated parameters in Data Buffer

### 4. TTC Beacon Task

**Input Data** (from Data Buffer):
- Critical telemetry parameters
- System health status

**Processing**:
- Format beacon message
- Prepare telemetry packet

**Output**:
- UART transmission to TTC module (9600 baud)

### 5. Flash Storage

**Input Data** (from Data Buffer):
- Parameters to persist
- Configuration data
- Log entries

**Processing**:
- Flash write operations
- Data verification

**Storage**:
- Non-volatile flash memory
- Retrieved on system startup

## Data Synchronization

### Thread Safety

**Mechanism**: FreeRTOS synchronization primitives

**Protection Methods**:
- Mutexes for data buffer access
- Critical sections for atomic operations
- Semaphores for task signaling

**Access Pattern**:
```c
// Read operation
mutex_lock(data_buffer_mutex);
value = data_buffer.parameter;
mutex_unlock(data_buffer_mutex);

// Write operation
mutex_lock(data_buffer_mutex);
data_buffer.parameter = new_value;
mutex_unlock(data_buffer_mutex);
```

### Update Rates

**Fast Updates** (1-10 Hz):
- Voltage measurements
- Current measurements
- Battery monitor data

**Medium Updates** (0.1-1 Hz):
- Temperature measurements (RTD)
- Battery charge calculations

**Slow Updates** (0.01-0.1 Hz):
- System time updates
- Version information (static)

**Event-Driven Updates**:
- Reset cause (on reset event)
- MPPT duty cycles (on algorithm iteration)
- Heater duty cycles (on PID iteration)

## Data Validation

### Error Detection

**CRC Checking**:
- I2C communication with OBDH uses CRC8-CCITT
- FSP protocol packets include CRC

**Range Checking**:
- Voltage readings checked against expected ranges
- Current readings validated for physical limits
- Temperature readings checked for sensor validity

**Timeout Detection**:
- Watchdog ensures tasks are running
- Communication timeouts detected

### Error Handling

**Invalid Sensor Data**:
- Marked as invalid in data buffer
- Previous valid value retained
- Error flag set

**Communication Errors**:
- CRC mismatch: Packet rejected
- Timeout: Communication retry
- Persistent errors: Fault LED activation

## Data Persistence

**Non-Volatile Storage**:
- Configuration parameters stored in flash
- Reset counter persisted
- Calibration data stored

**Volatile Data**:
- Real-time sensor readings (not persisted)
- Calculated values (regenerated on boot)
- Transient status information

**Backup Strategy**:
- Critical parameters written to flash periodically
- Restored on system startup
- Default values used if flash corrupted
