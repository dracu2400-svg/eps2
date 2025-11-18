######################################
# target
######################################
TARGET = eps2-stm32l4

######################################
# building variables
######################################
# debug build?
DEBUG = 1
# optimization
OPT = -Og

#######################################
# paths
#######################################
# Build path
BUILD_DIR = build

######################################
# source
######################################
# C sources
C_SOURCES =  \
system_stm32l4xx.c \
main.c \
system/clocks.c \
system/sys_log/sys_log.c \
system/sys_log/sys_log_uart.c \
system/sys_log/sys_log_mutex.c \
system/system.c \
system/hooks.c \
system/setup.c \
drivers/gpio/gpio.c \
drivers/ds2775g/ds2775g.c \
drivers/max9934/max9934.c \
drivers/pwm/pwm.c \
drivers/wdt/wdt.c \
drivers/i2c_slave/i2c_slave.c \
drivers/ds277Xg/ds277Xg.c \
drivers/tca4311a/tca4311a.c \
drivers/flash/flash.c \
drivers/ads1248/ads1248_delay.c \
drivers/ads1248/ads1248.c \
drivers/onewire/onewire.c \
drivers/uart/uart.c \
drivers/spi/spi.c \
drivers/adc/adc_delay.c \
drivers/adc/adc_mutex.c \
drivers/adc/adc.c \
drivers/uart_interrupt/uart_interrupt.c \
drivers/i2c/i2c.c \
drivers/tps54x0/tps54x0.c \
devices/bat_manager/bat_manager.c \
devices/ttc/ttc.c \
devices/current_sensor/current_sensor.c \
devices/mppt/mppt.c \
devices/heater/heater.c \
devices/heater/heater_on_off.c \
devices/watchdog/watchdog.c \
devices/media/media.c \
devices/power_conv/power_conv.c \
devices/obdh/obdh.c \
devices/leds/leds.c \
devices/battery_monitor/battery_monitor.c \
devices/temp_sensor/temp_sensor.c \
devices/voltage_sensor/voltage_sensor.c \
app/structs/eps2_data.c \
app/tasks/read_sensors.c \
app/tasks/heater_controller.c \
app/tasks/tasks.c \
app/tasks/system_reset.c \
app/tasks/heartbeat.c \
app/tasks/time_control.c \
app/tasks/mppt_algorithm.c \
app/tasks/startup.c \
app/tasks/param_server.c \
app/tasks/watchdog_reset.c \
app/tasks/device_response.c \
app/libs/fsp/fsp/fsp.c \
app/libs/fsp/fsp/crc16.c \
app/libs/libcsp-1.5.16/src/crypto/csp_hmac.c \
app/libs/libcsp-1.5.16/src/crypto/csp_sha1.c \
app/libs/libcsp-1.5.16/src/crypto/csp_xtea.c \
app/libs/libcsp-1.5.16/src/csp_promisc.c \
app/libs/libcsp-1.5.16/src/csp_bridge.c \
app/libs/libcsp-1.5.16/src/csp_service_handler.c \
app/libs/libcsp-1.5.16/src/transport/csp_udp.c \
app/libs/libcsp-1.5.16/src/transport/csp_rdp.c \
app/libs/libcsp-1.5.16/src/csp_conn.c \
app/libs/libcsp-1.5.16/src/interfaces/csp_if_lo.c \
app/libs/libcsp-1.5.16/src/interfaces/csp_if_can_pbuf.c \
app/libs/libcsp-1.5.16/src/interfaces/csp_if_i2c.c \
app/libs/libcsp-1.5.16/src/interfaces/csp_if_kiss.c \
app/libs/libcsp-1.5.16/src/interfaces/csp_if_spi.c \
app/libs/libcsp-1.5.16/src/interfaces/csp_if_can.c \
app/libs/libcsp-1.5.16/src/csp_port.c \
app/libs/libcsp-1.5.16/src/csp_dedup.c \
app/libs/libcsp-1.5.16/src/csp_route.c \
app/libs/libcsp-1.5.16/src/csp_iflist.c \
app/libs/libcsp-1.5.16/src/csp_crc32.c \
app/libs/libcsp-1.5.16/src/csp_hex_dump.c \
app/libs/libcsp-1.5.16/src/arch/freertos/csp_semaphore.c \
app/libs/libcsp-1.5.16/src/arch/freertos/csp_malloc.c \
app/libs/libcsp-1.5.16/src/arch/freertos/csp_queue.c \
app/libs/libcsp-1.5.16/src/arch/freertos/csp_thread.c \
app/libs/libcsp-1.5.16/src/arch/freertos/csp_system.c \
app/libs/libcsp-1.5.16/src/arch/freertos/csp_time.c \
app/libs/libcsp-1.5.16/src/csp_io.c \
app/libs/libcsp-1.5.16/src/csp_services.c \
app/libs/libcsp-1.5.16/src/rtable/csp_rtable_static.c \
app/libs/libcsp-1.5.16/src/csp_sfp.c \
app/libs/libcsp-1.5.16/src/csp_debug.c \
app/libs/libcsp-1.5.16/src/csp_qfifo.c \
app/libs/libcsp-1.5.16/src/csp_endian.c \
app/libs/libcsp-1.5.16/src/csp_buffer.c \
freertos/croutine.c \
freertos/event_groups.c \
freertos/list.c \
freertos/queue.c \
freertos/stream_buffer.c \
freertos/tasks.c \
freertos/timers.c \
freertos/portable/MemMang/heap_1.c \
freertos/portable/GCC/ARM_CM4F/port.c

# ASM sources
ASM_SOURCES =  \
startup_stm32l676xx.s

#######################################
# binaries
#######################################
PREFIX = arm-none-eabi-
# The gcc compiler bin path can be either defined in make command via GCC_PATH variable (> make GCC_PATH=xxx)
# either it can be added to the PATH environment variable.
ifdef GCC_PATH
CC = $(GCC_PATH)/$(PREFIX)gcc
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
else
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
endif
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S

#######################################
# CFLAGS
#######################################
# cpu
CPU = -mcpu=cortex-m4

# fpu
FPU = -mfpu=fpv4-sp-d16

# float-abi
FLOAT-ABI = -mfloat-abi=hard

# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# macros for gcc
# AS defines
AS_DEFS =

# C defines
C_DEFS =  \
-DSTM32L476xx \
-DUSE_HAL_DRIVER \
-DARM_MATH_CM4 \
-D__FPU_PRESENT=1 \
-D__FPU_USED=1

# AS includes
AS_INCLUDES =

# C includes
C_INCLUDES =  \
-I. \
-Iconfig \
-Isystem \
-Isystem/sys_log \
-Idrivers \
-Idrivers/gpio \
-Idrivers/uart \
-Idrivers/uart_interrupt \
-Idrivers/spi \
-Idrivers/i2c \
-Idrivers/i2c_slave \
-Idrivers/adc \
-Idrivers/pwm \
-Idrivers/wdt \
-Idrivers/flash \
-Idrivers/onewire \
-Idrivers/ads1248 \
-Idrivers/ds2775g \
-Idrivers/ds277Xg \
-Idrivers/max9934 \
-Idrivers/tca4311a \
-Idrivers/tps54x0 \
-Idevices \
-Idevices/bat_manager \
-Idevices/battery_monitor \
-Idevices/current_sensor \
-Idevices/heater \
-Idevices/leds \
-Idevices/media \
-Idevices/mppt \
-Idevices/obdh \
-Idevices/power_conv \
-Idevices/temp_sensor \
-Idevices/ttc \
-Idevices/voltage_sensor \
-Idevices/watchdog \
-Iapp \
-Iapp/tasks \
-Iapp/structs \
-Iapp/libs/fsp/fsp \
-Iapp/libs/libcsp-1.5.16/include \
-Ifreertos/include \
-Ifreertos/portable/GCC/ARM_CM4F

# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif

# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"

#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = STM32L676RGTx_FLASH.ld

# libraries
LIBS = -lc -lm -lnosys
LIBDIR =
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin

#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR)
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@

$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@

$(BUILD_DIR):
	mkdir $@

#######################################
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR)

#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)

# *** EOF ***
