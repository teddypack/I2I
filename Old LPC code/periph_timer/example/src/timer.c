/*
 * @brief LED blinking example using timers
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

#define TICKRATE_HZ1 (1)/* 1 ticks per second */
#define TICKRATE_HZ2 (2)/* 2 ticks per second */
#define PRESCALE_HZ2 (0xFFFF)	/* 16-bit prescale count */

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
 * @brief	Handle interrupt from 32-bit timer 0
 * @return	Nothing
 */
void TIMER32_0_IRQHandler(void)
{
	if (Chip_TIMER_MatchPending(LPC_TIMER32_0, 1)) {
		Chip_TIMER_ClearMatch(LPC_TIMER32_0, 1);
		Board_LED_Toggle(0);
	}
}

/**
 * @brief	Handle interrupt from 16-bit timer 0
 * @return	Nothing
 */
void TIMER16_0_IRQHandler(void)
{
	if (Chip_TIMER_MatchPending(LPC_TIMER16_0, 1)) {
		Chip_TIMER_ClearMatch(LPC_TIMER16_0, 1);
		Board_LED_Toggle(1);
	}
}

/**
 * @brief	main routine for timer example
 * @return	Function should not exit.
 */
int main(void)
{
	uint32_t timerFreq;

	SystemCoreClockUpdate();
	Board_Init();

	/* Initialize 32-bit timer 0 clock */
	Chip_TIMER_Init(LPC_TIMER32_0);

	/* Initialize 16-bit timer 0 clock */
	Chip_TIMER_Init(LPC_TIMER16_0);

	/* Timer rate is system clock rate */
	timerFreq = Chip_Clock_GetSystemClockRate();

	/* Timer setup for match and interrupt at TICKRATE_HZ */
	Chip_TIMER_Reset(LPC_TIMER32_0);
	Chip_TIMER_Reset(LPC_TIMER16_0);

	/* Enable both timers to generate interrupts when time matches */
	Chip_TIMER_MatchEnableInt(LPC_TIMER32_0, 1);
	Chip_TIMER_MatchEnableInt(LPC_TIMER16_0, 1);

	/* Setup prescale value on 16-bit timer to extend range */
	Chip_TIMER_PrescaleSet(LPC_TIMER16_0, PRESCALE_HZ2);

	/* Setup 32-bit timer's duration (32-bit match time) */
	Chip_TIMER_SetMatch(LPC_TIMER32_0, 1, (timerFreq / TICKRATE_HZ1));

	/* Setup 16-bit timer's duration (16-bit match time) */
	Chip_TIMER_SetMatch(LPC_TIMER16_0, 1, (timerFreq / TICKRATE_HZ2) >> 16);

	/* Setup both timers to restart when match occurs */
	Chip_TIMER_ResetOnMatchEnable(LPC_TIMER32_0, 1);
	Chip_TIMER_ResetOnMatchEnable(LPC_TIMER16_0, 1);

	/* Start both timers */
	Chip_TIMER_Enable(LPC_TIMER32_0);
	Chip_TIMER_Enable(LPC_TIMER16_0);

	/* Clear both timers of any pending interrupts */
	NVIC_ClearPendingIRQ(TIMER_32_0_IRQn);
	NVIC_ClearPendingIRQ(TIMER_16_0_IRQn);

	/* Enable both timer interrupts */
	NVIC_EnableIRQ(TIMER_32_0_IRQn);
	NVIC_EnableIRQ(TIMER_16_0_IRQn);

	/* Wait for timers to generate interrupts (LEDs toggle in interrupt handlers) */
	while (1) {
		__WFI();
	}

	return 0;
}
