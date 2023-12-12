################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Ffunctions/f_debug.c 

OBJS += \
./Core/Src/Ffunctions/f_debug.o 

C_DEPS += \
./Core/Src/Ffunctions/f_debug.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Ffunctions/%.o Core/Src/Ffunctions/%.su Core/Src/Ffunctions/%.cyclo: ../Core/Src/Ffunctions/%.c Core/Src/Ffunctions/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F745xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../USB_HOST/App -I../USB_HOST/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../Middlewares/ST/STM32_USB_Host_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Host_Library/Class/CDC/Inc -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Ffunctions

clean-Core-2f-Src-2f-Ffunctions:
	-$(RM) ./Core/Src/Ffunctions/f_debug.cyclo ./Core/Src/Ffunctions/f_debug.d ./Core/Src/Ffunctions/f_debug.o ./Core/Src/Ffunctions/f_debug.su

.PHONY: clean-Core-2f-Src-2f-Ffunctions

