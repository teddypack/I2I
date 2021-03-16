################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../src/aeabi_romdiv_patch.s 

CPP_SRCS += \
../src/I2I2-004.cpp \
../src/cr_cpp_config.cpp \
../src/cr_startup_lpc11u6x.cpp 

C_SRCS += \
../src/crp.c \
../src/mtb.c \
../src/sysinit.c 

OBJS += \
./src/I2I2-004.o \
./src/aeabi_romdiv_patch.o \
./src/cr_cpp_config.o \
./src/cr_startup_lpc11u6x.o \
./src/crp.o \
./src/mtb.o \
./src/sysinit.o 

CPP_DEPS += \
./src/I2I2-004.d \
./src/cr_cpp_config.d \
./src/cr_startup_lpc11u6x.d 

C_DEPS += \
./src/crp.d \
./src/mtb.d \
./src/sysinit.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C++ Compiler'
	arm-none-eabi-c++ -DDEBUG -D__CODE_RED -D__NEWLIB__ -DCORE_M0PLUS -D__MTB_BUFFER_SIZE=256 -D__USE_ROMDIVIDE -D__USE_LPCOPEN -DCPP_USE_HEAP -D__LPC11U6X__ -I"C:\Users\TeddyP\Documents\GitHub\I2I\lpc_board_nxp_lpcxpresso_11u68\inc" -I"C:\Users\TeddyP\Documents\GitHub\I2I\lpc_chip_11u6x\inc" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fno-rtti -fno-exceptions -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m0 -mthumb -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.s
	@echo 'Building file: $<'
	@echo 'Invoking: MCU Assembler'
	arm-none-eabi-gcc -c -x assembler-with-cpp -D__NEWLIB__ -DDEBUG -D__CODE_RED -DCORE_M0PLUS -D__USE_ROMDIVIDE -D__USE_LPCOPEN -D__LPC11U6X__ -I"C:\Users\TeddyP\Documents\GitHub\I2I\lpc_board_nxp_lpcxpresso_11u68\inc" -I"C:\Users\TeddyP\Documents\GitHub\I2I\lpc_chip_11u6x\inc" -g3 -mcpu=cortex-m0 -mthumb -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__CODE_RED -D__NEWLIB__ -DCORE_M0PLUS -D__MTB_BUFFER_SIZE=256 -D__USE_ROMDIVIDE -D__USE_LPCOPEN -DCPP_USE_HEAP -D__LPC11U6X__ -I"C:\Users\TeddyP\Documents\GitHub\I2I\lpc_board_nxp_lpcxpresso_11u68\inc" -I"C:\Users\TeddyP\Documents\GitHub\I2I\lpc_chip_11u6x\inc" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m0 -mthumb -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


