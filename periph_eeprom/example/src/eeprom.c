/*
 * @brief EEPROM example using IAP command to read/write from/to EEPROM
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
#include "string.h"
#include <stdio.h>

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

#define TICKRATE_HZ (10)	/* 10 ticks per second */

/* EEPROM address location */
#define EEPROM_ADDRESS      0x00000100

/* Write count */
#define IAP_NUM_BYTES_TO_READ_WRITE 32

/* Tag for checking if a string already exists in EEPROM */
#define CHKTAG          "NxP"
#define CHKTAG_SIZE     3

/* ASCII ESC character code */
#define ESC_CHAR        27

/* Read/write buffer (32-bit aligned) */
uint32_t buffer[IAP_NUM_BYTES_TO_READ_WRITE / sizeof(uint32_t)];

/* Test string for no DEBUG */
#define TESTSTRING "Test String"

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/* Show current string stored in UART */
static void ShowString(char *str) {
	int stSize;

	/* Is data tagged with check pattern? */
	if (strncmp(str, CHKTAG, CHKTAG_SIZE) == 0) {
		/* Get next byte, which is the string size in bytes */
		stSize = (uint32_t) str[3];
		if (stSize > 32) {
			stSize = 32;
		}

		/* Add terminator */
		str[4 + stSize] = '\0';

		/* Show string stored in EEPROM */
		DEBUGSTR("Stored string found in EEEPROM\r\n");
		DEBUGSTR("-->");
		DEBUGSTR((char *) &str[4]);
		DEBUGSTR("<--\r\n");
	}
	else {
		DEBUGSTR("No string stored in the EEPROM\r\n");
	}
}

/* Get a string to save from the UART */
static uint32_t MakeString(uint8_t *str)
{
	int index, byte;
	char strOut[2];

	/* Get a string up to 32 bytes to write into EEPROM */
	DEBUGSTR("\r\nEnter a string to write into EEPROM\r\n");
	DEBUGSTR("Up to 32 bytes in length, press ESC to accept\r\n");

	/* Setup header */
	strncpy((char *) str, CHKTAG, CHKTAG_SIZE);

#if defined(DEBUG_ENABLE)
	/* Read until escape, but cap at 32 characters */
	index = 0;
	strOut[1] = '\0';
	byte = DEBUGIN();
	while ((index < 32) && (byte != ESC_CHAR)) {
		if (byte != EOF) {
			strOut[0] = str[4 + index] = (uint8_t) byte;
			DEBUGSTR(strOut);
			index++;
		}
		byte = DEBUGIN();
	}

#else
	/* String to be written to EEPROM */
	strcpy(&str[4], TESTSTRING);
	index = strlen(TESTSTRING);
#endif
	str[3] = (uint8_t) index;

	return (uint32_t) index;
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/**
 * @brief	Handle interrupt from SysTick timer
 * @return	Nothing
 */
void SysTick_Handler(void)
{
	Board_LED_Toggle(0);
}

/**
 * @brief	main routine for EEPROM example
 * @return	Always returns 0
 */
int main(void)
{
	uint32_t command[5], result[4];
	uint8_t *ptr = (uint8_t *) buffer;

	/* Generic Initialization */
	SystemCoreClockUpdate();
	Board_Init();
	Board_LED_Set(0, false);

	/* Enable SysTick Timer */
	SysTick_Config(SystemCoreClock / TICKRATE_HZ);

	/* Data to be read from EEPROM */
	command[0] = IAP_EEPROM_READ;
	command[1] = EEPROM_ADDRESS;
	command[2] = (uint32_t) ptr;
	command[3] = IAP_NUM_BYTES_TO_READ_WRITE;
	command[4] = SystemCoreClock / 1000;
	iap_entry(command, result);

	/* Error checking */
	if (result[0] != IAP_CMD_SUCCESS) {
		DEBUGOUT("Command failed to execute, return code is: %x\r\n", result[0]);
	}

	/* Check and display string if it exists */
	ShowString((char *) ptr);

	/* Get a string to save */
	MakeString(ptr);

	/* Data to be written to EEPROM */
	command[0] = IAP_EEPROM_WRITE;
	command[1] = EEPROM_ADDRESS;
	command[2] = (uint32_t) ptr;
	command[3] = IAP_NUM_BYTES_TO_READ_WRITE;
	command[4] = SystemCoreClock / 1000;
	iap_entry(command, result);

	/* Error checking */
	if (result[0] == IAP_CMD_SUCCESS) {
		DEBUGSTR("EEPROM write passed\r\n");
	}
	else {
		DEBUGOUT("EEPROM write failed, return code is: %x\r\n", result[0]);
	}

	return 0;
}
