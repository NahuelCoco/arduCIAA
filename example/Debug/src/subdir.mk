################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/cr_cpp_config.cpp \
../src/cr_startup_lpc43xx.cpp \
../src/example.cpp 

C_SRCS += \
../src/crp.c \
../src/sysinit.c 

OBJS += \
./src/cr_cpp_config.o \
./src/cr_startup_lpc43xx.o \
./src/crp.o \
./src/example.o \
./src/sysinit.o 

CPP_DEPS += \
./src/cr_cpp_config.d \
./src/cr_startup_lpc43xx.d \
./src/example.d 

C_DEPS += \
./src/crp.d \
./src/sysinit.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C++ Compiler'
	arm-none-eabi-c++ -DDEBUG -D__CODE_RED -D__NEWLIB__ -DCORE_M4 -D__USE_LPCOPEN -DNO_BOARD_LIB -DCPP_USE_HEAP -D__LPC43XX__ -I"C:\Proyecto ARDUCIAA\arduCIAA\coco_api" -I"C:\Proyecto ARDUCIAA\arduCIAA\example\inc" -I"C:\Proyecto ARDUCIAA\arduCIAA\lpc_chip_43xx\inc" -I"C:\Proyecto ARDUCIAA\arduCIAA\lpc_chip_43xx\inc\usbd" -I"C:\Proyecto ARDUCIAA\arduCIAA\coco_api\inc" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fno-rtti -fno-exceptions -fsingle-precision-constant -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mthumb -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__CODE_RED -D__NEWLIB__ -DCORE_M4 -D__USE_LPCOPEN -DNO_BOARD_LIB -DCPP_USE_HEAP -D__LPC43XX__ -I"C:\Proyecto ARDUCIAA\arduCIAA\coco_api" -I"C:\Proyecto ARDUCIAA\arduCIAA\example\inc" -I"C:\Proyecto ARDUCIAA\arduCIAA\lpc_chip_43xx\inc" -I"C:\Proyecto ARDUCIAA\arduCIAA\lpc_chip_43xx\inc\usbd" -I"C:\Proyecto ARDUCIAA\arduCIAA\coco_api\inc" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fsingle-precision-constant -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mthumb -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


