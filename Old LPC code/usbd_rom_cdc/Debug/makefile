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
all: usbd_rom_cdc.axf

# Tool invocations
usbd_rom_cdc.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"C:\Users\TeddyP\Documents\MCUXpressoIDE_11.3.0_5222\workspace\lpc_chip_11u6x\Debug" -L"C:\Users\TeddyP\Documents\MCUXpressoIDE_11.3.0_5222\workspace\lpc_board_nxp_lpcxpresso_11u68\Debug" -Xlinker -Map="usbd_rom_cdc.map" -Xlinker --gc-sections -Xlinker --allow-multiple-definition -mcpu=cortex-m0 -mthumb -T "usbd_rom_cdc_Debug.ld" -o "usbd_rom_cdc.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) usbd_rom_cdc.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "usbd_rom_cdc.axf"; # arm-none-eabi-objcopy -O binary "usbd_rom_cdc.axf" "usbd_rom_cdc.bin" ; checksum -p LPC11U68 -d "usbd_rom_cdc.bin";
	-@echo ' '

.PHONY: all clean dependents post-build

-include ../makefile.targets
