################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../example/src/aeabi_romdiv_patch.s 

C_SRCS += \
../example/src/cr_startup_lpc11u6x.c \
../example/src/crp.c \
../example/src/hid_desc.c \
../example/src/hid_keyboard.c \
../example/src/hid_main.c \
../example/src/ms_timer.c \
../example/src/mtb.c 

OBJS += \
./example/src/aeabi_romdiv_patch.o \
./example/src/cr_startup_lpc11u6x.o \
./example/src/crp.o \
./example/src/hid_desc.o \
./example/src/hid_keyboard.o \
./example/src/hid_main.o \
./example/src/ms_timer.o \
./example/src/mtb.o 

C_DEPS += \
./example/src/cr_startup_lpc11u6x.d \
./example/src/crp.d \
./example/src/hid_desc.d \
./example/src/hid_keyboard.d \
./example/src/hid_main.d \
./example/src/ms_timer.d \
./example/src/mtb.d 


# Each subdirectory must supply rules for building sources it contributes
example/src/%.o: ../example/src/%.s
	@echo 'Building file: $<'
	@echo 'Invoking: MCU Assembler'
	arm-none-eabi-gcc -c -x assembler-with-cpp -D__REDLIB__ -DDEBUG -D__CODE_RED -D__USE_ROMDIVIDE -g3 -mcpu=cortex-m0 -mthumb -D__REDLIB__ -specs=redlib.specs -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

example/src/%.o: ../example/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DDEBUG -D__CODE_RED -D__USE_LPCOPEN -D__MTB_BUFFER_SIZE=256 -DCORE_M0PLUS -D__USE_ROMDIVIDE -I"C:\Users\TeddyP\Documents\MCUXpressoIDE_11.3.0_5222\workspace\lpc_chip_11u6x\inc" -I"C:\Users\TeddyP\Documents\MCUXpressoIDE_11.3.0_5222\workspace\lpc_board_manley_11u68\inc" -I"C:\Users\TeddyP\Documents\MCUXpressoIDE_11.3.0_5222\workspace\usbd_rom_hid_keyboard_ls\example\inc" -I"C:\Users\TeddyP\Documents\MCUXpressoIDE_11.3.0_5222\workspace\lpc_chip_11u6x\inc\usbd" -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m0 -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

