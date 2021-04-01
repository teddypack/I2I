/*
 * @brief I2CM bus master example using interrupt mode
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

/* I2CM transfer record */
static I2CM_XFER_T  i2cmXferRec;

/* 1Mbps I2C bit-rate */
#define I2C_BITRATE         (100000)

#define I2C_RD_CMD_BIT      (0x01)

#define I2C_FASTPLUS_BIT    (0)
#if (I2C_DEFAULT_SPEED > SPEED_400KHZ)
#undef  I2C_FASTPLUS_BIT
#define I2C_FASTPLUS_BIT IOCON_FASTI2C_EN
#endif

#if defined(BOARD_MANLEY_11U68)
/** 7-bit I2C addresses */
#define I2C_ADDR_7BIT       (0x48)

#elif defined(BOARD_NXP_LPCXPRESSO_11U68)
/** 7-bit and 10-bit I2C addresses */
#define I2C_ADDR_7BIT       (0x60)
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
#define TICKRATE_HZ         (10)

/* Current state for LED control via I2C cases */
static volatile int state;

static volatile int intErrCode;

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

/* Setup I2C handle and parameters */
static void setupI2CMaster()
{
	/* Enable I2C clock and reset I2C peripheral - the boot ROM does not
	   do this */
	Chip_I2CM_Init(LPC_I2C0);

	/* Setup I2CM transfer rate */
	Chip_I2CM_SetBusSpeed(LPC_I2C0, I2C_BITRATE);
}

/* Function to wait for I2CM transfer completion */
static void WaitForI2cXferComplete(I2CM_XFER_T *xferRecPtr)
{
	/* Test for still transferring data */
	while (xferRecPtr->status == I2CM_STATUS_BUSY) {
		/* Sleep until next interrupt */
		__WFI();
	}
}

/* Function to setup and execute I2C transfer request */
static void SetupXferRecAndExecute(uint8_t devAddr,
								   uint8_t *txBuffPtr,
								   uint16_t txSize,
								   uint8_t *rxBuffPtr,
								   uint16_t rxSize)
{
	/* Setup I2C transfer record */
	i2cmXferRec.slaveAddr = devAddr;
	i2cmXferRec.options = 0;
	i2cmXferRec.status = 0;
	i2cmXferRec.txSz = txSize;
	i2cmXferRec.rxSz = rxSize;
	i2cmXferRec.txBuff = txBuffPtr;
	i2cmXferRec.rxBuff = rxBuffPtr;
	Chip_I2CM_Xfer(LPC_I2C0, &i2cmXferRec);

	/* Wait for transfer completion */
	WaitForI2cXferComplete(&i2cmXferRec);
}

/* Master I2CM receive in interrupt mode */
#if defined(BOARD_MANLEY_11U68)
static void readI2CMaster(uint16_t AddressI2C, uint8_t *readPtr, bool address10Bit)
{
	uint8_t rx_buffer[3];
	uint8_t tx_buffer[3];

	/* Read LM75 temerature sensor */
	SetupXferRecAndExecute(AddressI2C, tx_buffer, 1, rx_buffer, 2);

	/* Test for valid operation */
	if (i2cmXferRec.status == I2CM_STATUS_OK) {
		/* Note results are only valid when there are no errors */
		*readPtr++ = rx_buffer[0];
		*readPtr++ = rx_buffer[1];
	}
}

/* Function to read I2C temperature sensor and output result */
static void ReadTemperatureI2CM(void)
{
	uint8_t readState_Input[2];

	/* Read temperature */
	readI2CMaster(I2C_ADDR_7BIT, readState_Input, false);

	/* Output temperature. */
	DEBUGOUT("Temperature read over I2C is %d (MSB) %d (LSB)\r\n", (int) readState_Input[0],
			 (int) (readState_Input[1] & 0x80));
}

#endif

/* Master transmit in interrupt mode */
#if defined(BOARD_NXP_LPCXPRESSO_11U68)

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
static uint32_t WriteLedsOverI2CM(uint32_t ledValue)
{
	uint8_t tx_buffer[3];

	/* Write 1st LED bank */
	tx_buffer[0] = PCA9532_LS0_ADDR;
	tx_buffer[1] = ledValue;
	SetupXferRecAndExecute(I2C_ADDR_7BIT, tx_buffer, 2, NULL, 0);

	/* Write 2nd LED bank */
	tx_buffer[0] = PCA9532_LS1_ADDR;
	tx_buffer[1] = ledValue >> 8;
	SetupXferRecAndExecute(I2C_ADDR_7BIT, tx_buffer, 2, NULL, 0);

	/* Inc LED pattern */
	ledValue = IncLedPattern(ledValue);

	/* Return updated LED pattern */
	return ledValue;
}

/* Function to read LED pattern then write LED pattern to board-specific device. */
static void ReadWriteLedsOverI2CM(void)
{
	uint8_t rx_buffer[3];
	uint8_t tx_buffer[3];

	/* Read 1st LED bank */
	tx_buffer[0] = PCA9532_LS0_ADDR;
	SetupXferRecAndExecute(I2C_ADDR_7BIT, tx_buffer, 1, rx_buffer, 1);

	/* Write 3rd LED bank */
	tx_buffer[0] = PCA9532_LS2_ADDR;
	tx_buffer[1] = rx_buffer[0];
	SetupXferRecAndExecute(I2C_ADDR_7BIT, tx_buffer, 2, NULL, 0);

	/* Read 2nd LED bank */
	tx_buffer[0] = PCA9532_LS1_ADDR;
	SetupXferRecAndExecute(I2C_ADDR_7BIT, tx_buffer, 1, rx_buffer, 1);

	/* Write 4th LED bank */
	tx_buffer[0] = PCA9532_LS3_ADDR;
	tx_buffer[1] = rx_buffer[0];
	SetupXferRecAndExecute(I2C_ADDR_7BIT, tx_buffer, 2, NULL, 0);
}

#endif

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/**
 * @brief	Handle I2C0 interrupt by calling I2CM interrupt transfer handler
 * @return	Nothing
 */
void I2C0_IRQHandler(void)
{
	/* Call I2CM ISR function with the I2C device and transfer rec */
	Chip_I2CM_XferHandler(LPC_I2C0, &i2cmXferRec);
}

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

	/* Enable the interrupt for the I2C */
	NVIC_EnableIRQ(I2C0_IRQn);

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
			ReadTemperatureI2CM();
#elif defined(BOARD_NXP_LPCXPRESSO_11U68)
			/* Write LED pattern over I2C */
			ledValue = WriteLedsOverI2CM(ledValue);
#endif
			break;

		case 1:
		default:
#if defined(BOARD_MANLEY_11U68)
			/* No read-write example for Manley board */

#elif defined(BOARD_NXP_LPCXPRESSO_11U68)
			/* Read LED pattern then write LED pattern over I2C */
			ReadWriteLedsOverI2CM();
#endif
			break;
		}

		/* Reset lastState to allow for WFI */
		lastState = state;

		/* Toggle LED to show activity. */
		Board_LED_Toggle(0);
	}

	/* Code never reaches here. Only used to satisfy standard main() */
	return 0;
}
