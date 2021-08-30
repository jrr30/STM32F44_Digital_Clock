################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ThirdParty/SEGGER/Rec/segger_uart.c 

C_DEPS += \
./ThirdParty/SEGGER/Rec/segger_uart.d 

OBJS += \
./ThirdParty/SEGGER/Rec/segger_uart.o 


# Each subdirectory must supply rules for building sources it contributes
ThirdParty/SEGGER/Rec/segger_uart.o: ../ThirdParty/SEGGER/Rec/segger_uart.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F446xx -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Jesus/STM32CubeIDE/Clock/ThirdParty/FreeRTOS/include" -I"C:/Users/Jesus/STM32CubeIDE/Clock/ThirdParty/FreeRTOS/portable/GCC/ARM_CM4F" -I"C:/Users/Jesus/STM32CubeIDE/Clock/ThirdParty/FreeRTOS" -I"C:/Users/Jesus/STM32CubeIDE/Clock/ThirdParty/SEGGER/Config" -I"C:/Users/Jesus/STM32CubeIDE/Clock/ThirdParty/SEGGER/OS" -I"C:/Users/Jesus/STM32CubeIDE/Clock/ThirdParty/SEGGER/SEGGER" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"ThirdParty/SEGGER/Rec/segger_uart.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

