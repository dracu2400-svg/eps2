# EPS2 Firmware Architecture

## Overview

The EPS2 firmware is built on a layered architecture running on FreeRTOS v10.2.1, designed for the MSP430F6659IPZR microcontroller. The architecture follows a modular design pattern with clear separation of concerns across hardware abstraction, drivers, operating system, device management, and application layers.

## Architecture Layers

```
┌─────────────────────────────────────────────┐
│         Application Layer                   │
│  (Tasks, Data Structures, Protocols)        │
├─────────────────────────────────────────────┤
│         Devices Layer                       │
│  (LEDs, MPPT, Battery, Sensors, etc.)       │
├─────────────────────────────────────────────┤
│         FreeRTOS Layer                      │
│  (Task Scheduler, Event Groups, Queues)     │
├─────────────────────────────────────────────┤
│         Drivers Layer                       │
│  (GPIO, I2C, SPI, UART, ADC, etc.)          │
├─────────────────────────────────────────────┤
│    Hardware Abstraction Layer (HAL)         │
│  (MSP430 DriverLib v2.91.11.01)             │
└─────────────────────────────────────────────┘
```

## Layer Details

### 1. Hardware Abstraction Layer (HAL)

**Location**: `/home/user/eps2/firmware/hal/`

**Purpose**: Provides low-level hardware access and register manipulation for the MSP430 microcontroller.

**Components**:
- MSP430 DriverLib v2.91.11.01 (Texas Instruments)
- Direct register access macros
- Hardware-specific initialization routines
- Interrupt vector definitions

**Key Features**:
- Abstraction of MSP430 peripheral registers
- Standard API for hardware configuration
- Minimal overhead for embedded systems

### 2. Drivers Layer

**Location**: `/home/user/eps2/firmware/drivers/`

**Purpose**: Implements hardware peripheral drivers with a consistent interface.

**Available Drivers**:

#### Communication Drivers
- **I2C Driver** (`i2c/`)
  - Master and slave mode support
  - 2 I2C buses (USCI_B0, USCI_B2)
  - Configurable speed (default 100 kbps)
  - Interrupt-driven communication

- **SPI Driver** (`spi/`)
  - Master mode operation
  - Configurable clock polarity and phase
  - Multiple chip select support
  - Used for ADS1248 ADC

- **UART Driver** (`uart/`)
  - 2 UART interfaces (USCI_A0, USCI_A2)
  - Configurable baud rates (9600, 115200)
  - Interrupt-based TX/RX
  - Debug logging support

#### Analog Drivers
- **ADC Driver** (`adc/`)
  - 12-bit internal ADC
  - Multiple channel support
  - Voltage and current sensing
  - Temperature measurement

#### Digital I/O
- **GPIO Driver** (`gpio/`)
  - Pin configuration (input/output)
  - Interrupt support
  - Pull-up/pull-down configuration
  - 74 GPIO pins available

#### Timing and Control
- **PWM Driver** (`pwm/`)
  - Timer-based PWM generation
  - Used for MPPT and heater control
  - Configurable duty cycle and frequency

- **Watchdog Driver** (`wdt/`)
  - Hardware watchdog timer
  - Periodic reset mechanism
  - System health monitoring

#### Storage
- **Flash Driver** (`flash/`)
  - Internal flash memory access
  - Non-volatile data storage
  - Parameter persistence

#### Specialized
- **OneWire Driver** (`onewire/`)
  - OneWire protocol implementation
  - Used for specialized sensors

### 3. Operating System Layer

**Location**: `/home/user/eps2/firmware/freertos/`

**Components**:
- **FreeRTOS v10.2.1**
  - Real-time task scheduler
  - Preemptive multitasking
  - Task priorities and synchronization

**FreeRTOS Configuration** (`/home/user/eps2/firmware/config/FreeRTOSConfig.h`):
```c
configCPU_CLOCK_HZ          = 32000000 (32 MHz)
configTICK_RATE_HZ          = 1000 (1 ms tick)
configMAX_PRIORITIES        = 5
configMINIMAL_STACK_SIZE    = 128 words
configTOTAL_HEAP_SIZE       = varies
```

**Synchronization Mechanisms**:
- **Event Groups**: Inter-task communication and synchronization
- **Semaphores**: Resource protection
- **Queues**: Message passing between tasks
- **Mutexes**: Critical section protection

### 4. Devices Layer

**Location**: `/home/user/eps2/firmware/devices/`

**Purpose**: High-level device abstractions built on top of drivers.

**Device Modules**:

#### Power Management
- **MPPT Device** (`mppt/`)
  - Maximum Power Point Tracking algorithm
  - 3-channel solar panel control
  - PWM duty cycle management

- **Battery Monitor** (`battery/`)
  - DS2777G I2C interface
  - Voltage, current, charge tracking
  - Temperature monitoring

- **Heater Device** (`heater/`)
  - PID control algorithm
  - PWM-based heater control
  - Temperature feedback

#### Communication
- **OBDH Device** (`obdh/`)
  - I2C slave communication
  - FloripaSat Protocol (FSP) implementation
  - Command processing
  - Data buffer interface

- **TTC Device** (`ttc/`)
  - UART beacon communication
  - Telemetry transmission

#### Sensors
- **Temperature Sensors** (`temp_sensor/`)
  - ADS1248 24-bit ADC interface
  - 7 RTD channels
  - MCU internal temperature
  - Calibration and conversion

- **Voltage Sensors** (`voltage/`)
  - Resistor divider networks
  - 5 voltage channels
  - Calibrated measurements

- **Current Sensors** (`current/`)
  - MAX9934 current sense amplifiers
  - 7 current channels
  - High-side sensing

#### Indicators
- **LED Device** (`leds/`)
  - System LED (heartbeat)
  - Fault LED (error indication)
  - GPIO-based control

### 5. Application Layer

**Location**: `/home/user/eps2/firmware/app/`

**Purpose**: Implements system-level tasks and business logic.

**Components**:

#### Task Implementations (`tasks/`)
- Startup task
- Watchdog reset task
- Heartbeat task
- System reset task
- Read sensors task
- Parameter server task
- MPPT algorithm task
- Heater controller task
- Device response task
- Time control task

#### Data Structures (`structs/`)
- **EPS2 Data Buffer** (`eps2_data.h`)
  - 49 system parameters
  - Unified data structure
  - Thread-safe access

#### Communication Protocols
- **FSP Protocol** (FloripaSat Protocol)
  - CRC8-CCITT error checking
  - Command/response format
  - Parameter read/write operations

## System Clock Configuration

**Main Clock (MCLK)**:
- Frequency: 32 MHz
- Source: Internal DCO
- Usage: CPU execution

**Sub-Main Clock (SMCLK)**:
- Frequency: 32 MHz
- Source: DCO
- Usage: Peripheral modules

**Auxiliary Clock (ACLK)**:
- Frequency: 32.768 kHz
- Source: XT1 crystal oscillator
- Usage: Low-power timers, RTC

## Memory Architecture

**Flash Memory**:
- Total: 512 KB
- Program code storage
- Non-volatile parameters
- Firmware updates

**RAM**:
- Total: 64 KB
- Task stacks
- FreeRTOS heap
- Data buffers
- Global variables

**Memory Allocation**:
- Each FreeRTOS task has dedicated stack space (configurable)
- Heap used for dynamic allocations
- Static buffers for sensor data and communication

## Interrupt Architecture

**Interrupt Priorities** (MSP430):
- Highest: Non-Maskable Interrupts (NMI)
- High: Timer interrupts (MPPT, Heater PWM)
- Medium: Communication interrupts (I2C, UART, SPI)
- Low: GPIO interrupts

**Interrupt Handlers**:
- Keep ISRs short and non-blocking
- Use FreeRTOS-safe API calls (FromISR variants)
- Signal tasks via event groups or semaphores

## Build Configuration

**Development Environment**:
- IDE: Code Composer Studio (CCS) v9.0
- Compiler: TI MSP430 Compiler
- Optimization: -O2 (typical)
- Debug: JTAG Spy-Bi-Wire

**Configuration Files**:
- Main config: `/home/user/eps2/firmware/config/config.h`
- FreeRTOS config: `/home/user/eps2/firmware/config/FreeRTOSConfig.h`
- Linker script: `/home/user/eps2/firmware/lnk_msp430f6659.cmd`

## Design Patterns

### 1. Layered Architecture
Clear separation between hardware, drivers, OS, devices, and application.

### 2. Task-Based Design
Each major functionality runs as independent FreeRTOS task.

### 3. Data Abstraction
Centralized data buffer with accessor functions (Parameter Server).

### 4. Event-Driven Communication
Tasks communicate via event groups and queues.

### 5. Hardware Abstraction
Device layer abstracts hardware details from application logic.

## Initialization Sequence

1. **Hardware Initialization**
   - Clock system configuration
   - GPIO initialization
   - Watchdog configuration

2. **Driver Initialization**
   - I2C, SPI, UART setup
   - ADC initialization
   - PWM timer configuration

3. **Device Initialization**
   - Battery monitor
   - Temperature sensors
   - OBDH communication

4. **FreeRTOS Initialization**
   - Create tasks
   - Initialize synchronization objects
   - Start scheduler

5. **Application Start**
   - Startup task executes
   - System transitions to normal operation
   - Periodic tasks begin execution

## Version Information

- **Firmware Version**: 0.4.0 (Development)
- **FreeRTOS Version**: v10.2.1
- **MSP430 DriverLib**: v2.91.11.01
- **Hardware Version**: Detected at runtime via resistor dividers
