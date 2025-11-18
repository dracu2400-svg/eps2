# EPS2 Firmware Porting Guide

## Overview

This guide provides comprehensive instructions for porting the EPS2 firmware to different microcontroller platforms or adapting it for derivative projects. The firmware's layered architecture facilitates porting by isolating hardware-specific code in the HAL and driver layers.

---

## Firmware Architecture Overview

```
┌─────────────────────────────────────────────┐
│         Application Layer                   │  ← Platform-independent
│  (Tasks, Data Structures, Algorithms)       │
├─────────────────────────────────────────────┤
│         Devices Layer                       │  ← Mostly platform-independent
│  (LEDs, MPPT, Battery, Sensors, etc.)       │
├─────────────────────────────────────────────┤
│         FreeRTOS Layer                      │  ← Platform-specific config
│  (Task Scheduler, Kernel)                   │
├─────────────────────────────────────────────┤
│         Drivers Layer                       │  ← Platform-specific
│  (GPIO, I2C, SPI, UART, ADC, PWM, etc.)     │
├─────────────────────────────────────────────┤
│    Hardware Abstraction Layer (HAL)         │  ← Platform-specific
│  (MCU peripheral library)                   │
└─────────────────────────────────────────────┘
```

**Porting Effort by Layer**:
- **Application Layer**: No changes needed (0%)
- **Devices Layer**: Minimal changes (5-10%)
- **FreeRTOS Layer**: Configuration updates (10-20%)
- **Drivers Layer**: Significant updates (60-70%)
- **HAL Layer**: Complete replacement (100%)

---

## Prerequisites

Before starting the porting process:

1. **Target Platform Selection**
   - Choose compatible MCU (ARM Cortex-M, RISC-V, another MSP430, etc.)
   - Verify peripheral availability: I2C, SPI, UART, ADC, PWM, timers
   - Check memory requirements: 512KB Flash minimum, 64KB RAM minimum
   - Ensure FreeRTOS support for target platform

2. **Development Environment**
   - IDE/toolchain for target MCU
   - FreeRTOS source code for target architecture
   - MCU vendor's HAL/peripheral library
   - Debugger/programmer for target hardware

3. **Hardware Considerations**
   - Pin mapping compatibility
   - Voltage levels (3.3V vs 5V)
   - Clock sources and frequencies
   - Peripheral module counts

---

## Step-by-Step Porting Process

### Step 1: Setup Project Structure

**1.1 Create New Project**
```
eps2_port/
├── firmware/
│   ├── hal/               # New HAL for target MCU
│   ├── drivers/           # Copy from original, modify
│   ├── devices/           # Copy from original
│   ├── app/               # Copy from original
│   ├── freertos/          # FreeRTOS for target
│   ├── config/            # Copy and modify
│   └── main.c             # Modify for target
```

**1.2 Copy Existing Code**
```bash
# Copy application and device layers (minimal changes needed)
cp -r eps2/firmware/app eps2_port/firmware/
cp -r eps2/firmware/devices eps2_port/firmware/
cp -r eps2/firmware/config eps2_port/firmware/
```

---

### Step 2: Replace HAL Layer

**2.1 Obtain Target MCU HAL**

For different platforms:
- **STM32**: Use STM32CubeIDE HAL
- **Nordic nRF**: Use nRF SDK
- **ESP32**: Use ESP-IDF
- **TI ARM**: Use TivaWare or MSP432 DriverLib
- **Microchip**: Use Harmony or MPLAB X libraries

**2.2 Organize HAL Files**
```
firmware/hal/
├── hal_gpio.h
├── hal_gpio.c
├── hal_i2c.h
├── hal_i2c.c
├── hal_spi.h
├── hal_spi.c
├── hal_uart.h
├── hal_uart.c
├── hal_adc.h
├── hal_adc.c
├── hal_timer.h
├── hal_timer.c
└── ...
```

**2.3 Create HAL Wrapper (Optional)**

To minimize driver changes, create a wrapper that maps EPS2 HAL functions to target HAL:

```c
// hal_wrapper.h
#ifndef HAL_WRAPPER_H
#define HAL_WRAPPER_H

#include "target_mcu_hal.h"  // Include target HAL

// Map MSP430 functions to target functions
#define GPIO_setOutputHighOnPin(port, pin)  target_gpio_set(port, pin)
#define GPIO_setOutputLowOnPin(port, pin)   target_gpio_clear(port, pin)
// ... more mappings

#endif
```

---

### Step 3: Port Driver Layer

#### 3.1 GPIO Driver

**File**: `firmware/drivers/gpio/gpio.c`

**Porting Tasks**:
1. Update pin-to-port mapping table
2. Replace HAL calls with target HAL
3. Verify GPIO configuration options (input/output, pull-up/down)

**Example for STM32**:
```c
// Original MSP430 code
int gpio_set_state(gpio_pin_t pin, bool state) {
    if (state) {
        GPIO_setOutputHighOnPin(port_map[pin].port, port_map[pin].pin);
    } else {
        GPIO_setOutputLowOnPin(port_map[pin].port, port_map[pin].pin);
    }
    return 0;
}

// Ported to STM32
int gpio_set_state(gpio_pin_t pin, bool state) {
    GPIO_PinState pin_state = state ? GPIO_PIN_SET : GPIO_PIN_RESET;
    HAL_GPIO_WritePin(port_map[pin].port, port_map[pin].pin, pin_state);
    return 0;
}
```

**Pin Mapping Table Update**:
```c
// Original MSP430
const gpio_map_t port_map[GPIO_PIN_COUNT] = {
    { GPIO_PORT_P1, GPIO_PIN0 },  // GPIO_PIN_0
    { GPIO_PORT_P1, GPIO_PIN1 },  // GPIO_PIN_1
    // ...
};

// For STM32
const gpio_map_t port_map[GPIO_PIN_COUNT] = {
    { GPIOA, GPIO_PIN_0 },  // GPIO_PIN_0
    { GPIOA, GPIO_PIN_1 },  // GPIO_PIN_1
    // ...
};
```

---

#### 3.2 I2C Driver

**File**: `firmware/drivers/i2c/i2c.c`

**Porting Tasks**:
1. Map I2C peripheral instances (I2C0, I2C1, I2C2 → I2C1, I2C2, etc.)
2. Update initialization with target HAL
3. Implement read/write functions using target HAL
4. Adjust timeout handling

**Example for STM32**:
```c
// Original MSP430
int i2c_write(i2c_port_t port, i2c_slave_adr_t adr, uint8_t *data, uint16_t len) {
    USCI_B_I2C_setSlaveAddress(port, adr);
    USCI_B_I2C_masterSendMultiByteStart(port, data[0]);
    // ... continue transmission
    return 0;
}

// Ported to STM32
int i2c_write(i2c_port_t port, i2c_slave_adr_t adr, uint8_t *data, uint16_t len) {
    I2C_HandleTypeDef *hi2c = get_i2c_handle(port);
    HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(hi2c, adr << 1, data, len, I2C_TIMEOUT);
    return (status == HAL_OK) ? 0 : -1;
}
```

---

#### 3.3 SPI Driver

**File**: `firmware/drivers/spi/spi.c`

**Porting Tasks**:
1. Map SPI peripheral instances
2. Configure SPI mode (CPOL, CPHA)
3. Implement chip select control
4. Update transfer functions

**Example for STM32**:
```c
// Original MSP430
int spi_transfer(spi_port_t port, spi_cs_t cs, uint8_t *wd, uint8_t *rd, uint16_t len) {
    spi_select_slave(port, cs, true);
    for (uint16_t i = 0; i < len; i++) {
        USCI_A_SPI_transmitData(port, wd[i]);
        rd[i] = USCI_A_SPI_receiveData(port);
    }
    spi_select_slave(port, cs, false);
    return 0;
}

// Ported to STM32
int spi_transfer(spi_port_t port, spi_cs_t cs, uint8_t *wd, uint8_t *rd, uint16_t len) {
    SPI_HandleTypeDef *hspi = get_spi_handle(port);
    spi_select_slave(port, cs, true);
    HAL_StatusTypeDef status = HAL_SPI_TransmitReceive(hspi, wd, rd, len, SPI_TIMEOUT);
    spi_select_slave(port, cs, false);
    return (status == HAL_OK) ? 0 : -1;
}
```

---

#### 3.4 UART Driver

**File**: `firmware/drivers/uart/uart.c`

**Porting Tasks**:
1. Map UART peripheral instances
2. Configure baud rate calculation
3. Implement read/write with interrupts or DMA
4. Update buffer management

**Baud Rate Calculation**:
```c
// Original MSP430 (calculated by HAL)
USCI_A_UART_init(baseAddress, config);

// STM32 (set in CubeMX or manually)
huart->Init.BaudRate = config->baudrate;
HAL_UART_Init(huart);
```

---

#### 3.5 ADC Driver

**File**: `firmware/drivers/adc/adc.c`

**Porting Tasks**:
1. Map ADC channels
2. Configure ADC resolution (12-bit vs 10-bit)
3. Set reference voltage
4. Implement conversion trigger and read

**Example for STM32**:
```c
// Original MSP430
int adc_read(adc_port_t port, uint16_t *val) {
    ADC12_A_setupSamplingTimer(...);
    ADC12_A_startConversion(...);
    while(ADC12_A_isBusy(...));
    *val = ADC12_A_getResults(...);
    return 0;
}

// Ported to STM32
int adc_read(adc_port_t port, uint16_t *val) {
    ADC_HandleTypeDef *hadc = get_adc_handle(port);
    HAL_ADC_Start(hadc);
    HAL_ADC_PollForConversion(hadc, ADC_TIMEOUT);
    *val = HAL_ADC_GetValue(hadc);
    HAL_ADC_Stop(hadc);
    return 0;
}
```

**ADC Calibration**:
- Update `ADC_VREF_V` constant in `adc.h` to match target reference voltage
- Adjust resolution if different from 12-bit

---

#### 3.6 PWM Driver

**File**: `firmware/drivers/pwm/pwm.c`

**Porting Tasks**:
1. Map timer modules to target timers
2. Configure timer clock source
3. Calculate period and duty cycle values
4. Set up PWM output channels

**Period and Duty Cycle Calculation**:
```c
// Original MSP430
uint16_t period = (timer_clock * period_us) / 1000000;
uint16_t compare = (period * duty_cycle) / 100;

// STM32 (similar but may need prescaler)
uint16_t prescaler = (HAL_RCC_GetPCLK1Freq() / 1000000) - 1;  // 1 MHz
uint16_t period = period_us - 1;
uint16_t compare = (period * duty_cycle) / 100;
```

---

#### 3.7 Watchdog Timer Driver

**File**: `firmware/drivers/wdt/wdt.c`

**Porting Tasks**:
1. Configure watchdog peripheral
2. Set timeout period
3. Implement reset function

**Example for STM32**:
```c
// Original MSP430
void wdt_reset(void) {
    WDT_A_resetTimer(WDT_A_BASE);
}

// Ported to STM32
void wdt_reset(void) {
    HAL_IWDG_Refresh(&hiwdg);
}
```

---

#### 3.8 Flash Driver

**File**: `firmware/drivers/flash/flash.c`

**Porting Tasks**:
1. Update memory map for target flash
2. Implement erase function (page/sector based)
3. Implement write function (word/byte based)
4. Handle flash unlock/lock

**Important**: Flash programming differs significantly between MCUs
- MSP430: Byte-level writes
- STM32: Word-aligned writes, page erase
- ESP32: Sector erase, requires alignment

---

### Step 4: Port Device Layer (Minor Changes)

Most device layer files require minimal or no changes. Focus on:

#### 4.1 Update Constants

**File**: `firmware/devices/current_sensor/current_sensor.c`

Update calibration values if hardware changes:
```c
// Solar panel current sensor calibration
#define SOLAR_RSENSE_MOHM    20
#define SOLAR_RL_KOHM        3.3
#define GAIN_UV_PER_MV       25
```

#### 4.2 Update Pin Assignments

**File**: `firmware/devices/leds/leds.h`

```c
// Original
#define LED_SYSTEM_PIN    GPIO_PIN_36
#define LED_FAULT_PIN     GPIO_PIN_35

// Update for new hardware
#define LED_SYSTEM_PIN    GPIO_PIN_10
#define LED_FAULT_PIN     GPIO_PIN_11
```

#### 4.3 MPPT and Heater Configuration

**File**: `firmware/devices/mppt/mppt.c`

Update PWM configuration if timer assignments change:
```c
// Original
#define MPPT_TIMER_SOURCE    TIMER_B0

// Updated
#define MPPT_TIMER_SOURCE    TIM2  // STM32 timer
```

---

### Step 5: Port FreeRTOS Configuration

#### 5.1 FreeRTOS Source

Download FreeRTOS for target architecture:
- ARM Cortex-M: Use ARM_CM4F or ARM_CM3 port
- RISC-V: Use RISC-V port
- Xtensa (ESP32): Use Xtensa port

**Location**: `firmware/freertos/Source/portable/<architecture>/`

#### 5.2 Update FreeRTOSConfig.h

**File**: `firmware/config/FreeRTOSConfig.h`

Key settings to update:
```c
// CPU clock frequency
#define configCPU_CLOCK_HZ              ( 72000000UL )  // Update for target

// Tick rate (1 ms tick is common)
#define configTICK_RATE_HZ              ( 1000 )

// Heap size (adjust based on available RAM)
#define configTOTAL_HEAP_SIZE           ( 32768 )

// Number of priorities (adjust as needed)
#define configMAX_PRIORITIES            ( 5 )

// Minimal stack size (depends on architecture)
#define configMINIMAL_STACK_SIZE        ( 128 )  // Words for ARM, bytes for others
```

#### 5.3 System Tick Configuration

Ensure system tick is configured correctly for FreeRTOS:
```c
// For ARM Cortex-M (done in SystemInit)
void SystemInit(void) {
    // Configure SysTick for 1ms tick
    SysTick_Config(SystemCoreClock / 1000);
}
```

---

### Step 6: Update Application Layer

#### 6.1 Main Entry Point

**File**: `firmware/main.c`

Update hardware initialization:
```c
int main(void) {
    // Original MSP430
    WDTCTL = WDTPW | WDTHOLD;  // Stop watchdog
    PMM_setVCore(PMM_CORE_LEVEL_3);  // Set core voltage
    system_clocks_init();  // Initialize clocks

    // For STM32
    HAL_Init();  // Initialize HAL
    SystemClock_Config();  // Configure system clock

    // Common (no change needed)
    tasks_init();  // Create FreeRTOS tasks
    vTaskStartScheduler();  // Start scheduler

    while(1);  // Should never reach here
}
```

#### 6.2 Clock Configuration

Create target-specific clock configuration:
```c
// For STM32
void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    // Configure PLL to achieve target frequency (e.g., 72 MHz)
    // ... HAL clock configuration code
}
```

#### 6.3 Startup Task

**File**: `firmware/app/tasks/startup.c`

May need minor updates for hardware version detection or peripheral initialization order.

---

### Step 7: Update Configuration File

**File**: `firmware/config/config.h`

#### 7.1 Enable/Disable Features

Keep same structure:
```c
// Task enables
#define CONFIG_TASK_STARTUP_ENABLED                 1
#define CONFIG_TASK_WATCHDOG_RESET_ENABLED          1
// ... etc

// Device enables
#define CONFIG_DEV_LEDS_ENABLED                     1
#define CONFIG_DEV_MPPT_ENABLED                     1
// ... etc
```

#### 7.2 Update Hardware Constants

```c
// Battery configuration
#define MAX_BATTERY_CHARGE                          2450  // mAh

// ADC reference voltage (UPDATE FOR TARGET)
#define ADC_VREF_V                                  3.0   // Volts

// Task periods (keep same or adjust)
#define CONFIG_TASK_READ_SENSORS_PERIOD_MS          60000  // 60 seconds
```

---

### Step 8: Build System Setup

#### 8.1 Makefile (if using make)

Create Makefile for target:
```makefile
# Compiler
CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy

# Target MCU flags
MCUFLAGS = -mcpu=cortex-m3 -mthumb

# Include paths
INCLUDES = -Ifirmware/hal \
           -Ifirmware/drivers \
           -Ifirmware/devices \
           -Ifirmware/freertos/Source/include \
           -Ifirmware/app \
           -Ifirmware/config

# Source files
SRC = firmware/main.c \
      firmware/drivers/gpio/gpio.c \
      firmware/drivers/i2c/i2c.c \
      # ... all source files

# Compile and link
$(PROJECT).elf: $(SRC)
	$(CC) $(MCUFLAGS) $(INCLUDES) $(SRC) -o $@
```

#### 8.2 IDE Project (if using IDE)

For STM32CubeIDE, Keil, IAR, etc.:
1. Create new project for target MCU
2. Add all source files to project
3. Configure include paths
4. Set compiler flags
5. Configure linker script

---

### Step 9: Testing and Validation

#### 9.1 Layer-by-Layer Testing

Test each layer independently:

**HAL Layer**:
- Test basic GPIO toggle
- Test I2C communication with test device
- Test UART echo test
- Verify ADC readings

**Driver Layer**:
- Test each driver API function
- Verify correct operation with logic analyzer
- Check timing requirements

**Device Layer**:
- Test each device module with actual hardware
- Verify sensor readings
- Test MPPT and heater algorithms

**Application Layer**:
- Test task creation and scheduling
- Verify inter-task communication
- Check system behavior under load

#### 9.2 Integration Testing

Full system tests:
- Power-on sequence
- Sensor data acquisition
- MPPT operation
- Heater control
- Communication with OBDH/TTC
- Long-duration stability test

#### 9.3 Debugging Tools

Use available debugging tools:
- JTAG/SWD debugger (GDB, Ozone, etc.)
- Logic analyzer for bus signals
- Oscilloscope for PWM and analog signals
- Serial console for log output

---

## Common Porting Challenges and Solutions

### Challenge 1: Different Register Access Patterns

**Problem**: MSP430 uses direct register access, ARM uses HAL structures.

**Solution**: Create abstraction layer or wrapper functions.

### Challenge 2: Clock Configuration Differences

**Problem**: Different PLLs, dividers, and clock trees.

**Solution**: Document target clock configuration thoroughly. Use vendor's clock configuration tools.

### Challenge 3: Interrupt Handling

**Problem**: Different interrupt vector tables and priorities.

**Solution**: Update interrupt handlers. Use RTOS-safe interrupt functions.

### Challenge 4: Memory Alignment

**Problem**: ARM requires word-aligned access for some operations.

**Solution**: Use `__attribute__((aligned(4)))` or compiler-specific attributes.

### Challenge 5: Stack Size

**Problem**: Different architectures have different stack requirements.

**Solution**: Increase task stack sizes, monitor stack usage with FreeRTOS utilities.

---

## Optimization Tips

1. **Use DMA for I2C/SPI/UART**: Offload data transfers from CPU
2. **Enable Compiler Optimization**: Use -O2 or -O3 for production
3. **Reduce Power Consumption**: Use MCU low-power modes when idle
4. **Profile Performance**: Identify bottlenecks using profiling tools
5. **Minimize Interrupt Latency**: Keep ISRs short

---

## Platform-Specific Notes

### Porting to STM32 (ARM Cortex-M)

- Use STM32CubeMX for peripheral initialization code generation
- HAL library provides comprehensive peripheral support
- CMSIS-RTOS wrapper can be used instead of FreeRTOS directly
- DMA support for most peripherals

### Porting to ESP32 (Xtensa)

- Use ESP-IDF framework
- FreeRTOS is built into ESP-IDF
- Dual-core architecture requires task affinity consideration
- Wi-Fi/Bluetooth stack integration if needed

### Porting to Nordic nRF (ARM Cortex-M)

- Use nRF SDK
- SoftDevice (BLE stack) integration if wireless needed
- Low power consumption optimization critical
- Use app_timer for application timing

### Porting to RISC-V

- Ensure FreeRTOS RISC-V port is used
- Check interrupt controller differences (PLIC, CLIC)
- Verify toolchain support (GCC RISC-V)

---

## Checklist

Use this checklist to track porting progress:

- [ ] Target MCU selected and development board obtained
- [ ] Development environment set up (IDE, toolchain, debugger)
- [ ] FreeRTOS source for target architecture obtained
- [ ] HAL layer replaced with target HAL
- [ ] GPIO driver ported and tested
- [ ] I2C driver ported and tested
- [ ] SPI driver ported and tested
- [ ] UART driver ported and tested
- [ ] ADC driver ported and tested
- [ ] PWM driver ported and tested
- [ ] Watchdog driver ported and tested
- [ ] Flash driver ported and tested
- [ ] All device modules updated with new pin assignments
- [ ] FreeRTOSConfig.h updated for target
- [ ] main.c updated with target initialization
- [ ] Clock configuration implemented
- [ ] Project builds without errors
- [ ] Basic functionality tested (LED blink, UART output)
- [ ] All drivers tested individually
- [ ] All devices tested individually
- [ ] Integration test passed
- [ ] System stability verified

---

## Additional Resources

**FreeRTOS Documentation**: https://www.freertos.org/
**MSP430 DriverLib**: TI MSP430Ware documentation
**STM32 HAL**: STM32CubeIDE documentation
**ESP-IDF**: Espressif ESP-IDF programming guide
**Nordic nRF SDK**: Nordic Semiconductor documentation

---

## Conclusion

Porting the EPS2 firmware to a new platform is a systematic process that primarily involves replacing the HAL and updating drivers. The modular architecture ensures that higher-level code (devices and application) remains largely unchanged. Following this guide step-by-step will minimize porting effort and reduce the likelihood of errors.

For questions or assistance, consult the original EPS2 documentation and the target MCU vendor's resources.
