/*
 * @brief CLKOUT example
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

static volatile uint32_t ticks100;

/* Use this pin for clockout */
#define CLKOUT_PIO0_1 1

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/**
 * @brief	Handle interrupt from SysTick timer
 * @return	Nothing
 */
void SysTick_Handler(void)
{
	ticks100++;
	if ((ticks100 % 100) == 0) {
		Board_LED_Toggle(0);
	}
}

/**
 * @brief	main routine for CLKOUT example
 * @return	Function should not exit.
 */
int main(void)
{
	CHIP_SYSCTL_CLKOUTSRC_T clkoutClks;

	SystemCoreClockUpdate();
	Board_Init();

	Board_LED_Set(0, false);

	/* Enable and setup SysTick Timer at a 100Hz rate */
	SysTick_Config(SystemCoreClock / 100);

	/* WDT oscillator freq = 0.6MHz divided by 64 = 9.375khz */
	Chip_Clock_SetWDTOSC(WDTLFO_OSC_0_60, 64);

	/* Enable the power to the WDT */
	Chip_SYSCTL_PowerUp(SYSCTL_SLPWAKE_WDTOSC_PD);

#if (CLKOUT_PIO0_1 == 1)
	/* Configure PIN0.1 for CLKOUT */
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 1, IOCON_FUNC1);
#endif

	/* Cycle through all clock sources for the CLKOUT pin */
	while (1) {
		for (clkoutClks = SYSCTL_CLKOUTSRC_IRC;
			 clkoutClks <= SYSCTL_CLKOUTSRC_MAINSYSCLK; clkoutClks++) {

			/* The CLKOUT source selection may hang if a clock is selected
			   for CLKOUT that is not operational. */

		   /* Setup CLKOUT pin for specific clock with a divider of 1 */
			Chip_Clock_SetCLKOUTSource(clkoutClks, 1);

			/* Wait 5 seconds */
			ticks100 = 0;
			while (ticks100 < 500) {
				__WFI();
			}
		}

		/* Disable CLKOUT pin by setting divider to 0 */
		Chip_Clock_SetCLKOUTSource(SYSCTL_CLKOUTSRC_MAINSYSCLK, 0);
	}

	return 0;
}
