################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../startup/startup_stm32f411xe.s 

OBJS += \
./startup/startup_stm32f411xe.o 


# Each subdirectory must supply rules for building sources it contributes
startup/%.o: ../startup/%.s
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Assembler'
	@echo $(PWD)
	arm-none-eabi-as -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -I"C:/Users/Brian/eclipse-workspace/HCM_LED_MATRIX/inc" -I"C:/Users/Brian/eclipse-workspace/HCM_LED_MATRIX/CMSIS/core" -I"C:/Users/Brian/eclipse-workspace/HCM_LED_MATRIX/CMSIS/device" -I"C:/Users/Brian/eclipse-workspace/HCM_LED_MATRIX/StdPeriph_Driver/inc" -g -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


