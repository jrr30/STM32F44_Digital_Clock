################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ThirdParty/SEGGER/OS/SEGGER_SYSVIEW_FreeRTOS.c 

C_DEPS += \
./ThirdParty/SEGGER/OS/SEGGER_SYSVIEW_FreeRTOS.d 

OBJS += \
./ThirdParty/SEGGER/OS/SEGGER_SYSVIEW_FreeRTOS.o 


# Each subdirectory must supply rules for building sources it contributes
ThirdParty/SEGGER/OS/SEGGER_SYSVIEW_FreeRTOS.o: ../ThirdParty/SEGGER/OS/SEGGER_SYSVIEW_FreeRTOS.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F446xx -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Jesus/STM32CubeIDE/Clock/ThirdParty/FreeRTOS/include" -I"C:/Users/Jesus/STM32CubeIDE/Clock/ThirdParty/FreeRTOS/portable/GCC/ARM_CM4F" -I"C:/Users/Jesus/STM32CubeIDE/Clock/ThirdParty/FreeRTOS" -I"C:/Users/Jesus/STM32CubeIDE/Clock/ThirdParty/SEGGER/Config" -I"C:/Users/Jesus/STM32CubeIDE/Clock/ThirdParty/SEGGER/OS" -I"C:/Users/Jesus/STM32CubeIDE/Clock/ThirdParty/SEGGER/SEGGER" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"ThirdParty/SEGGER/OS/SEGGER_SYSVIEW_FreeRTOS.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

