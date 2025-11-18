# EPS2 Driver Implementations

## Overview

This document provides detailed implementation information for all drivers in the EPS2 firmware. Each driver wraps the MSP430 HAL layer to provide a simplified, consistent API for device-level code.

**Driver Location**: `/home/user/eps2/firmware/drivers/`

---

## 1. GPIO Driver

**Location**: `/home/user/eps2/firmware/drivers/gpio/`

**Files**:
- `gpio.h` - Header with API and types
- `gpio.c` - Implementation

### API Functions

```c
int gpio_init(gpio_pin_t pin, gpio_config_t config);
int gpio_set_state(gpio_pin_t pin, bool state);
int gpio_get_state(gpio_pin_t pin);
int gpio_toggle(gpio_pin_t pin);
```

### Configuration Structure

```c
typedef struct {
    gpio_mode_t mode;  // GPIO_MODE_OUTPUT or GPIO_MODE_INPUT
} gpio_config_t;
```

### Pin Definitions

**Pin Range**: GPIO_PIN_0 to GPIO_PIN_69 (70 total GPIO pins)

**Pin Mapping**: Logical pins map to MSP430 physical ports (P1-P9, PJ)

### Example Usage

```c
// Initialize LED pin as output
gpio_config_t led_cfg = { .mode = GPIO_MODE_OUTPUT };
gpio_init(GPIO_PIN_36, led_cfg);

// Turn on LED
gpio_set_state(GPIO_PIN_36, true);

// Toggle LED
gpio_toggle(GPIO_PIN_36);
```

### Dependencies
- MSP430 HAL GPIO module
- `driverlib.h`

### Porting Notes
- Update pin-to-port mapping table for different MCU
- Verify GPIO port availability
- Check pin function multiplexing

---

## 2. I2C Driver

**Location**: `/home/user/eps2/firmware/drivers/i2c/`

**Files**:
- `i2c.h` - Header with API
- `i2c.c` - Implementation

### API Functions

```c
int i2c_init(i2c_port_t port, i2c_config_t config);
int i2c_write(i2c_port_t port, i2c_slave_adr_t adr, uint8_t *data, uint16_t len);
int i2c_read(i2c_port_t port, i2c_slave_adr_t adr, uint8_t *data, uint16_t len);
```

### Configuration Structure

```c
typedef struct {
    uint32_t speed_hz;  // Bus speed in Hz (typically 100000 or 400000)
} i2c_config_t;
```

### Ports Available

- **I2C_PORT_0**: USCI_B0
  - SDA: P2.1
  - SCL: P2.2
  - Usage: Battery Monitor (DS2777G)

- **I2C_PORT_1**: USCI_B1 (if enabled)
  - SDA: P8.5
  - SCL: P8.6

- **I2C_PORT_2**: USCI_B2
  - SDA: P9.5
  - SCL: P9.6
  - Usage: OBDH Communication (I2C Slave mode)

### Constants

```c
#define I2C_TIMEOUT     10000  // Transaction timeout in cycles
```

### Example Usage

```c
// Initialize I2C port 0 at 100 kHz
i2c_config_t i2c_cfg = { .speed_hz = 100000 };
i2c_init(I2C_PORT_0, i2c_cfg);

// Write data to slave device
uint8_t data[] = {0x10, 0x20, 0x30};
i2c_write(I2C_PORT_0, 0x34, data, 3);

// Read data from slave device
uint8_t buffer[4];
i2c_read(I2C_PORT_0, 0x34, buffer, 4);
```

### Dependencies
- MSP430 HAL USCI_B_I2C module
- HAL GPIO (for pin configuration)
- HAL UCS (for clock configuration)

### Porting Notes
- Update USCI module assignments
- Verify I2C peripheral availability
- Check pin multiplexing configuration
- Adjust timeout values based on clock speed

---

## 3. SPI Driver

**Location**: `/home/user/eps2/firmware/drivers/spi/`

**Files**:
- `spi.h` - Header with API
- `spi.c` - Implementation

### API Functions

```c
int spi_init(spi_port_t port, spi_config_t config);
int spi_select_slave(spi_port_t port, spi_cs_t cs, bool active);
int spi_write(spi_port_t port, spi_cs_t cs, uint8_t *data, uint16_t len);
int spi_read(spi_port_t port, spi_cs_t cs, uint8_t *data, uint16_t len);
int spi_transfer(spi_port_t port, spi_cs_t cs, uint8_t *wd, uint8_t *rd, uint16_t len);
int spi_transfer_no_cs(spi_port_t port, uint8_t *wd, uint8_t *rd, uint16_t len);
```

### Configuration Structure

```c
typedef struct {
    uint32_t speed_hz;  // SPI clock speed in Hz
    spi_mode_t mode;    // SPI mode (0-3)
} spi_config_t;
```

### SPI Modes

```c
typedef enum {
    SPI_MODE_0 = 0,  // CPOL=0, CPHA=0
    SPI_MODE_1 = 1,  // CPOL=0, CPHA=1
    SPI_MODE_2 = 2,  // CPOL=1, CPHA=0
    SPI_MODE_3 = 3,  // CPOL=1, CPHA=1
} spi_mode_t;
```

### Ports and Chip Selects

**Ports**: SPI_PORT_0 to SPI_PORT_5

**Chip Selects**: SPI_CS_0 to SPI_CS_9, SPI_CS_NONE

**Primary Usage**:
- SPI_PORT_1 with SPI_CS_0: ADS1248 temperature ADC

### Example Usage

```c
// Initialize SPI at 100 kHz, mode 1
spi_config_t spi_cfg = {
    .speed_hz = 100000,
    .mode = SPI_MODE_1
};
spi_init(SPI_PORT_1, spi_cfg);

// Transfer data with chip select
uint8_t tx_data[] = {0x01, 0x02, 0x03};
uint8_t rx_data[3];
spi_transfer(SPI_PORT_1, SPI_CS_0, tx_data, rx_data, 3);
```

### Dependencies
- MSP430 HAL USCI/EUSCI SPI modules
- GPIO driver (for chip select control)

### Porting Notes
- Update USCI module mappings
- Verify SPI peripheral count
- Adjust clock divider calculations
- Update CS pin assignments

---

## 4. UART Driver

**Location**: `/home/user/eps2/firmware/drivers/uart/`

**Files**:
- `uart.h` - Header with API
- `uart.c` - Implementation

### API Functions

```c
int uart_init(uart_port_t port, uart_config_t config);
int uart_available(uart_port_t port);
int uart_flush(uart_port_t port);
int uart_write(uart_port_t port, uint8_t *data, uint16_t len);
int uart_read(uart_port_t port, uint8_t *data, uint16_t len);
```

### Configuration Structure

```c
typedef struct {
    uint32_t clock;           // Clock source frequency
    uint32_t baudrate;        // Baud rate (e.g., 9600, 115200)
    uint8_t data_bits;        // Data bits (7 or 8)
    uart_parity_t parity;     // Parity setting
    uart_stop_bits_t stop_bits; // Stop bits (1 or 2)
} uart_config_t;
```

### Parity Options

```c
typedef enum {
    UART_NO_PARITY = 0,
    UART_ODD_PARITY = 1,
    UART_EVEN_PARITY = 2
} uart_parity_t;
```

### Stop Bits Options

```c
typedef enum {
    UART_ONE_STOP_BIT = 0,
    UART_TWO_STOP_BITS = 1
} uart_stop_bits_t;
```

### Ports Available

- **UART_PORT_0**: USCI_A0
  - Usage: TTC Beacon Communication (9600 baud)

- **UART_PORT_1**: USCI_A1

- **UART_PORT_2**: USCI_A2
  - Usage: Debug Console (115200 baud)

### Example Usage

```c
// Initialize debug UART at 115200 baud
uart_config_t uart_cfg = {
    .clock = 32000000,        // 32 MHz SMCLK
    .baudrate = 115200,
    .data_bits = 8,
    .parity = UART_NO_PARITY,
    .stop_bits = UART_ONE_STOP_BIT
};
uart_init(UART_PORT_2, uart_cfg);

// Write string
char msg[] = "Hello World\n";
uart_write(UART_PORT_2, (uint8_t*)msg, strlen(msg));

// Read data
uint8_t buffer[16];
int bytes_read = uart_read(UART_PORT_2, buffer, 16);
```

### Dependencies
- MSP430 HAL USCI_A_UART module

### Porting Notes
- Update USCI module assignments
- Recalculate baud rate divisors for different clock speeds
- Verify UART peripheral availability

---

## 5. ADC Driver

**Location**: `/home/user/eps2/firmware/drivers/adc/`

**Files**:
- `adc.h` - Header with API
- `adc.c` - Implementation

### API Functions

```c
int adc_init(adc_port_t port, adc_config_t config);
int adc_read(adc_port_t port, uint16_t *val);
void adc_delay_ms(uint16_t ms);
bool adc_mutex_create(void);
bool adc_mutex_take(void);
bool adc_mutex_give(void);
```

### Configuration Structure

```c
typedef struct {
    uint8_t sh_src;    // Sample-and-hold source
    uint8_t clk_src;   // Clock source
    uint8_t clk_div;   // Clock divider
} adc_config_t;
```

### Constants

```c
#define ADC_VREF_V      2.5    // Reference voltage in volts
#define ADC_MAX_VAL     4095   // 12-bit ADC (2^12 - 1)
#define ADC_TIMEOUT_MS  100    // Read timeout in milliseconds
```

### ADC Ports

**Voltage Sensors**:
- ADC_PORT_10: Main Power Bus
- ADC_PORT_12: Solar Panels -Y and +X
- ADC_PORT_13: Solar Panels -X and +Z
- ADC_PORT_14: Solar Panels -Z and +Y
- ADC_PORT_15: Total Solar Panels

**Current Sensors**:
- ADC_PORT_1: Solar Panel -Y Current
- ADC_PORT_2: Solar Panel +X Current
- ADC_PORT_3: Solar Panel -X Current
- ADC_PORT_4: Solar Panel +Z Current
- ADC_PORT_5: Solar Panel -Z Current
- ADC_PORT_6: Solar Panel +Y Current
- ADC_PORT_7: EPS + Beacon Current

**Temperature**:
- ADC_PORT_0: MCU Internal Temperature

### Example Usage

```c
// Initialize ADC port
adc_config_t adc_cfg = {
    .sh_src = ADC12_A_SAMPLEHOLDSOURCE_SC,
    .clk_src = ADC12_A_CLOCKSOURCE_SMCLK,
    .clk_div = ADC12_A_CLOCKDIVIDER_1
};
adc_init(ADC_PORT_10, adc_cfg);

// Read voltage (with mutex protection)
adc_mutex_take();
uint16_t raw_value;
adc_read(ADC_PORT_10, &raw_value);
adc_mutex_give();

// Convert to voltage
float voltage = (raw_value * ADC_VREF_V) / ADC_MAX_VAL;
```

### Dependencies
- MSP430 HAL ADC12_A module
- FreeRTOS (for mutex support)

### Porting Notes
- Update ADC channel mappings
- Adjust reference voltage configuration
- Verify ADC resolution (12-bit vs 10-bit)
- Update timeout handling for different clock speeds

---

## 6. PWM Driver

**Location**: `/home/user/eps2/firmware/drivers/pwm/`

**Files**:
- `pwm.h` - Header with API
- `pwm.c` - Implementation

### API Functions

```c
int pwm_init(pwm_source_t source, pwm_port_t port, pwm_config_t config);
int pwm_update(pwm_source_t source, pwm_port_t port, pwm_config_t config);
int pwm_stop(pwm_source_t source, pwm_port_t port, pwm_config_t config);
int pwm_disable(pwm_source_t source);
```

### Configuration Structure

```c
typedef struct {
    uint32_t period_us;      // PWM period in microseconds
    uint8_t duty_cycle;      // Duty cycle 0-100%
} pwm_config_t;
```

### PWM Sources (Timer Modules)

```c
typedef enum {
    TIMER_A0 = 0,
    TIMER_A1 = 1,
    TIMER_A2 = 2,
    TIMER_B0 = 3
} pwm_source_t;
```

### PWM Ports

- **PWM_PORT_0** to **PWM_PORT_6**: Output channels

**MPPT Usage**:
- TIMER_B0 with PWM_PORT_1, PWM_PORT_2, PWM_PORT_3

**Heater Usage**:
- TIMER_A1 with PWM_PORT_1, PWM_PORT_2

### Example Usage

```c
// Initialize MPPT PWM at 25 kHz, 50% duty cycle
pwm_config_t pwm_cfg = {
    .period_us = 40,        // 40 μs = 25 kHz
    .duty_cycle = 50        // 50%
};
pwm_init(TIMER_B0, PWM_PORT_1, pwm_cfg);

// Update duty cycle
pwm_cfg.duty_cycle = 75;
pwm_update(TIMER_B0, PWM_PORT_1, pwm_cfg);

// Stop PWM
pwm_stop(TIMER_B0, PWM_PORT_1, pwm_cfg);
```

### Dependencies
- MSP430 HAL Timer_A and Timer_B modules
- GPIO (for pin function configuration)

### Porting Notes
- Update timer module assignments
- Verify timer peripheral count
- Adjust period calculations based on timer clock
- Check PWM output pin availability

---

## 7. Watchdog Timer Driver

**Location**: `/home/user/eps2/firmware/drivers/wdt/`

**Files**:
- `wdt.h` - Header with API
- `wdt.c` - Implementation

### API Functions

```c
int wdt_init(wdt_config_t config);
void wdt_reset(void);
```

### Configuration Structure

```c
typedef struct {
    wdt_clk_src_t clk_src;   // Clock source
    wdt_clk_div_t clk_div;   // Clock divider
} wdt_config_t;
```

### Clock Sources

```c
typedef enum {
    WDT_CLK_SRC_SMCLK = 0,
    WDT_CLK_SRC_ACLK = 1,
    WDT_CLK_SRC_VLOCLK = 2,
    WDT_CLK_SRC_XCLK = 3
} wdt_clk_src_t;
```

### Clock Dividers

```c
typedef enum {
    WDT_CLK_DIV_2G = 0,    // Divide by 2^31
    WDT_CLK_DIV_128M = 1,  // Divide by 2^27
    WDT_CLK_DIV_8192K = 2, // Divide by 2^23
    WDT_CLK_DIV_512K = 3,  // Divide by 2^19
    WDT_CLK_DIV_32K = 4,   // Divide by 2^15
    WDT_CLK_DIV_8192 = 5,  // Divide by 2^13
    WDT_CLK_DIV_512 = 6,   // Divide by 2^9
    WDT_CLK_DIV_64 = 7     // Divide by 2^6
} wdt_clk_div_t;
```

### Example Usage

```c
// Initialize WDT with ACLK (32.768 kHz), divide by 32K
// Timeout = 32768 / 32768 = 1 second
wdt_config_t wdt_cfg = {
    .clk_src = WDT_CLK_SRC_ACLK,
    .clk_div = WDT_CLK_DIV_32K
};
wdt_init(wdt_cfg);

// Reset watchdog periodically
wdt_reset();
```

### Dependencies
- MSP430 HAL WDT_A module

### Porting Notes
- Verify watchdog peripheral availability
- Adjust timeout calculations
- Check reset behavior on different MCUs

---

## 8. Flash Memory Driver

**Location**: `/home/user/eps2/firmware/drivers/flash/`

**Files**:
- `flash.h` - Header with API
- `flash.c` - Implementation

### API Functions

```c
int flash_init();
void flash_write(uint8_t *data, uint16_t len);
void flash_write_single(uint8_t data, uint8_t *addr);
uint8_t flash_read_single(uint8_t *addr);
void flash_write_long(uint32_t data, uint32_t *addr);
uint32_t flash_read_long(uint32_t *addr);
void flash_erase(uint32_t *region);
```

### Memory Map

**Flash Banks** (128 KB each):
- Bank 0: 0x00008000
- Bank 1: 0x00028000
- Bank 2: 0x00048000
- Bank 3: 0x00068000

**Info Segments** (128 bytes each):
- Info A-D: Various information storage

**BSL Segments** (512 bytes each):
- BSL 0-3: Bootstrap loader segments

**Last Write Address**: 0x00087FFF

### Example Usage

```c
// Initialize flash
flash_init();

// Erase a segment
uint32_t *segment = (uint32_t*)0x00008000;
flash_erase(segment);

// Write single byte
uint8_t *addr = (uint8_t*)0x00008000;
flash_write_single(0xAB, addr);

// Read single byte
uint8_t value = flash_read_single(addr);

// Write 32-bit value
flash_write_long(0x12345678, (uint32_t*)addr);
```

### Dependencies
- MSP430 HAL FLASHCTL module

### Porting Notes
- Update memory map for different flash sizes
- Verify segment/page sizes
- Adjust erase and write timing
- Check voltage requirements for flash programming

---

## 9. OneWire Driver

**Location**: `/home/user/eps2/firmware/drivers/onewire/`

**Files**:
- `onewire.h` - Header with API
- `onewire.c` - Implementation

### API Functions

```c
int onewire_init(onewire_port_t port);
int onewire_reset(onewire_port_t port);
int onewire_write_bit(onewire_port_t port, int bit);
int onewire_write_byte(onewire_port_t port, uint8_t *data, uint16_t len);
int onewire_read_bit(onewire_port_t port);
int onewire_read_byte(onewire_port_t port, uint8_t *data, uint16_t len);
```

### Configuration

**Ports**: ONEWIRE_PORT_0 to ONEWIRE_PORT_69 (maps to GPIO pins)

**Clock**: 8 MHz (one_wire_clock constant)

### Protocol Timing

OneWire protocol requires precise timing:
- Reset pulse: 480 μs
- Presence detect: 60-240 μs
- Write 0: 60-120 μs low
- Write 1: 1-15 μs low
- Read slot: 1-15 μs low, sample at 15 μs

### Example Usage

```c
// Initialize OneWire on GPIO pin
onewire_init(ONEWIRE_PORT_10);

// Reset bus
int presence = onewire_reset(ONEWIRE_PORT_10);

// Write byte
uint8_t cmd = 0xCC;  // Skip ROM command
onewire_write_byte(ONEWIRE_PORT_10, &cmd, 1);

// Read byte
uint8_t data;
onewire_read_byte(ONEWIRE_PORT_10, &data, 1);
```

### Dependencies
- GPIO driver

### Porting Notes
- Adjust timing delays for different clock speeds
- Verify GPIO speed is sufficient for protocol
- Consider using timer-based implementation for accuracy

---

## 10. IC-Specific Drivers

### 10.1 ADS1248 (24-bit ADC)

**Location**: `/home/user/eps2/firmware/drivers/ads1248/`

### API Functions

```c
int ads1248_init(ads1248_config_t *config);
int ads1248_reset(ads1248_config_t *config, ads1248_reset_mode_t mode);
int ads1248_read_data(ads1248_config_t *config, uint8_t *rd, uint8_t positive_channel);
int ads1248_write_cmd(ads1248_config_t *config, ads1248_cmd_t cmd, uint8_t *rd, uint8_t positive_channel);
```

### Configuration Structure

```c
typedef struct {
    spi_port_t spi_port;     // SPI port to use
    spi_cs_t spi_cs;         // Chip select pin
    gpio_pin_t reset_pin;    // Hardware reset pin
    gpio_pin_t start_pin;    // Start conversion pin
} ads1248_config_t;
```

### Commands

```c
typedef enum {
    ADS1248_CMD_RESET = 0x06,
    ADS1248_CMD_START = 0x08,
    ADS1248_CMD_RDATA = 0x12,
    ADS1248_CMD_RDATAC = 0x14,
    ADS1248_CMD_SDATAC = 0x16,
    // ... additional commands
} ads1248_cmd_t;
```

### Resolution

**24-bit**: Range 0 to 16777216 (2^24)

### Example Usage

```c
// Configure ADS1248
ads1248_config_t ads_cfg = {
    .spi_port = SPI_PORT_1,
    .spi_cs = SPI_CS_0,
    .reset_pin = GPIO_PIN_58,
    .start_pin = GPIO_PIN_60
};

// Initialize
ads1248_init(&ads_cfg);

// Reset device
ads1248_reset(&ads_cfg, ADS1248_RESET_HARD);

// Read channel 0
uint8_t data[3];
ads1248_read_data(&ads_cfg, data, 0);

// Convert to 32-bit value
int32_t value = (data[0] << 16) | (data[1] << 8) | data[2];
```

---

### 10.2 DS2775G/DS2777G (Battery Monitor)

**Location**: `/home/user/eps2/firmware/drivers/ds2775g/`

### API Functions

```c
int ds2775g_init(ds2775g_config_t *config);
int ds2775g_write_data(onewire_port_t port, uint8_t *data, uint16_t len);
int ds2775g_read_register(onewire_port_t port, uint8_t register_address, uint8_t *data_read);
```

### Configuration

Uses OneWire protocol on specified GPIO port.

### Units and Conversions

```c
#define DS2775G_VOLTAGE_LSB     0.004883         // Volts per LSB
#define DS2775G_CURRENT_LSB(R)  (0.0000015625/R) // Amps per LSB
#define DS2775G_RSENSE          0.01             // Sense resistor (Ohms)
```

### Registers

- 0x0C-0x0D: Voltage
- 0x0E-0x0F: Current
- 0x10-0x11: Accumulated Current
- 0x0A-0x0B: Temperature
- Additional registers for RAAC, RSAC, RARC, RSRC, status

### Example Usage

```c
// Initialize on OneWire port
ds2775g_config_t bm_cfg = { .port = ONEWIRE_PORT_20 };
ds2775g_init(&bm_cfg);

// Read voltage register (0x0C)
uint8_t voltage_data[2];
ds2775g_read_register(ONEWIRE_PORT_20, 0x0C, voltage_data);

// Convert to voltage
uint16_t raw = (voltage_data[0] << 8) | voltage_data[1];
float voltage = raw * DS2775G_VOLTAGE_LSB;
```

---

### 10.3 TPS54X0 (DC-DC Converter)

**Location**: `/home/user/eps2/firmware/drivers/tps54x0/`

### API Functions

```c
int tps54x0_init(tps54x0_config_t config);
int tps54x0_enable(tps54x0_config_t config);
int tps54x0_disable(tps54x0_config_t config);
```

### Configuration Structure

```c
typedef struct {
    gpio_pin_t enable_pin;  // GPIO pin to control enable
} tps54x0_config_t;
```

### Example Usage

```c
// Configure DC-DC converter
tps54x0_config_t dcdc_cfg = { .enable_pin = GPIO_PIN_10 };

// Initialize
tps54x0_init(dcdc_cfg);

// Enable converter
tps54x0_enable(dcdc_cfg);

// Disable converter
tps54x0_disable(dcdc_cfg);
```

---

### 10.4 TCA4311A (I2C Buffer)

**Location**: `/home/user/eps2/firmware/drivers/tca4311a/`

### API Functions

```c
int tca4311a_init(tca4311a_config_t config, bool en);
int tca4311a_enable(tca4311a_config_t config);
int tca4311a_disable(tca4311a_config_t config);
int tca4311a_is_ready(tca4311a_config_t config);
int tca4311a_write(tca4311a_config_t config, i2c_slave_adr_t adr, uint8_t *data, uint16_t len);
int tca4311a_read(tca4311a_config_t config, i2c_slave_adr_t adr, uint8_t *data, uint16_t len);
```

### Configuration Structure

```c
typedef struct {
    i2c_port_t i2c_port;     // I2C port connected to buffer
    gpio_pin_t enable_pin;   // EN pin (GPIO_PIN_66)
    gpio_pin_t ready_pin;    // READY pin (GPIO_PIN_69)
} tca4311a_config_t;
```

### Example Usage

```c
// Configure I2C buffer
tca4311a_config_t buf_cfg = {
    .i2c_port = I2C_PORT_2,
    .enable_pin = GPIO_PIN_66,
    .ready_pin = GPIO_PIN_69
};

// Initialize and enable
tca4311a_init(buf_cfg, true);

// Check if ready
if (tca4311a_is_ready(buf_cfg)) {
    // Write through buffer
    uint8_t data[] = {0x01, 0x02};
    tca4311a_write(buf_cfg, 0x36, data, 2);
}
```

---

### 10.5 MAX9934 (Current Sense Amplifier)

**Location**: `/home/user/eps2/firmware/drivers/max9934/`

### API Functions

```c
int max9934_init(max9934_config_t config);
int max9934_read(max9934_config_t config, uint16_t *raw_val);
```

### Configuration Structure

```c
typedef struct {
    adc_port_t adc_port;     // ADC port for reading amplifier output
    adc_config_t adc_config; // ADC configuration
} max9934_config_t;
```

### Specifications

- **Gain**: 25 μA/mV (fixed)
- **Bandwidth**: 1 MHz
- **Input Offset**: < 50 μV

### Example Usage

```c
// Configure current sensor
max9934_config_t cs_cfg = {
    .adc_port = ADC_PORT_1,
    .adc_config = { /* ADC config */ }
};

// Initialize
max9934_init(cs_cfg);

// Read current
uint16_t raw_value;
max9934_read(cs_cfg, &raw_value);

// Convert to current (with Rsense = 20 mOhm)
// V_adc = raw * (VREF / 4095)
// I = V_adc / (Gain * Rsense)
```

---

### 10.6 I2C Slave Driver

**Location**: `/home/user/eps2/firmware/drivers/i2c_slave/`

### API Functions

```c
int i2c_slave_init(i2c_slave_port_t port, i2c_slave_address_t adr);
int i2c_slave_enable(void);
int i2c_slave_disable(void);
int i2c_slave_read(uint8_t *data, uint16_t *len);
int i2c_slave_write(uint8_t *data, uint16_t len);
```

### Configuration

**Buffer Sizes**: 16 bytes for TX and RX

**EPS2 I2C Address**: 0x36

**Port**: I2C_PORT_2 (USCI_B2 in slave mode)

### Example Usage

```c
// Initialize I2C slave at address 0x36
i2c_slave_init(I2C_SLAVE_PORT_2, 0x36);
i2c_slave_enable();

// Read incoming data
uint8_t rx_buffer[16];
uint16_t rx_len;
i2c_slave_read(rx_buffer, &rx_len);

// Write response
uint8_t tx_data[] = {0x01, 0x02, 0x03};
i2c_slave_write(tx_data, 3);
```

---

## Driver Integration Guidelines

### 1. Initialization Order

Recommended initialization sequence:
1. GPIO (required by many drivers)
2. Watchdog Timer
3. System clocks
4. Communication interfaces (I2C, SPI, UART)
5. ADC
6. PWM
7. IC-specific drivers (ADS1248, DS2777G, etc.)

### 2. Error Handling

All drivers return integer status codes:
- **0 or positive**: Success
- **Negative**: Error code

Common error codes should be defined in a central header.

### 3. Multi-Task Access

Drivers that may be accessed by multiple tasks should use:
- FreeRTOS mutexes (example: ADC driver)
- Critical sections
- Atomic operations where applicable

### 4. Resource Management

- Initialize peripherals only once
- Disable unused peripherals to save power
- Use interrupts instead of polling where possible

### 5. Testing

Each driver should have:
- Unit test (if possible in embedded environment)
- Integration test with hardware
- Boundary condition testing
- Error injection testing

---

## Summary

The EPS2 driver layer provides:
- **10 core drivers**: GPIO, I2C, SPI, UART, ADC, PWM, WDT, Flash, OneWire
- **6 IC-specific drivers**: ADS1248, DS2777G, TPS54X0, TCA4311A, MAX9934, I2C Slave
- Consistent API design across all drivers
- Hardware abstraction for portability
- FreeRTOS integration where needed

This modular design allows easy porting to different hardware platforms by updating the HAL layer while keeping driver APIs unchanged.
