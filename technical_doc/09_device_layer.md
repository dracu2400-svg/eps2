# EPS2 Device Layer Documentation

## Overview

The device layer provides high-level abstractions built on top of the driver layer. Each device module encapsulates specific hardware functionality and implements application-level logic such as algorithms, calibration, and state management.

**Device Location**: `/home/user/eps2/firmware/devices/`

---

## Device Architecture

```
┌──────────────────────────────────────────────────────┐
│              Application Tasks                        │
│  (MPPT Task, Heater Task, Read Sensors, etc.)        │
└───────────────────┬──────────────────────────────────┘
                    │
                    v
┌──────────────────────────────────────────────────────┐
│              Device Layer                             │
│  ┌────────┬────────┬────────┬────────┬────────────┐  │
│  │ LEDs   │ MPPT   │Battery │Voltage │  Current   │  │
│  │        │        │Monitor │Sensor  │  Sensor    │  │
│  ├────────┼────────┼────────┼────────┼────────────┤  │
│  │ Temp   │ Heater │  OBDH  │  TTC   │ Watchdog   │  │
│  │ Sensor │        │        │        │            │  │
│  └────────┴────────┴────────┴────────┴────────────┘  │
└───────────────────┬──────────────────────────────────┘
                    │
                    v
┌──────────────────────────────────────────────────────┐
│              Driver Layer                             │
│  (GPIO, I2C, SPI, UART, ADC, PWM, etc.)              │
└──────────────────────────────────────────────────────┘
```

---

## 1. LEDs Device

**Location**: `/home/user/eps2/firmware/devices/leds/`

### Purpose
Provides visual indicators for system status and faults.

### Hardware
- **System LED**: GPIO_PIN_36 (heartbeat indicator)
- **Fault LED**: GPIO_PIN_35 (error indicator)

### API Functions

```c
int leds_init();
int led_set(led_t l);
int led_clear(led_t l);
int led_toggle(led_t l);
```

### LED Types

```c
typedef enum {
    LED_SYSTEM = 0,  // Heartbeat LED
    LED_FAULT = 1    // Fault indicator LED
} led_t;
```

### Usage Example

```c
// Initialize LEDs
leds_init();

// Turn on system LED
led_set(LED_SYSTEM);

// Blink system LED (heartbeat)
while(1) {
    led_toggle(LED_SYSTEM);
    vTaskDelay(500);  // 500 ms
}

// Set fault LED on error
if (error_detected) {
    led_set(LED_FAULT);
}
```

### Dependencies
- GPIO driver

### Configuration
```c
#define LED_SYSTEM_PIN    GPIO_PIN_36
#define LED_FAULT_PIN     GPIO_PIN_35
```

---

## 2. Battery Monitor Device

**Location**: `/home/user/eps2/firmware/devices/battery_monitor/`

### Purpose
Interfaces with DS2777G battery fuel gauge IC to monitor battery status.

### Hardware
- **IC**: DS2777G (Maxim Integrated)
- **Interface**: OneWire protocol
- **Sense Resistor**: 10 mΩ (0.01 Ω)

### API Functions

```c
int battery_monitor_init();

// Voltage readings
int bm_get_cell_one_voltage(int16_t *voltage);
int bm_get_cell_two_voltage(int16_t *voltage);
int bm_get_voltage(uint16_t *voltage);

// Current readings
int bm_get_instantaneous_current(int16_t *current);
int bm_get_average_current(int16_t *current);

// Temperature
int bm_get_temperature_kelvin(uint16_t *temp);
int bm_get_temperature_celsius(int16_t *temp);

// Capacity
int bm_get_raac_mah(uint16_t *data);      // Remaining Active Absolute Capacity
int bm_get_rsac_mah(uint16_t *data);      // Remaining Standby Absolute Capacity
int bm_get_rarc_percent(uint8_t *data);   // Remaining Active Relative Capacity
int bm_get_rsrc_percent(uint8_t *data);   // Remaining Standby Relative Capacity

// Status
int bm_get_status(uint8_t *status);
int bm_get_protection(uint8_t *protection);
```

### DS2777G Registers

| Register | Address | Description |
|----------|---------|-------------|
| Voltage | 0x0C-0x0D | Battery voltage (10-bit) |
| Current | 0x0E-0x0F | Current (signed 15-bit) |
| Accumulated Current | 0x10-0x11 | Accumulated charge |
| Temperature | 0x0A-0x0B | Temperature (10-bit) |
| RAAC | 0x02-0x03 | Remaining Active Absolute Capacity |
| RSAC | 0x04-0x05 | Remaining Standby Absolute Capacity |
| RARC | 0x06 | Remaining Active Relative Capacity |
| RSRC | 0x07 | Remaining Standby Relative Capacity |
| Status | 0x01 | Status register |
| Protection | 0x00 | Protection register |

### Conversions

```c
// Voltage: 4.883 mV per LSB
voltage_mv = raw_value * 4.883;

// Current: 1.5625 μV / Rsense per LSB
current_ma = (raw_value * 0.0000015625 / 0.01) * 1000;

// Temperature: 0.125°C per LSB
temp_c = (raw_value * 0.125) - 273.15;  // Convert from Kelvin
```

### Usage Example

```c
// Initialize battery monitor
battery_monitor_init();

// Read battery voltage
uint16_t voltage;
bm_get_voltage(&voltage);
printf("Battery Voltage: %u mV\n", voltage);

// Read remaining capacity
uint8_t capacity_percent;
bm_get_rarc_percent(&capacity_percent);
printf("Battery Charge: %u%%\n", capacity_percent);

// Read temperature
int16_t temp;
bm_get_temperature_celsius(&temp);
printf("Battery Temperature: %d°C\n", temp);
```

### Dependencies
- DS2775G/DS2777G driver (OneWire)

---

## 3. MPPT Device

**Location**: `/home/user/eps2/firmware/devices/mppt/`

### Purpose
Implements Maximum Power Point Tracking algorithm to optimize solar panel power extraction.

### Hardware
- **PWM Source**: TIMER_B0
- **3 Channels**:
  - Channel 0 (PWM_PORT_1): Solar panels -Y and +X
  - Channel 1 (PWM_PORT_2): Solar panels -X and +Z
  - Channel 2 (PWM_PORT_3): Solar panels -Z and +Y

### API Functions

```c
int mppt_init();
int mppt_algorithm(mppt_channel_t channel);
int mppt_set_duty_cycle(mppt_channel_t channel, uint32_t duty_cycle);
int mppt_get_duty_cycle(mppt_channel_t channel, uint32_t *duty_cycle);
```

### MPPT Channels

```c
typedef enum {
    MPPT_CHANNEL_0 = 0,  // -Y and +X panels
    MPPT_CHANNEL_1 = 1,  // -X and +Z panels
    MPPT_CHANNEL_2 = 2,  // -Z and +Y panels
} mppt_channel_t;
```

### Algorithm: Perturb and Observe (P&O)

**Principle**: Incrementally adjust duty cycle and observe power change.

```
Algorithm:
1. Measure voltage V and current I
2. Calculate power P = V × I
3. Compare P with previous P_prev
4. If P > P_prev:
     Continue in same direction
   Else:
     Reverse direction
5. Update duty cycle
6. Repeat
```

**Implementation**:
```c
int mppt_algorithm(mppt_channel_t channel) {
    // Read voltage and current
    uint16_t voltage = voltage_sensor_read(channel);
    uint16_t current = current_sensor_read(channel);

    // Calculate power
    uint32_t power = voltage * current;

    // P&O algorithm
    if (power > power_prev[channel]) {
        // Power increased, continue in same direction
        if (voltage > voltage_prev[channel]) {
            duty_cycle[channel] += MPPT_STEP;
        } else {
            duty_cycle[channel] -= MPPT_STEP;
        }
    } else {
        // Power decreased, reverse direction
        if (voltage > voltage_prev[channel]) {
            duty_cycle[channel] -= MPPT_STEP;
        } else {
            duty_cycle[channel] += MPPT_STEP;
        }
    }

    // Limit duty cycle
    if (duty_cycle[channel] > MPPT_MAX_DUTY) duty_cycle[channel] = MPPT_MAX_DUTY;
    if (duty_cycle[channel] < MPPT_MIN_DUTY) duty_cycle[channel] = MPPT_MIN_DUTY;

    // Update PWM
    mppt_set_duty_cycle(channel, duty_cycle[channel]);

    // Store for next iteration
    power_prev[channel] = power;
    voltage_prev[channel] = voltage;

    return 0;
}
```

### Configuration

```c
#define MPPT_STEP                 1      // Step size: 1%
#define MPPT_INITIAL_DUTY_CYCLE   50     // Initial: 50%
#define MPPT_MIN_DUTY             10     // Minimum: 10%
#define MPPT_MAX_DUTY             90     // Maximum: 90%
#define MPPT_PWM_PERIOD_US        4      // 250 kHz
```

### Usage Example

```c
// Initialize MPPT
mppt_init();

// Run MPPT algorithm periodically
while(1) {
    for (int ch = 0; ch < 3; ch++) {
        mppt_algorithm(ch);
    }
    vTaskDelay(100);  // 100 ms
}
```

### Dependencies
- PWM driver
- Voltage sensor device
- Current sensor device

---

## 4. Current Sensor Device

**Location**: `/home/user/eps2/firmware/devices/current_sensor/`

### Purpose
Reads and calibrates current measurements from MAX9934 current sense amplifiers.

### Hardware
- **Amplifier**: MAX9934 (gain: 25 μA/mV)
- **Sense Resistors**:
  - Solar panels: 20 mΩ
  - EPS system: 75 mΩ
- **ADC**: 12-bit internal ADC

### API Functions

```c
int current_sensor_init();
uint16_t current_sensor_raw_to_ma(adc_port_t port, uint16_t raw);
int current_sensor_read(adc_port_t port, uint16_t *cur);
```

### ADC Port Mapping

```c
typedef enum {
    PANNEL_MINUS_Y = ADC_PORT_1,
    PANNEL_PLUS_X = ADC_PORT_2,
    PANNEL_MINUS_X = ADC_PORT_3,
    PANNEL_PLUS_Z = ADC_PORT_4,
    PANNEL_MINUS_Z = ADC_PORT_5,
    PANNEL_PLUS_Y = ADC_PORT_6,
    EPS_BEACON = ADC_PORT_7
} current_sensor_port_t;
```

### Calibration

**Solar Panel Current**:
```c
Rsense = 20 mΩ = 0.020 Ω
RL = 3.3 kΩ
Gain = 25 μA/mV

V_sense = I × Rsense
V_out = V_sense × (RL / Rsense) × Gain
I = (V_adc / ADC_VREF) × ADC_MAX × (1 / Rsense) × (1 / Gain)
```

**EPS Current**:
```c
Rsense = 75 mΩ = 0.075 Ω
RL = 4.02 kΩ
Gain = 25 μA/mV

(Similar conversion formula)
```

### Usage Example

```c
// Initialize current sensors
current_sensor_init();

// Read solar panel -Y current
uint16_t current_ma;
current_sensor_read(PANNEL_MINUS_Y, &current_ma);
printf("Panel -Y Current: %u mA\n", current_ma);

// Read all solar currents
for (int i = PANNEL_MINUS_Y; i <= PANNEL_PLUS_Y; i++) {
    current_sensor_read(i, &current_ma);
    printf("Panel %d: %u mA\n", i, current_ma);
}
```

### Dependencies
- ADC driver
- MAX9934 driver

---

## 5. Voltage Sensor Device

**Location**: `/home/user/eps2/firmware/devices/voltage_sensor/`

### Purpose
Reads and calibrates voltage measurements from resistor divider networks.

### Hardware
- **Voltage Dividers**: Resistor networks
- **ADC**: 12-bit internal ADC
- **Reference**: 2.5V precision reference

### API Functions

```c
int voltage_sensor_init(void);
uint16_t voltage_sensor_raw_to_mv(adc_port_t port, uint16_t raw);
int voltage_sensor_read(adc_port_t port, uint16_t *volt);
```

### ADC Port Mapping

```c
typedef enum {
    PANNELS_MINUS_Y_PLUS_X = ADC_PORT_12,
    PANNELS_MINUS_X_PLUS_Z = ADC_PORT_13,
    PANNELS_MINUS_Z_PLUS_Y = ADC_PORT_14,
    TOTAL_SOLAR_PANNELS = ADC_PORT_15,
    MAIN_POWER_BUS = ADC_PORT_10
} voltage_sensor_port_t;
```

### Voltage Divider Ratios

```c
// Solar panel groups: 4.0x divider
#define SOLAR_DIVIDER_RATIO    4.0

// Main power bus: 1.93x divider
#define BUS_DIVIDER_RATIO      1.93
```

### Calibration

```c
// ADC value to actual voltage
V_actual = (ADC_raw / ADC_MAX) × ADC_VREF × Divider_Ratio

// For solar panels (4.0x divider)
V_actual = (raw / 4095) × 2.5 × 4.0

// For main bus (1.93x divider)
V_actual = (raw / 4095) × 2.5 × 1.93
```

### Usage Example

```c
// Initialize voltage sensors
voltage_sensor_init();

// Read solar panel voltage
uint16_t voltage_mv;
voltage_sensor_read(PANNELS_MINUS_Y_PLUS_X, &voltage_mv);
printf("Solar -Y+X Voltage: %u mV\n", voltage_mv);

// Read main bus voltage
voltage_sensor_read(MAIN_POWER_BUS, &voltage_mv);
printf("Main Bus: %u mV\n", voltage_mv);
```

### Dependencies
- ADC driver

---

## 6. Temperature Sensor Device

**Location**: `/home/user/eps2/firmware/devices/temp_sensor/`

### Purpose
Provides temperature measurements from multiple sources.

### Hardware
- **MCU Internal**: MSP430 internal temperature sensor
- **External RTD**: 7 channels via ADS1248 24-bit ADC
  - 4 battery module sensors (RTD 0-3)
  - 3 solar panel sensors (RTD 4-6)

### API Functions

```c
int temp_sensor_init(void);

// MCU temperature
int temp_mcu_read_c(int16_t *temp);
int temp_mcu_read_k(uint16_t *temp);

// RTD temperature
int temp_rtd_read_c(uint8_t channel, uint16_t *temp);
int temp_rtd_read_k(uint8_t channel, uint16_t *temp);
```

### RTD Channels

```c
typedef enum {
    RTD_BATTERY_0 = 0,
    RTD_BATTERY_1 = 1,
    RTD_BATTERY_2 = 2,
    RTD_BATTERY_3 = 3,
    RTD_SOLAR_Y = 4,
    RTD_SOLAR_X = 5,
    RTD_SOLAR_Z = 6
} rtd_channel_t;
```

### Temperature Conversion

**MCU Internal**:
- Uses MSP430 calibration constants
- Formula provided in datasheet

**RTD (Platinum PT100/PT1000)**:
- Resistance changes with temperature
- Linear approximation: R = R0 × (1 + α × T)
- R0 = 100Ω or 1000Ω at 0°C
- α = 0.00385 /°C (temperature coefficient)

### ADS1248 Configuration

```c
#define TEMP_SENSOR_SPI_PORT     SPI_PORT_1
#define TEMP_SENSOR_CS           SPI_CS_0
#define TEMP_SENSOR_RESET_PIN    GPIO_PIN_58
#define TEMP_SENSOR_START_PIN    GPIO_PIN_60
```

### Usage Example

```c
// Initialize temperature sensors
temp_sensor_init();

// Read MCU temperature
int16_t mcu_temp;
temp_mcu_read_c(&mcu_temp);
printf("MCU Temp: %d°C\n", mcu_temp);

// Read battery temperatures
for (int ch = 0; ch < 4; ch++) {
    uint16_t temp;
    temp_rtd_read_c(ch, &temp);
    printf("Battery %d: %u°C\n", ch, temp);
}
```

### Dependencies
- ADC driver (for MCU internal)
- ADS1248 driver (for RTD)
- SPI driver

---

## 7. Heater Device

**Location**: `/home/user/eps2/firmware/devices/heater/`

### Purpose
Controls battery heaters using PID algorithm to maintain optimal temperature.

### Hardware
- **Heater Elements**: 2 resistive heaters
- **PWM Control**: Timer A1
- **Channels**:
  - Heater 0: Controlled by RTD 6, PWM_PORT_1
  - Heater 1: Controlled by RTD 2, PWM_PORT_2

### API Functions

```c
int heater_init(heater_channel_t channel);
float heater_algorithm(float setpoint, float measurement);
int heater_get_sensor(heater_channel_t channel, temperature_t *temp);
int heater_set_actuator(heater_channel_t channel, float pid_output);
```

### Heater Channels

```c
typedef enum {
    HEATER_CHANNEL_0 = 0,  // RTD 6, Battery area 1
    HEATER_CHANNEL_1 = 1   // RTD 2, Battery area 2
} heater_channel_t;
```

### PID Algorithm

**PID Equation**:
```
output(t) = Kp × e(t) + Ki × ∫e(t)dt + Kd × de(t)/dt

Where:
e(t) = setpoint - measurement (error)
Kp = Proportional gain
Ki = Integral gain
Kd = Derivative gain
```

**Implementation**:
```c
float heater_algorithm(float setpoint, float measurement) {
    // Calculate error
    float error = setpoint - measurement;

    // Proportional term
    float p_term = Kp * error;

    // Integral term
    integral += Ki * error * dt;

    // Anti-windup
    if (integral > I_MAX) integral = I_MAX;
    if (integral < I_MIN) integral = I_MIN;

    // Derivative term
    float d_term = Kd * (error - error_prev) / dt;
    error_prev = error;

    // PID output
    float output = p_term + integral + d_term;

    // Limit output to 0-100%
    if (output > 100.0) output = 100.0;
    if (output < 0.0) output = 0.0;

    return output;
}
```

### PID Tuning Parameters

```c
#define HEATER_KP           2.0      // Proportional gain
#define HEATER_KI           0.1      // Integral gain
#define HEATER_KD           0.5      // Derivative gain
#define HEATER_I_MAX        50.0     // Max integral windup
#define HEATER_I_MIN        -50.0    // Min integral windup
#define HEATER_SETPOINT     10.0     // Target: 10°C
#define HEATER_DT           1.0      // Sample time: 1 second
```

### Usage Example

```c
// Initialize heaters
heater_init(HEATER_CHANNEL_0);
heater_init(HEATER_CHANNEL_1);

// Control loop (runs periodically)
while(1) {
    // Read temperature
    temperature_t temp;
    heater_get_sensor(HEATER_CHANNEL_0, &temp);

    // Run PID algorithm
    float duty_cycle = heater_algorithm(HEATER_SETPOINT, temp.celsius);

    // Set heater output
    heater_set_actuator(HEATER_CHANNEL_0, duty_cycle);

    // Wait for next iteration
    vTaskDelay(1000);  // 1 second
}
```

### Safety Features

- **Over-temperature cutoff**: Disable heater if temp > 50°C
- **Integral windup prevention**: Limit integral term
- **Maximum duty cycle**: Limit to 90% to prevent overheating
- **Timeout**: Disable if sensor read fails

### Dependencies
- PWM driver
- Temperature sensor device

---

## 8. OBDH Device

**Location**: `/home/user/eps2/firmware/devices/obdh/`

### Purpose
Handles communication with OBDH (On-Board Data Handling) module via I2C slave interface.

### Hardware
- **Interface**: I2C slave (address 0x36)
- **Port**: I2C_PORT_2 (USCI_B2)
- **Buffer**: TCA4311A I2C buffer
- **Protocol**: FloripaSat Protocol (FSP) with CRC8

### API Functions

```c
int obdh_init(void);
int obdh_decode(uint8_t *adr, uint32_t *val, uint8_t *cmd);
int obdh_write_output_buffer(uint8_t adr, uint32_t val);
uint8_t obdh_crc8(uint8_t *data, uint8_t len);
bool obdh_check_crc(uint8_t *data, uint8_t len, uint8_t crc);
```

### FSP Packet Format

**Command (Read)**:
```
Byte 0: Parameter address
Byte 1: CRC8
```

**Command (Write)**:
```
Byte 0: Parameter address
Byte 1-4: Value (32-bit)
Byte 5: CRC8
```

**Response**:
```
Byte 0: Parameter address
Byte 1-4: Value (32-bit)
Byte 5: CRC8
```

### CRC8-CCITT

**Polynomial**: 0x07
**Initial value**: 0x00

```c
uint8_t obdh_crc8(uint8_t *data, uint8_t len) {
    uint8_t crc = 0x00;
    for (uint8_t i = 0; i < len; i++) {
        crc ^= data[i];
        for (uint8_t j = 0; j < 8; j++) {
            if (crc & 0x80) {
                crc = (crc << 1) ^ 0x07;
            } else {
                crc <<= 1;
            }
        }
    }
    return crc;
}
```

### Usage Example

```c
// Initialize OBDH interface
obdh_init();

// In I2C slave interrupt handler or task
uint8_t rx_buffer[16];
uint16_t rx_len;
i2c_slave_read(rx_buffer, &rx_len);

// Decode command
uint8_t param_addr;
uint32_t param_value;
uint8_t cmd_type;
obdh_decode(&param_addr, &param_value, &cmd_type);

// Process command
if (cmd_type == CMD_READ) {
    // Read parameter from data buffer
    uint32_t value = data_buffer[param_addr];
    obdh_write_output_buffer(param_addr, value);
} else if (cmd_type == CMD_WRITE) {
    // Write parameter to data buffer
    data_buffer[param_addr] = param_value;
}
```

### Dependencies
- I2C slave driver
- TCA4311A driver
- Parameter server (data buffer access)

---

## 9. TTC Device

**Location**: `/home/user/eps2/firmware/devices/ttc/`

### Purpose
Handles communication with TTC (Telemetry, Tracking, Command) beacon module via UART.

### Hardware
- **Interface**: UART (9600 baud)
- **Port**: UART_PORT_0 (USCI_A0)

### API Functions

```c
int ttc_init(void);
int ttc_decode(uint8_t *adr, uint32_t *val, uint8_t *cmd);
int ttc_answer(uint8_t adr, uint32_t val);
int ttc_answer_long(uint8_t *buf, uint8_t len);
uint8_t ttc_crc8(uint8_t *data, uint8_t len);
bool ttc_check_crc(uint8_t *data, uint8_t len, uint8_t crc);
```

### Protocol
Similar to OBDH (FSP with CRC8), but over UART instead of I2C.

### Usage Example

```c
// Initialize TTC interface
ttc_init();

// Receive command
uint8_t rx_data[16];
uart_read(UART_PORT_0, rx_data, 16);

// Decode
uint8_t param_addr;
uint32_t param_value;
uint8_t cmd_type;
ttc_decode(&param_addr, &param_value, &cmd_type);

// Send response
ttc_answer(param_addr, data_buffer[param_addr]);
```

### Dependencies
- UART driver

---

## 10. Watchdog Device

**Location**: `/home/user/eps2/firmware/devices/watchdog/`

### Purpose
Wraps watchdog timer driver for device-level access.

### API Functions

```c
int watchdog_init();
void watchdog_reset();
```

### Usage Example

```c
// Initialize watchdog
watchdog_init();

// In watchdog reset task
while(1) {
    watchdog_reset();
    vTaskDelay(100);  // 100 ms
}
```

### Dependencies
- WDT driver

---

## Device Integration

### Initialization Sequence

Recommended order (in Startup Task):
1. System Logger
2. LEDs
3. Watchdog
4. Voltage Sensors
5. Current Sensors
6. Temperature Sensors
7. Battery Monitor
8. MPPT
9. Heater
10. OBDH
11. TTC

### Inter-Device Communication

Devices communicate via:
- **Data Buffer**: Centralized parameter storage
- **FreeRTOS Events**: Signal data ready or command received
- **Direct Function Calls**: Device functions call other device functions

### Error Handling

All device functions return status codes:
- **0**: Success
- **Negative**: Error code (to be defined)

### Configuration

Enable/disable devices in `config.h`:
```c
#define CONFIG_DEV_LEDS_ENABLED                 1
#define CONFIG_DEV_MPPT_ENABLED                 1
#define CONFIG_DEV_BATTERY_MONITOR_ENABLED      1
#define CONFIG_DEV_CURRENT_SENSOR_ENABLED       1
#define CONFIG_DEV_VOLTAGE_SENSOR_ENABLED       1
#define CONFIG_DEV_TEMP_SENSOR_ENABLED          1
#define CONFIG_DEV_HEATER_ENABLED               1
#define CONFIG_DEV_OBDH_ENABLED                 1
#define CONFIG_DEV_TTC_ENABLED                  1
#define CONFIG_DEV_WATCHDOG_ENABLED             1
```

---

## Summary

The device layer abstracts hardware functionality into logical units:
- **10 device modules**: LEDs, Battery Monitor, MPPT, Current Sensor, Voltage Sensor, Temperature Sensor, Heater, OBDH, TTC, Watchdog
- **High-level APIs**: Simplify application development
- **Algorithm implementation**: MPPT P&O, Heater PID
- **Communication protocols**: FSP with CRC8
- **Modular design**: Easy to enable/disable features

This layer is mostly platform-independent and requires minimal changes during porting.
