/*
 * @brief I2C bus master example using the ROM API in polling mode
 *
 * @note
 * Copyright(C) NXP Semiconductors, 2013
 * All rights reserved.
 *
 * @par
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * LPC products.  This software is supplied "AS IS" without any warranties of
 * any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * @par
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, under NXP Semiconductors' and its
 * licensor's relevant copyrights in the software, without fee, provided that it
 * is used in conjunction with NXP Semiconductors microcontrollers.  This
 * copyright, permission, and disclaimer notice must appear in all copies of
 * this code.
 */

#include "board.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/* I2C master handle and memory for ROM API */
static I2C_HANDLE_T *i2cHandleMaster;

/* Use a buffer size larger than the expected return value of
   i2c_get_mem_size() for the static I2C handle type */
static uint32_t i2cMasterHandleMEM[0x20];

/* 1Mbps I2C bit-rate */
#define I2C_BITRATE             (100000)

#define I2C_RD_CMD_BIT          (0x01)

#define I2C_FASTPLUS_BIT        (0)
#if (I2C_DEFAULT_SPEED > SPEED_400KHZ)
#undef  I2C_FASTPLUS_BIT
#define I2C_FASTPLUS_BIT IOCON_FASTI2C_EN
#endif

#if defined(BOARD_MANLEY_11U68)
/** 7-bit I2C addresses */
	#define I2C_ADDR_7BIT       (0x90)		/* This is the 7-bit address shifted up 1-bit (orig 0x48) */

	#elif defined(BOARD_NXP_LPCXPRESSO_11U68)
/** 7-bit and 10-bit I2C addresses */
	#define I2C_ADDR_7BIT       (0xC0)		/* This is the 7-bit address shifted up 1-bit (orig 0x60) */
	#define I2C_ADDR_10BIT      (0x2CA)

	#define PCA9532_LS0_ADDR    (0x06)
	#define PCA9532_LS1_ADDR    (0x07)
	#define PCA9532_LS2_ADDR    (0x08)
	#define PCA9532_LS3_ADDR    (0x09)

	#define LED_OVERFLOW_VAL    (0x015555)
	#define LED_SHIFT_COUNT     (0x02)
	#define LED_INIT_VAL        (0x01)
#endif

/* SysTick rate in Hz */
#define TICKRATE_HZ             (10)

/* Current state for LED control via I2C cases */
static volatile int state;

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/* Initializes pin muxing for I2C interface - note that SystemInit() may
   already setup your pin muxing at system startup */
static void Init_I2C_PinMux(void)
{
	Chip_SYSCTL_PeriphReset(RESET_I2C0);
#if defined(BOARD_MANLEY_11U68)
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 4, IOCON_FUNC1 | I2C_FASTPLUS_BIT);
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 5, IOCON_FUNC1 | I2C_FASTPLUS_BIT);

#elif defined(BOARD_NXP_LPCXPRESSO_11U68)
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 4,
						 (IOCON_FUNC1 | I2C_FASTPLUS_BIT) | IOCON_DIGMODE_EN);
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 5,
						 (IOCON_FUNC1 | I2C_FASTPLUS_BIT) | IOCON_DIGMODE_EN);

#else
#error "No I2C Pin Muxing defined for this example"
#endif
}

/* Turn on LED to indicate an error */
static void errorI2C(void)
{
	Board_LED_Set(0, true);
	while (1) {}
}

/* Setup I2C handle and parameters */
static void setupI2CMaster()
{
	/* Enable I2C clock and reset I2C peripheral - the boot ROM does not
	   do this */
	Chip_I2C_Init(I2C0);

	/* Perform a sanity check on the storage allocation */
	if (LPC_I2CD_API->i2c_get_mem_size() > sizeof(i2cMasterHandleMEM)) {
		/* Example only: this should never happen and probably isn't needed for
		   most I2C code. */
		errorI2C();
	}

	/* Setup the I2C handle */
	i2cHandleMaster = LPC_I2CD_API->i2c_setup(LPC_I2C0_BASE, i2cMasterHandleMEM);
	if (i2cHandleMaster == NULL) {
		errorI2C();
	}

	/* Set I2C bitrate */
	if (LPC_I2CD_API->i2c_set_bitrate(i2cHandleMaster, Chip_Clock_GetSystemClockRate(),
									  I2C_BITRATE) != LPC_OK) {
		errorI2C();
	}
}

/* Master receive in polling mode */
#if defined(BOARD_MANLEY_11U68)
static void readI2CMaster(uint16_t AddressI2C, uint8_t *readPtr, bool address10Bit)
{
	uint8_t recvData[10];
	I2C_PARAM_T param;
	I2C_RESULT_T result;
	ErrorCode_t error_code;
	int index = 0;

	/* Setup I2C receive for address/read, read desired LED state, then stop */
	if (address10Bit) {
		/* 10-bit addressing - 4 MSBs of slave address in first byte of
		   transmit buffer */
		recvData[index++] = (uint8_t) (((AddressI2C >> 7) & 0x06) | 0xF0);
		recvData[index++] = (uint8_t) (AddressI2C & 0x0FF);
	}
	else {
		/* 7-bit address */
		recvData[index++] = (uint8_t) AddressI2C;
	}

	/* Setup I2C parameters for number of bytes with stop - appears as follows on bus:
	   Start - address7 or address10upper - ack
	   (10 bits addressing only) address10lower - ack
	   value 1 (read) - ack
	   value 2 read) - ack - stop */
	param.num_bytes_send = 0;
	param.num_bytes_rec = 2;
	param.buffer_ptr_rec = &recvData[0];
	param.stop_flag = 1;

	/* Do master read transfer */
	error_code = LPC_I2CD_API->i2c_master_receive_poll(i2cHandleMaster, &param, &result);

	/* Completed without erors? */
	if (error_code != LPC_OK) {
		/* Likely cause is NAK */
		DEBUGOUT("i2c_master_receive error code : %x\r\b", error_code);
		errorI2C();
	}

	/* Note results are only valid when there are no errors */
	*readPtr++ = recvData[0];
	*readPtr++ = recvData[1];
}

/* Function to read I2C temperature sensor and output result */
static void ReadTemperatureI2C(void)
{
	uint8_t readState_Input[2];

	/* Read temperature */
	readI2CMaster(I2C_ADDR_7BIT | I2C_RD_CMD_BIT, readState_Input, false);

	/* Output temperature. */
	DEBUGOUT("Temperature read over I2C is %d (MSB) %d (LSB)\r\n", (int) readState_Input[0],
			 (int) (readState_Input[1] & 0x80));
}
#endif

#if defined(BOARD_NXP_LPCXPRESSO_11U68)
/* Master transmit/receive in polling mode */
static void sendReadI2CMaster(uint16_t AddressI2C,
							  uint8_t ledStateOut,
							  uint8_t *ledStateInPtr,
							  bool address10Bit,
							  uint8_t regAddr)
{
	uint8_t recvData[10], sendData[10];
	I2C_PARAM_T param;
	I2C_RESULT_T result;
	ErrorCode_t error_code;
	int sindex = 0, rindex = 0;

	/* Setup I2C send for address/send, send desired LED state, then stop */
	/* Setup I2C receive for address/read, read desired LED state, then stop */
	if (address10Bit) {
		/* 10-bit addressing - 4 MSBs of slave address in first byte of
		   transmit buffer */
		sendData[sindex] = (uint8_t) (((AddressI2C >> 7) & 0x06) | 0xF0);
		recvData[rindex++] = sendData[sindex++];
		sendData[sindex] = (uint8_t) (AddressI2C & 0x0FF);
		recvData[rindex++] = sendData[sindex++];
	}
	else {
		/* 7-bit address */
		sendData[sindex++] = (uint8_t) AddressI2C;
		recvData[rindex++] = (uint8_t) AddressI2C | I2C_RD_CMD_BIT;
	}
	sendData[sindex++] = (uint8_t) regAddr;

	/* Setup parameters for transfer */
	param.num_bytes_send = sindex;
	param.num_bytes_rec = rindex;
	param.buffer_ptr_send = &sendData[0];
	param.buffer_ptr_rec = &recvData[0];
	param.stop_flag = 1;

	/* Do master read transfer */
	error_code = LPC_I2CD_API->i2c_master_tx_rx_poll(i2cHandleMaster, &param, &result);

	/* Completed without errors? */
	if (error_code != LPC_OK) {
		/* Likely cause is NAK */
		DEBUGOUT("i2c_master_tx_rx error code : %x\r\b", error_code);
		errorI2C();
	}
	else {
		/* Note results are only valid when there are no errors */
		*ledStateInPtr++ = recvData[0];
		*ledStateInPtr++ = recvData[1];
	}
}

/* Master transmit in polling mode */
static void sendI2CMaster(uint16_t AddressI2C, bool address10Bit, uint8_t regAddr, uint8_t regVal)
{
	uint8_t sendData[10];
	I2C_PARAM_T param;
	I2C_RESULT_T result;
	ErrorCode_t error_code;
	int index = 0;

	/* Setup I2C send for address/send, send desired LED state, then stop */
	if (address10Bit) {
		/* 10-bit addressing - 4 MSBs of slave address in first byte of
		   transmit buffer */
		sendData[index++] = (uint8_t) (((AddressI2C >> 7) & 0x06) | 0xF0);
		sendData[index++] = (uint8_t) (AddressI2C & 0x0FF);
	}
	else {
		/* 7-bit address */
		sendData[index++] = (uint8_t) AddressI2C;
	}

	sendData[index++] = (uint8_t) regAddr;			/* Send control byte to access PCA9532_LS0_ADDR */
	sendData[index++] = (uint8_t) regVal;			/* Send data byte to set PCA9532_LS0 LED register */

	/* Setup I2C parameters for number of bytes with stop - appears as follows on bus:
	   Start - address7 or address10upper - ack
	   (10 bits addressing only) address10lower - ack
	   value 1 - ack
	   value 2 - ack - stop */
	param.num_bytes_send = index;
	param.buffer_ptr_send = &sendData[0];
	param.num_bytes_rec = 0;
	param.stop_flag = 1;

	/* Do master write transfer */
	error_code = LPC_I2CD_API->i2c_master_transmit_poll(i2cHandleMaster, &param, &result);

	/* Completed without erors? */
	if (error_code != LPC_OK) {
		/* Likely cause is NAK */
		DEBUGOUT("i2c_master_transmit error code : %x\r\b", error_code);
		errorI2C();
	}

	/* Note results are only valid when there are no errors */
}

/* Function to shift LED display pattern & test for overflow */
static uint32_t IncLedPattern(uint32_t inputValue)
{
	/* Shift LED pattern to next LED. */
	inputValue <<= LED_SHIFT_COUNT;
	inputValue |= LED_INIT_VAL;

	/* Test for LED overflow */
	if (inputValue == LED_OVERFLOW_VAL) {
		/* Reset to starting value (1 LED on) */
		inputValue = LED_INIT_VAL;
	}

	/* Return next LED pattern */
	return inputValue;
}

/* Function to write LED pattern to Xpresso base board */
static uint32_t WriteLedsOverI2C(uint32_t ledValue)
{
	/* Write LS0 and LS1 LEDs on base board */
	sendI2CMaster(I2C_ADDR_7BIT, false, PCA9532_LS0_ADDR, (uint8_t) ledValue);
	sendI2CMaster(I2C_ADDR_7BIT, false, PCA9532_LS1_ADDR, (uint8_t) (ledValue >> 8));

	/* Inc LED pattern */
	ledValue = IncLedPattern(ledValue);

	/* Return updated LED pattern */
	return ledValue;
}

/* Function to read LED pattern then write LED pattern to board-specific device. */
static void ReadWriteLedsOverI2C(void)
{
	uint8_t readState_Input0;
	uint8_t readState_Input1;

	/* Write LS0 and LS1 LEDs on base board */
	sendReadI2CMaster(I2C_ADDR_7BIT, true, &readState_Input0, false, PCA9532_LS0_ADDR);
	sendI2CMaster(I2C_ADDR_7BIT, false, PCA9532_LS2_ADDR, readState_Input0);
	sendReadI2CMaster(I2C_ADDR_7BIT, true, &readState_Input1, false, PCA9532_LS1_ADDR);
	sendI2CMaster(I2C_ADDR_7BIT, false, PCA9532_LS3_ADDR, readState_Input1);
}
#endif

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/**
 * @brief	Handle interrupt from SysTick timer
 * @return	Nothing
 */
void SysTick_Handler(void)
{
	static int ticks = 0;

	ticks++;
	if (ticks > TICKRATE_HZ) {
		ticks = 0;
		state = 1 - state;
	}
}

/**
 * @brief	Main routine for I2C example
 * @return	Function should not exit
 */
int main(void)
{
	int lastState = -1;
#if defined(BOARD_NXP_LPCXPRESSO_11U68)
	uint32_t ledValue = LED_INIT_VAL;
#endif

	/* Generic Initialization */
	SystemCoreClockUpdate();
	Board_Init();

	/* Clear activity LED */
	Board_LED_Set(0, false);

	/* Setup I2C pin muxing */
	Init_I2C_PinMux();

	/* Allocate I2C handle, setup I2C rate, and initialize I2C
	   clocking */
	setupI2CMaster();

	/* Enable SysTick Timer */
	SysTick_Config(SystemCoreClock / TICKRATE_HZ);

	/* Loop forever, toggle LED on other board via I2C */
	while (1) {

		/* Sleep until a state change occurs in SysTick */
		while (lastState == state) {
			__WFI();
		}

		/* Handle states */
		switch (state) {
		case 0:
#if defined(BOARD_MANLEY_11U68)
			/* Read Manley board's I2C temperature sensor and output result */
			ReadTemperatureI2C();
#elif defined(BOARD_NXP_LPCXPRESSO_11U68)
			/* Write LED pattern over I2C */
			ledValue = WriteLedsOverI2C(ledValue);
#endif
			break;

		case 1:
		default:
#if defined(BOARD_MANLEY_11U68)

			/* No read-write example for Manley board */

#elif defined(BOARD_NXP_LPCXPRESSO_11U68)
			/* Read LED pattern then write LED pattern over I2C */
			ReadWriteLedsOverI2C();
#endif
			break;
		}

		/* Reset lastState to allow for WFI. */
		lastState = state;

		/* Toggle LED to show activity. */
		Board_LED_Toggle(0);
	}

	/* Code never reaches here. Only used to satisfy standard main(). */
	return 0;
}
