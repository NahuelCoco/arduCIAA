################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: example.axf

dependents:
	-cd C:/Proyecto\ ARDUCIAA/arduCIAA/coco_api/Debug && $(MAKE) clean all

# Tool invocations
example.axf: $(OBJS) $(USER_OBJS) C:/Proyecto\ ARDUCIAA/arduCIAA/coco_api/Debug/libcoco_api.a
	@echo 'Building target: $@'
	@echo 'Invoking: MCU C++ Linker'
	arm-none-eabi-c++ -nostdlib -L"C:\Proyecto ARDUCIAA\arduCIAA\lpc_chip_43xx\Debug" -L"C:\Proyecto ARDUCIAA\arduCIAA\coco_api\Debug" -Xlinker -Map="example.map" -Xlinker --cref -Xlinker --gc-sections -Xlinker -print-memory-usage -mcpu=cortex-m4 -mthumb -T "example_Debug.ld" -o "example.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(CC_DEPS)$(C++_DEPS)$(EXECUTABLES)$(C_UPPER_DEPS)$(CXX_DEPS)$(OBJS)$(CPP_DEPS)$(C_DEPS) example.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "example.axf"; # arm-none-eabi-objcopy -v -O binary "example.axf" "example.bin" ; # checksum -p LPC4337 -d "example.bin";
	-@echo ' '

.PHONY: all clean dependents post-build
C:/Proyecto\ ARDUCIAA/arduCIAA/coco_api/Debug/libcoco_api.a:

-include ../makefile.targets
