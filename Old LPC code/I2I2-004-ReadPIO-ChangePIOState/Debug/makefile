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
all: I2I2-004-ReadPIO-ChangePIOState.axf

# Tool invocations
I2I2-004-ReadPIO-ChangePIOState.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU C++ Linker'
	arm-none-eabi-c++ -nostdlib -L"C:\Users\TeddyP\Documents\GitHub\I2I\lpc_board_nxp_lpcxpresso_11u68\Debug" -L"C:\Users\TeddyP\Documents\GitHub\I2I\lpc_chip_11u6x\Debug" -Xlinker -Map="I2I2-004-ReadPIO-ChangePIOState.map" -Xlinker --cref -Xlinker --gc-sections -Xlinker -print-memory-usage -mcpu=cortex-m0 -mthumb -T "I2I2-003-ChangePIOState_Debug.ld" -o "I2I2-004-ReadPIO-ChangePIOState.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(CC_DEPS)$(C++_DEPS)$(EXECUTABLES)$(C_UPPER_DEPS)$(CXX_DEPS)$(OBJS)$(CPP_DEPS)$(C_DEPS) I2I2-004-ReadPIO-ChangePIOState.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "I2I2-004-ReadPIO-ChangePIOState.axf"; # arm-none-eabi-objcopy -v -O binary "I2I2-004-ReadPIO-ChangePIOState.axf" "I2I2-004-ReadPIO-ChangePIOState.bin" ; # checksum -p LPC11U68 -d "I2I2-004-ReadPIO-ChangePIOState.bin";
	-@echo ' '

.PHONY: all clean dependents post-build

-include ../makefile.targets
