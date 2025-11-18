# EPS2 FreeRTOS Tasks

## Overview

The EPS2 firmware runs on FreeRTOS v10.2.1 and implements a multi-tasking architecture with 9-10 concurrent tasks. Each task handles a specific system function and runs independently with its own stack and priority level.

## Task Architecture

```
┌────────────────────────────────────────────────────────────┐
│                   FreeRTOS Scheduler                        │
│                 (Preemptive, Priority-based)                │
└────────────────────────────────────────────────────────────┘
                           │
    ┌──────────────────────┼──────────────────────┐
    │                      │                      │
    v                      v                      v
┌─────────┐          ┌─────────┐           ┌──────────┐
│ Startup │          │Watchdog │           │Heartbeat │
│  Task   │          │  Reset  │           │   Task   │
└─────────┘          └─────────┘           └──────────┘
    │
    v                      v                      v
┌─────────┐          ┌─────────┐           ┌──────────┐
│ System  │          │  Read   │           │Parameter │
│  Reset  │          │ Sensors │           │  Server  │
└─────────┘          └─────────┘           └──────────┘
                           │
                           v                      v
                    ┌──────────┐          ┌──────────┐
                    │   MPPT   │          │  Heater  │
                    │Algorithm │          │Controller│
                    └──────────┘          └──────────┘
                           │
                           v                      v
                    ┌──────────┐          ┌──────────┐
                    │  Device  │          │   Time   │
                    │ Response │          │ Control  │
                    └──────────┘          └──────────┘
```

## Task List

### 1. Startup Task

**Location**: `/home/user/eps2/firmware/app/tasks/startup.c`

**Priority**: Highest (runs once at system initialization)

**Stack Size**: Configurable (typically 256-512 words)

**Purpose**: System initialization and hardware configuration

**Execution**:
- Runs once at system boot
- Initializes all hardware peripherals
- Configures devices
- Detects hardware version
- Starts all other tasks
- Deletes itself after completion

**Key Operations**:
1. Hardware version detection (GPIO/ADC reading)
2. Clock system initialization
3. GPIO configuration
4. Peripheral initialization:
   - I2C buses (master and slave)
   - UART interfaces
   - SPI interface
   - ADC
   - PWM timers
5. Device initialization:
   - Battery monitor (DS2777G)
   - Temperature sensors (ADS1248)
   - LEDs
   - MPPT modules
   - Heaters
6. FreeRTOS object creation:
   - Event groups
   - Semaphores
   - Queues
7. Initial data buffer population
8. Boot status reporting

**Completion**: Task deletes itself using `vTaskDelete(NULL)` after initialization

---

### 2. Watchdog Reset Task

**Location**: `/home/user/eps2/firmware/app/tasks/watchdog_reset.c`

**Priority**: High

**Stack Size**: Small (128-256 words)

**Period**: Configurable (typically 100-500 ms)

**Purpose**: Periodic watchdog timer reset to prevent system reset

**Execution**:
```c
while(1) {
    WDT_A_resetTimer();
    vTaskDelay(WATCHDOG_RESET_PERIOD);
}
```

**Key Operations**:
1. Reset hardware watchdog timer
2. Delay for specified period
3. Repeat

**Watchdog Configuration**:
- Watchdog timeout: Longer than reset period (e.g., 1-2 seconds)
- If watchdog not reset: System hardware reset
- Prevents system hang/deadlock

**Failure Mode**:
- If this task fails to run: System resets automatically
- Reset cause logged for diagnostics

---

### 3. Heartbeat Task

**Location**: `/home/user/eps2/firmware/app/tasks/heartbeat.c`

**Priority**: Low

**Stack Size**: Small (128 words)

**Period**: Configurable (typically 500-1000 ms)

**Purpose**: Visual indication that system is running (LED blink)

**Execution**:
```c
while(1) {
    LED_Toggle(SYSTEM_LED);
    vTaskDelay(HEARTBEAT_PERIOD);
}
```

**Key Operations**:
1. Toggle system LED (GPIO_PIN_36)
2. Delay for specified period
3. Repeat

**LED Patterns**:
- Normal operation: Regular blink (e.g., 1 Hz)
- Can be modified for different system states

**Resource Usage**: Minimal (simple GPIO toggle)

---

### 4. System Reset Task

**Location**: `/home/user/eps2/firmware/app/tasks/system_reset.c`

**Priority**: Low

**Stack Size**: Small (128-256 words)

**Period**: Configurable (typically 10 hours)

**Purpose**: Periodic system reset to ensure long-term stability

**Execution**:
```c
while(1) {
    vTaskDelay(SYSTEM_RESET_PERIOD);

    // Prepare for reset
    SaveCriticalData();
    IncrementResetCounter();

    // Perform software reset
    PMMCTL0 = PMMPW | PMMSWBOR;  // Software BOR
}
```

**Key Operations**:
1. Wait for reset period (10 hours typical)
2. Save critical data to flash
3. Increment reset counter
4. Trigger software reset

**Reset Types**:
- Software Brown-Out Reset (BOR)
- Ensures clean system restart

**Reset Cause Tracking**:
- Reset reason logged in data buffer
- Stored in non-volatile memory

**Rationale**: Prevents accumulation of software errors over long missions

---

### 5. Read Sensors Task

**Location**: `/home/user/eps2/firmware/app/tasks/read_sensors.c`

**Priority**: Medium-High

**Stack Size**: Medium (256-512 words)

**Period**: Configurable (typically 100-1000 ms)

**Purpose**: Acquire all sensor data and update data buffer

**Execution**:
```c
while(1) {
    // Read voltage sensors
    ReadVoltageSensors(&voltages);

    // Read current sensors
    ReadCurrentSensors(&currents);

    // Read temperature sensors
    ReadMCUTemperature(&mcu_temp);
    ReadRTDTemperatures(&rtd_temps);

    // Read battery monitor
    ReadBatteryMonitor(&battery_data);

    // Update data buffer
    UpdateDataBuffer(voltages, currents, temps, battery_data);

    // Signal data ready
    xEventGroupSetBits(events, DATA_READY_BIT);

    vTaskDelay(READ_SENSORS_PERIOD);
}
```

**Key Operations**:

1. **Voltage Measurements** (5 channels):
   - Solar panel voltages (3 groups)
   - Total solar voltage
   - Main bus voltage
   - Via internal ADC with resistor dividers

2. **Current Measurements** (7 channels):
   - Solar panel currents (6 channels)
   - EPS + Beacon current
   - Via internal ADC with MAX9934 amplifiers

3. **Temperature Measurements**:
   - MCU internal temperature (1 channel)
   - RTD temperatures via ADS1248 SPI (7 channels)

4. **Battery Monitor Data** (I2C):
   - Battery voltage
   - Battery current
   - Accumulated charge (RAAC, RSAC)
   - Remaining capacity (RARC, RSRC)
   - Battery temperature
   - Status and protection registers

5. **Data Buffer Update**:
   - Write all readings to centralized data buffer
   - Thread-safe access using mutexes

6. **Event Signaling**:
   - Set event bits to notify other tasks
   - MPPT and Heater tasks wait for data ready

**Error Handling**:
- Sensor read failures logged
- Invalid data flagged
- Previous valid values retained on error

**Timing**:
- Critical for system operation
- Update rate affects control loop performance

---

### 6. Parameter Server Task

**Location**: `/home/user/eps2/firmware/app/tasks/param_server.c`

**Priority**: Medium

**Stack Size**: Medium (256-512 words)

**Period**: Event-driven (waits for requests)

**Purpose**: Centralized data buffer access and management

**Execution**:
```c
while(1) {
    // Wait for parameter access request
    xEventGroupWaitBits(events, PARAM_REQUEST_BIT, ...);

    // Process request
    if (request.type == READ) {
        value = ReadParameter(request.param_id);
        SendResponse(value);
    } else if (request.type == WRITE) {
        WriteParameter(request.param_id, request.value);
        SendAck();
    }
}
```

**Key Operations**:
1. Wait for parameter access request (event-driven)
2. Process read requests: Retrieve parameter from data buffer
3. Process write requests: Update parameter in data buffer
4. Provide thread-safe access to 49 system parameters
5. Send response to requesting task

**Thread Safety**:
- Mutex protection for data buffer access
- Prevents race conditions
- Ensures data consistency

**Parameter Types**:
- Read-only: Sensor values, version info
- Read-write: Configuration parameters, setpoints

**Access API**:
```c
eps2_param_read(PARAM_ID, &value);
eps2_param_write(PARAM_ID, value);
```

---

### 7. MPPT Algorithm Task

**Location**: `/home/user/eps2/firmware/app/tasks/mppt_algorithm.c`

**Priority**: Medium

**Stack Size**: Medium (256-512 words)

**Period**: Configurable (typically 100-500 ms)

**Purpose**: Maximum Power Point Tracking for solar panels

**Execution**:
```c
while(1) {
    // Wait for sensor data ready
    xEventGroupWaitBits(events, DATA_READY_BIT, ...);

    for (ch = 0; ch < 3; ch++) {
        // Read voltage and current for this channel
        V = GetSolarVoltage(ch);
        I = GetSolarCurrent(ch);

        // Calculate power
        P = V * I;

        // Perturb and Observe algorithm
        if (P > P_prev[ch]) {
            // Power increased, continue in same direction
            if (V > V_prev[ch]) {
                duty_cycle[ch] += DELTA;
            } else {
                duty_cycle[ch] -= DELTA;
            }
        } else {
            // Power decreased, reverse direction
            if (V > V_prev[ch]) {
                duty_cycle[ch] -= DELTA;
            } else {
                duty_cycle[ch] += DELTA;
            }
        }

        // Limit duty cycle
        if (duty_cycle[ch] > MAX_DUTY) duty_cycle[ch] = MAX_DUTY;
        if (duty_cycle[ch] < MIN_DUTY) duty_cycle[ch] = MIN_DUTY;

        // Update PWM
        SetPWM(ch, duty_cycle[ch]);

        // Store for next iteration
        P_prev[ch] = P;
        V_prev[ch] = V;
    }

    // Update data buffer
    UpdateMPPTDutyCycles(duty_cycle);

    vTaskDelay(MPPT_PERIOD);
}
```

**MPPT Channels**:
- Channel 0: -Y and +X solar panels
- Channel 1: -X and +Z solar panels
- Channel 2: -Z and +Y solar panels

**Algorithm**: Perturb and Observe (P&O)
1. Measure voltage and current
2. Calculate power: P = V × I
3. Compare with previous power
4. Adjust duty cycle to increase power
5. Repeat

**PWM Control**:
- Timer: Timer B0
- Frequency: Configurable (typically 10-100 kHz)
- Duty cycle range: 0-100% (typically limited to 10-90%)

**Convergence**:
- Step size (DELTA): Configurable (trade-off between speed and stability)
- Smaller steps: More stable, slower convergence
- Larger steps: Faster convergence, more oscillation

**Special Conditions**:
- No sun: Duty cycle set to minimum
- Battery full: MPPT disabled or reduced
- Over-voltage protection: Duty cycle limited

---

### 8. Heater Controller Task

**Location**: `/home/user/eps2/firmware/app/tasks/heater_controller.c`

**Priority**: Medium

**Stack Size**: Medium (256-512 words)

**Period**: Configurable (typically 1-10 seconds)

**Purpose**: Battery temperature control via PID algorithm

**Execution**:
```c
while(1) {
    // Wait for sensor data ready
    xEventGroupWaitBits(events, DATA_READY_BIT, ...);

    for (ch = 0; ch < 2; ch++) {
        // Read temperature
        T = GetBatteryTemperature(heater_sensor[ch]);

        // Calculate error
        error = SETPOINT - T;

        // PID algorithm
        P_term = Kp * error;
        I_term += Ki * error * dt;
        D_term = Kd * (error - error_prev[ch]) / dt;

        // Anti-windup for integral term
        if (I_term > I_MAX) I_term = I_MAX;
        if (I_term < I_MIN) I_term = I_MIN;

        // Calculate output
        output = P_term + I_term + D_term;

        // Convert to duty cycle
        duty_cycle[ch] = output;

        // Limit duty cycle
        if (duty_cycle[ch] > 100) duty_cycle[ch] = 100;
        if (duty_cycle[ch] < 0) duty_cycle[ch] = 0;

        // Update PWM
        SetHeaterPWM(ch, duty_cycle[ch]);

        // Store for next iteration
        error_prev[ch] = error;
    }

    // Update data buffer
    UpdateHeaterDutyCycles(duty_cycle);

    vTaskDelay(HEATER_CONTROL_PERIOD);
}
```

**Heater Channels**:
- Channel 0: Controlled by RTD Channel 6 temperature
- Channel 1: Controlled by RTD Channel 2 temperature

**PID Parameters**:
- **Kp** (Proportional gain): Immediate response to error
- **Ki** (Integral gain): Eliminates steady-state error
- **Kd** (Derivative gain): Dampens oscillations

**Temperature Setpoint**: Configurable (typically 0-10°C for Li-ion batteries)

**PWM Control**:
- Timer: Timer A1
- Frequency: Configurable (typically 1-10 Hz)
- Duty cycle range: 0-100%

**Safety Features**:
- Over-temperature cutoff
- Integral windup prevention
- Heater timeout (maximum on-time)

---

### 9. Device Response Task

**Location**: `/home/user/eps2/firmware/app/tasks/device_response.c`

**Priority**: High

**Stack Size**: Medium-Large (512-1024 words)

**Period**: Event-driven (triggered by I2C interrupt)

**Purpose**: Handle commands from OBDH module via I2C

**Execution**:
```c
while(1) {
    // Wait for I2C command received event
    xEventGroupWaitBits(events, I2C_CMD_RECEIVED_BIT, ...);

    // Read command packet from I2C buffer
    ReadI2CPacket(&packet);

    // Validate CRC
    if (!ValidateCRC(packet)) {
        SendNACK();
        continue;
    }

    // Parse command
    switch (packet.command) {
        case CMD_READ_PARAM:
            value = ReadParameter(packet.param_id);
            SendResponse(value);
            break;

        case CMD_WRITE_PARAM:
            WriteParameter(packet.param_id, packet.value);
            SendACK();
            break;

        case CMD_RESET:
            PerformSystemReset();
            break;

        case CMD_STATUS:
            status = GetSystemStatus();
            SendResponse(status);
            break;

        default:
            SendNACK();
            break;
    }
}
```

**Trigger**: I2C slave interrupt (address match, data received)

**Protocol**: FloripaSat Protocol (FSP) with CRC8-CCITT

**Commands** (typical):
- Read parameter(s)
- Write configuration
- System reset
- Status query
- Enable/disable functions

**Response Time**: < 10 ms (typical)

**Error Handling**:
- CRC validation
- Invalid command: Send NACK
- Timeout protection

---

### 10. Time Control Task

**Location**: `/home/user/eps2/firmware/app/tasks/time_control.c`

**Priority**: Low

**Stack Size**: Small (128-256 words)

**Period**: 1 second

**Purpose**: System time management and tracking

**Execution**:
```c
while(1) {
    system_time++;
    UpdateDataBuffer(PARAM_SYSTEM_TIME, system_time);
    vTaskDelay(1000);  // 1 second
}
```

**Key Operations**:
1. Increment system time counter
2. Update data buffer with current time
3. Delay for 1 second
4. Repeat

**Time Representation**: Seconds since boot (or epoch)

**Usage**: Timestamping events, periodic operations

---

## Task Synchronization

### Event Groups

Used for inter-task communication and synchronization:

- **DATA_READY_BIT**: Sensor data updated
- **I2C_CMD_RECEIVED_BIT**: Command received from OBDH
- **MPPT_COMPLETE_BIT**: MPPT iteration done
- **HEATER_UPDATE_BIT**: Heater control updated

### Semaphores

Used for resource protection:
- Data buffer access
- I2C bus access
- Flash memory access

### Queues

Used for message passing (if applicable):
- Command queue
- Log message queue

---

## Task Priorities Summary

| Task              | Priority | Period      | Type         |
|-------------------|----------|-------------|--------------|
| Startup           | Highest  | Once        | Initialization|
| Watchdog Reset    | High     | 100-500 ms  | Periodic     |
| Device Response   | High     | Event       | Event-driven |
| Read Sensors      | Medium-High | 100-1000 ms | Periodic  |
| MPPT Algorithm    | Medium   | 100-500 ms  | Periodic     |
| Heater Controller | Medium   | 1-10 s      | Periodic     |
| Parameter Server  | Medium   | Event       | Event-driven |
| System Reset      | Low      | 10 hours    | Periodic     |
| Heartbeat         | Low      | 500-1000 ms | Periodic     |
| Time Control      | Low      | 1 s         | Periodic     |

---

## Stack Allocation

Each task has dedicated stack space (in 16-bit words for MSP430):
- Small tasks: 128-256 words (256-512 bytes)
- Medium tasks: 256-512 words (512-1024 bytes)
- Large tasks: 512-1024 words (1024-2048 bytes)

Total stack allocation must fit within 64 KB RAM.

---

## Task Timing Diagram

```
Time ->
 0ms     100ms    200ms    300ms    400ms    500ms
  |        |        |        |        |        |
Watchdog  Watchdog Watchdog Watchdog Watchdog Watchdog
  |        |        |        |        |        |
Heartbeat          |                 Heartbeat
  |        |        |        |        |        |
Read      Read     Read     Read     Read     Read
Sensors   Sensors  Sensors  Sensors  Sensors  Sensors
  |        |        |        |        |        |
MPPT      MPPT     MPPT     MPPT     MPPT     MPPT
  |        |        |        |        |        |
(Event-driven tasks run asynchronously)
```
