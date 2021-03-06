################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/coco_adc.cpp \
../src/coco_bmp280.cpp \
../src/coco_gpio.cpp \
../src/coco_mpu6050.cpp \
../src/coco_pwm.cpp \
../src/coco_qmc5883l.cpp \
../src/coco_spi.cpp \
../src/coco_timer.cpp \
../src/coco_uart.cpp \
../src/coco_wire.cpp 

OBJS += \
./src/coco_adc.o \
./src/coco_bmp280.o \
./src/coco_gpio.o \
./src/coco_mpu6050.o \
./src/coco_pwm.o \
./src/coco_qmc5883l.o \
./src/coco_spi.o \
./src/coco_timer.o \
./src/coco_uart.o \
./src/coco_wire.o 

CPP_DEPS += \
./src/coco_adc.d \
./src/coco_bmp280.d \
./src/coco_gpio.d \
./src/coco_mpu6050.d \
./src/coco_pwm.d \
./src/coco_qmc5883l.d \
./src/coco_spi.d \
./src/coco_timer.d \
./src/coco_uart.d \
./src/coco_wire.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C++ Compiler'
	arm-none-eabi-c++ -DDEBUG -D__CODE_RED -D__NEWLIB__ -DCORE_M4 -D__USE_LPCOPEN -D__LPC43XX__ -I"C:\Proyecto ARDUCIAA\arduCIAA\coco_api\inc" -I"C:\Proyecto ARDUCIAA\arduCIAA\lpc_chip_43xx\inc" -I"C:\Proyecto ARDUCIAA\arduCIAA\lpc_chip_43xx\inc\usbd" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fno-rtti -fno-exceptions -fsingle-precision-constant -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mthumb -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


