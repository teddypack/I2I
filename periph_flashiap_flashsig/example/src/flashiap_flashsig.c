/*
 * @brief FLASH IAP programming & FLASH signature example using IAP commands
 * to write to FLASH memory and a FLASH signature generator
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
#include <stdio.h>

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

#define TICKRATE_HZ (10)	/* 10 ticks per second */

/* Last sector address */
#define START_ADDR_LAST_SECTOR      0x00038000

/* Size of each sector */
#define SECTOR_SIZE                 32768

/* LAST SECTOR */
#define IAP_LAST_SECTOR             28

/* Number of bytes to be written to the last sector */
#define IAP_NUM_BYTES_TO_WRITE      256

/* Number elements in array */
#define ARRAY_ELEMENTS          (IAP_NUM_BYTES_TO_WRITE / sizeof(uint32_t))

/* Data array to write to flash */
static uint32_t src_iap_array_data[ARRAY_ELEMENTS];

/*****************************************************************************
 * Private functions
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
 * @brief	Main program body
 * @return	Always returns 0
 */
int main(void)
{
	uint32_t command[5], result[4];
	int i;

	/* Generic Initialization */
	SystemCoreClockUpdate();
	Board_Init();
	Board_LED_Set(0, false);

	/* Enable SysTick Timer */
	SysTick_Config(SystemCoreClock / TICKRATE_HZ);

	/* Initialize the array data to be written to FLASH */
	for (i = 0; i < ARRAY_ELEMENTS; i++) {
		src_iap_array_data[i] = 0x11223340 + i;
	}

	/* Read Part Identification Number*/
	command[0] = IAP_REPID_CMD;						/* Read ID command code */
	iap_entry(command, result);
	DEBUGOUT("Part ID is: %x\r\n", result[1]);

	/* Disable interrupt mode so it doesn't fire during FLASH updates */
	__disable_irq();

	/* IAP Flash programming */
	/* Prepare to write/erase the last sector */
	command[0] = IAP_PREWRRITE_CMD;						/* Prepare to write/erase command code */
	command[1] = IAP_LAST_SECTOR;						/* Start Sector Number */
	command[2] = IAP_LAST_SECTOR;						/* End Sector Number */
	iap_entry(command, result);

	/* Error checking */
	if(result[0] != IAP_CMD_SUCCESS)
	{
			DEBUGOUT("Command failed to execute, return code is: %x\r\n", result[0]);
	}

	/* Erase the last sector */
	command[0] = IAP_ERSSECTOR_CMD;						/* Erase command code*/
	command[1] = IAP_LAST_SECTOR;						/* Start Sector Number */
	command[2] = IAP_LAST_SECTOR;						/* Start Sector Number */
	iap_entry(command, result);

	/* Error checking */
	if(result[0] != IAP_CMD_SUCCESS)
	{
			DEBUGOUT("Command failed to execute, return code is: %x\r\n", result[0]);
	}

	/* Prepare to write/erase the last sector */
	command[0] = IAP_PREWRRITE_CMD;						/* Prepare to write/erase command code */
	command[1] = IAP_LAST_SECTOR;						/* Start Sector Number */
	command[2] = IAP_LAST_SECTOR;						/* Start Sector Number */
	iap_entry(command, result);

	/* Error checking */
	if(result[0] != IAP_CMD_SUCCESS)
	{
			DEBUGOUT("Command failed to execute, return code is: %x\r\n", result[0]);
	}

	/* Write to the last sector */
	command[0] = IAP_WRISECTOR_CMD;						/* Write command code */
	command[1] = (uint32_t) START_ADDR_LAST_SECTOR;		/* Destination Flash Address */
	command[2] = (uint32_t) &src_iap_array_data;		/* Source RAM Address */
	command[3] = IAP_NUM_BYTES_TO_WRITE;				/* Number of Bytes to be written */
	command[4] = SystemCoreClock / 1000;				/* System clock frequency */
	iap_entry(command, result);

	/* Error checking */
	if(result[0] != IAP_CMD_SUCCESS)
	{
			DEBUGOUT("Command failed to execute, return code is: %x\r\n", result[0]);
	}

	/* Re-enable interrupt mode */
	__enable_irq();

	/* Start the signature generator for the last sector */
	Chip_FMC_ComputeSignatureBlocks(START_ADDR_LAST_SECTOR, (SECTOR_SIZE / 16));

	/* Check for signature geenration completion */
	while (Chip_FMC_IsSignatureBusy()) {}

	/* Get the generated FLASH signature value */
	DEBUGOUT("Generated signature for the last sector is: %x \r\n", Chip_FMC_GetSignature(0));

	NVIC_DisableIRQ(SysTick_IRQn);

	return 0;
}
