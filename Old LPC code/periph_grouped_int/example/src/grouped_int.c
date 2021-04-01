/*
 * @brief Grouped GPIO Interrupt example
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

#if defined(BOARD_MANLEY_11U68)
/* These two inputs for GROUP GPIO Interrupt 0. */
#define LEFT_SWITCH_PORT    0
#define LEFT_SWITCH_BIT     7
#define UP_SWITCH_PORT      0
#define UP_SWITCH_BIT       22

/* These two inputs for GROUP GPIO Interrupt 1. */
#define RIGHT_SWITCH_PORT   1
#define RIGHT_SWITCH_BIT    23
#define DOWN_SWITCH_PORT    0
#define DOWN_SWITCH_BIT     23

#elif defined(BOARD_NXP_LPCXPRESSO_11U68)
/* These two inputs for GROUP GPIO Interrupt 0. */
#define LEFT_SWITCH_PORT    2
#define LEFT_SWITCH_BIT     19
#define UP_SWITCH_PORT      0
#define UP_SWITCH_BIT       21

/* These two inputs for GROUP GPIO Interrupt 1. */
#define RIGHT_SWITCH_PORT   0
#define RIGHT_SWITCH_BIT    20
#define DOWN_SWITCH_PORT    2
#define DOWN_SWITCH_BIT     2

#else
#error "Grouped GPIO Interrupt not configured for this example"
#endif /* defined (BOARD_MANLEY_11U68) */

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
 * @brief	Handle Group GPIO 0 interrupt
 * @return	Nothing
 */
void GINT0_IRQHandler(void)
{
	Chip_GPIOGP_ClearIntStatus(LPC_GPIOGROUP, 0);
	Board_LED_Set(0, true);
}

/**
 * @brief	Handle Group GPIO 1 interrupt
 * @return	Nothing
 */
void GINT1_IRQHandler(void)
{
	Chip_GPIOGP_ClearIntStatus(LPC_GPIOGROUP, 1);
	Board_LED_Set(0, false);
}

/**
 * @brief	Main program body
 * @return	Does not return
 */
int main(void)
{
	/* Generic Initialization */
	SystemCoreClockUpdate();

	/* Board_Init calls Chip_GPIO_Init and enables GPIO clock if needed,
	   Chip_GPIO_Init is not called again */
	Board_Init();
	Board_LED_Set(0, false);

	/* Group GPIO interrupt 0 will be invoked when both the joystick up
	   switch and left switch are pressed. */
	Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_GROUP0INT);
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, LEFT_SWITCH_PORT, LEFT_SWITCH_BIT);
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, UP_SWITCH_PORT, UP_SWITCH_BIT);
	Chip_GPIOGP_SelectLowLevel(LPC_GPIOGROUP, 0, LEFT_SWITCH_PORT, 1 << LEFT_SWITCH_BIT);
	Chip_GPIOGP_SelectLowLevel(LPC_GPIOGROUP, 0, UP_SWITCH_PORT, 1 << UP_SWITCH_BIT);
	Chip_GPIOGP_EnableGroupPins(LPC_GPIOGROUP, 0, LEFT_SWITCH_PORT, 1 << LEFT_SWITCH_BIT);
	Chip_GPIOGP_EnableGroupPins(LPC_GPIOGROUP, 0, UP_SWITCH_PORT, 1 << UP_SWITCH_BIT);
	Chip_GPIOGP_SelectAndMode(LPC_GPIOGROUP, 0);
	Chip_GPIOGP_SelectEdgeMode(LPC_GPIOGROUP, 0);

	/* Enable Group GPIO interrupt 0 */
	NVIC_EnableIRQ(GINT0_IRQn);

	/* Group GPIO interrupt 1 will be invoked when both the joystick down
	   switch and the right switch are pressed. */
	Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_GROUP1INT);
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, RIGHT_SWITCH_PORT, RIGHT_SWITCH_BIT);
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, DOWN_SWITCH_PORT, DOWN_SWITCH_BIT);
	Chip_GPIOGP_SelectLowLevel(LPC_GPIOGROUP, 1, RIGHT_SWITCH_PORT, 1 << RIGHT_SWITCH_BIT);
	Chip_GPIOGP_SelectLowLevel(LPC_GPIOGROUP, 1, DOWN_SWITCH_PORT, 1 << DOWN_SWITCH_BIT);
	Chip_GPIOGP_EnableGroupPins(LPC_GPIOGROUP, 1, RIGHT_SWITCH_PORT, 1 << RIGHT_SWITCH_BIT);
	Chip_GPIOGP_EnableGroupPins(LPC_GPIOGROUP, 1, DOWN_SWITCH_PORT, 1 << DOWN_SWITCH_BIT);
	Chip_GPIOGP_SelectAndMode(LPC_GPIOGROUP, 1);
	Chip_GPIOGP_SelectEdgeMode(LPC_GPIOGROUP, 1);

	/* Enable Group GPIO interrupt 1 */
	NVIC_EnableIRQ(GINT1_IRQn);

	/* Stay in sleep mode.  All the work is done in ISRs. */
	while (1) {
		/* Go to sleep state - will wake up automatically on interrupt */
		Chip_PMU_SleepState(LPC_PMU);
	}

	/* Does not return */
	return 0;
}
