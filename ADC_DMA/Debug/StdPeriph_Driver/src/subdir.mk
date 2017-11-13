################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../StdPeriph_Driver/src/misc.c \
../StdPeriph_Driver/src/stm32f4xx_adc.c \
../StdPeriph_Driver/src/stm32f4xx_dbgmcu.c \
../StdPeriph_Driver/src/stm32f4xx_dcmi.c \
../StdPeriph_Driver/src/stm32f4xx_dfsdm.c \
../StdPeriph_Driver/src/stm32f4xx_dma.c \
../StdPeriph_Driver/src/stm32f4xx_dma2d.c \
../StdPeriph_Driver/src/stm32f4xx_flash.c \
../StdPeriph_Driver/src/stm32f4xx_flash_ramfunc.c \
../StdPeriph_Driver/src/stm32f4xx_gpio.c \
../StdPeriph_Driver/src/stm32f4xx_lptim.c \
../StdPeriph_Driver/src/stm32f4xx_ltdc.c \
../StdPeriph_Driver/src/stm32f4xx_pwr.c \
../StdPeriph_Driver/src/stm32f4xx_rcc.c \
../StdPeriph_Driver/src/stm32f4xx_rtc.c \
../StdPeriph_Driver/src/stm32f4xx_spi.c \
../StdPeriph_Driver/src/stm32f4xx_syscfg.c \
../StdPeriph_Driver/src/stm32f4xx_tim.c \
../StdPeriph_Driver/src/stm32f4xx_usart.c 

OBJS += \
./StdPeriph_Driver/src/misc.o \
./StdPeriph_Driver/src/stm32f4xx_adc.o \
./StdPeriph_Driver/src/stm32f4xx_dbgmcu.o \
./StdPeriph_Driver/src/stm32f4xx_dcmi.o \
./StdPeriph_Driver/src/stm32f4xx_dfsdm.o \
./StdPeriph_Driver/src/stm32f4xx_dma.o \
./StdPeriph_Driver/src/stm32f4xx_dma2d.o \
./StdPeriph_Driver/src/stm32f4xx_flash.o \
./StdPeriph_Driver/src/stm32f4xx_flash_ramfunc.o \
./StdPeriph_Driver/src/stm32f4xx_gpio.o \
./StdPeriph_Driver/src/stm32f4xx_lptim.o \
./StdPeriph_Driver/src/stm32f4xx_ltdc.o \
./StdPeriph_Driver/src/stm32f4xx_pwr.o \
./StdPeriph_Driver/src/stm32f4xx_rcc.o \
./StdPeriph_Driver/src/stm32f4xx_rtc.o \
./StdPeriph_Driver/src/stm32f4xx_spi.o \
./StdPeriph_Driver/src/stm32f4xx_syscfg.o \
./StdPeriph_Driver/src/stm32f4xx_tim.o \
./StdPeriph_Driver/src/stm32f4xx_usart.o 

C_DEPS += \
./StdPeriph_Driver/src/misc.d \
./StdPeriph_Driver/src/stm32f4xx_adc.d \
./StdPeriph_Driver/src/stm32f4xx_dbgmcu.d \
./StdPeriph_Driver/src/stm32f4xx_dcmi.d \
./StdPeriph_Driver/src/stm32f4xx_dfsdm.d \
./StdPeriph_Driver/src/stm32f4xx_dma.d \
./StdPeriph_Driver/src/stm32f4xx_dma2d.d \
./StdPeriph_Driver/src/stm32f4xx_flash.d \
./StdPeriph_Driver/src/stm32f4xx_flash_ramfunc.d \
./StdPeriph_Driver/src/stm32f4xx_gpio.d \
./StdPeriph_Driver/src/stm32f4xx_lptim.d \
./StdPeriph_Driver/src/stm32f4xx_ltdc.d \
./StdPeriph_Driver/src/stm32f4xx_pwr.d \
./StdPeriph_Driver/src/stm32f4xx_rcc.d \
./StdPeriph_Driver/src/stm32f4xx_rtc.d \
./StdPeriph_Driver/src/stm32f4xx_spi.d \
./StdPeriph_Driver/src/stm32f4xx_syscfg.d \
./StdPeriph_Driver/src/stm32f4xx_tim.d \
./StdPeriph_Driver/src/stm32f4xx_usart.d 


# Each subdirectory must supply rules for building sources it contributes
StdPeriph_Driver/src/%.o: ../StdPeriph_Driver/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F4 -DSTM32 -DNUCLEO_F411RE -DSTM32F411RETx -DDEBUG -DUSE_STDPERIPH_DRIVER -DSTM32F411xE -I"C:/Users/Brian/eclipse-workspace/ADC_DMA/inc" -I"C:/Users/Brian/eclipse-workspace/ADC_DMA/CMSIS/core" -I"C:/Users/Brian/eclipse-workspace/ADC_DMA/CMSIS/device" -I"C:/Users/Brian/eclipse-workspace/ADC_DMA/StdPeriph_Driver/inc" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


