################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../example/src/aeabi_romdiv_patch.s 

C_SRCS += \
../example/src/cr_startup_lpc11u6x.c \
../example/src/crp.c \
../example/src/font_render.c \
../example/src/hid_desc.c \
../example/src/hid_i2c.c \
../example/src/lcd_usbsio.c \
../example/src/lpc_winfreesystem14x16.c \
../example/src/mtb.c \
../example/src/nxp_logo128x47.c \
../example/src/sysinit.c \
../example/src/usbsio_hid_i2c.c 

OBJS += \
./example/src/aeabi_romdiv_patch.o \
./example/src/cr_startup_lpc11u6x.o \
./example/src/crp.o \
./example/src/font_render.o \
./example/src/hid_desc.o \
./example/src/hid_i2c.o \
./example/src/lcd_usbsio.o \
./example/src/lpc_winfreesystem14x16.o \
./example/src/mtb.o \
./example/src/nxp_logo128x47.o \
./example/src/sysinit.o \
./example/src/usbsio_hid_i2c.o 

C_DEPS += \
./example/src/cr_startup_lpc11u6x.d \
./example/src/crp.d \
./example/src/font_render.d \
./example/src/hid_desc.d \
./example/src/hid_i2c.d \
./example/src/lcd_usbsio.d \
./example/src/lpc_winfreesystem14x16.d \
./example/src/mtb.d \
./example/src/nxp_logo128x47.d \
./example/src/sysinit.d \
./example/src/usbsio_hid_i2c.d 


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
	arm-none-eabi-gcc -D__REDLIB__ -DDEBUG -D__CODE_RED -D__USE_LPCOPEN -D__MTB_BUFFER_SIZE=256 -DCORE_M0PLUS -D__USE_ROMDIVIDE -I"C:\Users\TeddyP\Documents\MCUXpressoIDE_11.3.0_5222\workspace\lpc_chip_11u6x\inc" -I"C:\Users\TeddyP\Documents\MCUXpressoIDE_11.3.0_5222\workspace\lpc_board_manley_11u68\inc" -I"C:\Users\TeddyP\Documents\MCUXpressoIDE_11.3.0_5222\workspace\demo_usbsio_lcd\example\inc" -I"C:\Users\TeddyP\Documents\MCUXpressoIDE_11.3.0_5222\workspace\lpc_chip_11u6x\inc\usbd" -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m0 -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

