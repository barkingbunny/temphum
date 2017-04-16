################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/adc.c \
../Src/gpio.c \
../Src/i2c.c \
../Src/main.c \
../Src/rtc.c \
../Src/spi.c \
../Src/stm32l0xx_hal_msp.c \
../Src/stm32l0xx_it.c \
../Src/system_stm32l0xx.c \
../Src/tim.c \
../Src/usb_device.c \
../Src/usbd_cdc_if.c \
../Src/usbd_conf.c \
../Src/usbd_desc.c 

OBJS += \
./Src/adc.o \
./Src/gpio.o \
./Src/i2c.o \
./Src/main.o \
./Src/rtc.o \
./Src/spi.o \
./Src/stm32l0xx_hal_msp.o \
./Src/stm32l0xx_it.o \
./Src/system_stm32l0xx.o \
./Src/tim.o \
./Src/usb_device.o \
./Src/usbd_cdc_if.o \
./Src/usbd_conf.o \
./Src/usbd_desc.o 

C_DEPS += \
./Src/adc.d \
./Src/gpio.d \
./Src/i2c.d \
./Src/main.d \
./Src/rtc.d \
./Src/spi.d \
./Src/stm32l0xx_hal_msp.d \
./Src/stm32l0xx_it.d \
./Src/system_stm32l0xx.d \
./Src/tim.d \
./Src/usb_device.d \
./Src/usbd_cdc_if.d \
./Src/usbd_conf.d \
./Src/usbd_desc.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -mfloat-abi=soft '-D__weak=__attribute__((weak))' '-D__packed="__attribute__((__packed__))"' -DUSE_HAL_DRIVER -DSTM32L072xx -I"/home/jakub/workspace/eclipse/Termostat/TempHum_display/Inc" -I"/home/jakub/workspace/eclipse/Termostat/TempHum_display/Drivers/STM32L0xx_HAL_Driver/Inc" -I"/home/jakub/workspace/eclipse/Termostat/TempHum_display/Drivers/STM32L0xx_HAL_Driver/Inc/Legacy" -I"/home/jakub/workspace/eclipse/Termostat/TempHum_display/Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"/home/jakub/workspace/eclipse/Termostat/TempHum_display/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I"/home/jakub/workspace/eclipse/Termostat/TempHum_display/Drivers/CMSIS/Device/ST/STM32L0xx/Include" -I"/home/jakub/workspace/eclipse/Termostat/TempHum_display/Drivers/CMSIS/Include" -I"/home/jakub/workspace/eclipse/Termostat/TempHum_display/Inc/module"  -Os -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


