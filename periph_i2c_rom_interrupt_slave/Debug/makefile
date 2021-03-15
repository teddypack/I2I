################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include example/src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: periph_i2c_rom_interrupt_slave.axf

# Tool invocations
periph_i2c_rom_interrupt_slave.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"C:\Users\TeddyP\Documents\MCUXpressoIDE_11.3.0_5222\workspace\lpc_chip_11u6x\Debug" -L"C:\Users\TeddyP\Documents\MCUXpressoIDE_11.3.0_5222\workspace\lpc_board_nxp_lpcxpresso_11u68\Debug" -Xlinker -Map="periph_i2c_rom_interrupt_slave.map" -Xlinker --gc-sections -Xlinker --allow-multiple-definition -mcpu=cortex-m0 -mthumb -T "periph_i2c_rom_interrupt_slave_Debug.ld" -o "periph_i2c_rom_interrupt_slave.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) periph_i2c_rom_interrupt_slave.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "periph_i2c_rom_interrupt_slave.axf"; # arm-none-eabi-objcopy -O binary "periph_i2c_rom_interrupt_slave.axf" "periph_i2c_rom_interrupt_slave.bin" ; checksum -p LPC11U68 -d "periph_i2c_rom_interrupt_slave.bin";
	-@echo ' '

.PHONY: all clean dependents post-build

-include ../makefile.targets
