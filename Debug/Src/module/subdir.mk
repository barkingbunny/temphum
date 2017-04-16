################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/module/BME280.c \
../Src/module/Time.c \
../Src/module/ds18b20.c \
../Src/module/lcd_12864.c \
../Src/module/one_wire.c \
../Src/module/peripherals.c \
../Src/module/pinmap.c \
../Src/module/rtc_api.c \
../Src/module/spi_com.c \
../Src/module/waiter.c 

OBJS += \
./Src/module/BME280.o \
./Src/module/Time.o \
./Src/module/ds18b20.o \
./Src/module/lcd_12864.o \
./Src/module/one_wire.o \
./Src/module/peripherals.o \
./Src/module/pinmap.o \
./Src/module/rtc_api.o \
./Src/module/spi_com.o \
./Src/module/waiter.o 

C_DEPS += \
./Src/module/BME280.d \
./Src/module/Time.d \
./Src/module/ds18b20.d \
./Src/module/lcd_12864.d \
./Src/module/one_wire.d \
./Src/module/peripherals.d \
./Src/module/pinmap.d \
./Src/module/rtc_api.d \
./Src/module/spi_com.d \
./Src/module/waiter.d 


# Each subdirectory must supply rules for building sources it contributes
Src/module/%.o: ../Src/module/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -mfloat-abi=soft '-D__weak=__attribute__((weak))' '-D__packed="__attribute__((__packed__))"' -DUSE_HAL_DRIVER -DSTM32L072xx -I"/home/jakub/workspace/eclipse/Termostat/TempHum_display/Inc" -I"/home/jakub/workspace/eclipse/Termostat/TempHum_display/Drivers/STM32L0xx_HAL_Driver/Inc" -I"/home/jakub/workspace/eclipse/Termostat/TempHum_display/Drivers/STM32L0xx_HAL_Driver/Inc/Legacy" -I"/home/jakub/workspace/eclipse/Termostat/TempHum_display/Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"/home/jakub/workspace/eclipse/Termostat/TempHum_display/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I"/home/jakub/workspace/eclipse/Termostat/TempHum_display/Drivers/CMSIS/Device/ST/STM32L0xx/Include" -I"/home/jakub/workspace/eclipse/Termostat/TempHum_display/Drivers/CMSIS/Include" -I"/home/jakub/workspace/eclipse/Termostat/TempHum_display/Inc/module"  -Os -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


