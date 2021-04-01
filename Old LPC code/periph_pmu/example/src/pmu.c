/*
 * @brief PMU example
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

/* Change this value to increase/decrease the time between power state changes */
#define POWER_CYCLE_SEC_DELAY 4

/* Index of PMU GP registers */
#define PWR_CYCLE_COUNT_REG_INDEX 0

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/* Read or reset the power-cycle counter */
static uint32_t ProcessCycleCounter(void)
{
	uint32_t returnVal;

	/* Read current power-cycle count register */
	returnVal = Chip_PMU_ReadGPREG(LPC_PMU, PWR_CYCLE_COUNT_REG_INDEX);

	/* Write power-cycle count register */
	Chip_PMU_WriteGPREG(LPC_PMU, PWR_CYCLE_COUNT_REG_INDEX, returnVal + 1);

	/* Return current value of cycle-count */
	return returnVal;
}

/* Delay to allow all serial output to be processed before power state change */
static void DelayForSerialOutput(void)
{
	volatile uint32_t tempTimeout;

	/* Delay until all serial processing complete */
	tempTimeout = Chip_RTC_GetCount(LPC_RTC) + 1;
	while (Chip_RTC_GetCount(LPC_RTC) < tempTimeout) {}
}

/* Handle interrupt from GPIO pin or GPIO pin mapped to PININT */
static void ProcessPowerState(CHIP_PMU_MCUPOWER_T crntPowerSetting)
{
	volatile uint32_t tempTimeout;

	/* Output power status message, add separating space */
	DEBUGSTR("\r\n");

	/* Switch on current selected power setting */
	switch (crntPowerSetting) {
	case PMU_MCU_SLEEP:
	default:
		DEBUGSTR("-----------------------------------------------------------------\r\n");
		DEBUGSTR("     Entering SLEEP power setting\r\n");
		DEBUGOUT("       (System will exit SLEEP in %d seconds)\r\n", POWER_CYCLE_SEC_DELAY);
		DEBUGSTR("-----------------------------------------------------------------\r\n\r\n");

		/* Wait for all serial characters to be output */
		DelayForSerialOutput();

		/* Enter MCU Sleep mode */
		Chip_PMU_SleepState(LPC_PMU);

		break;

	case PMU_MCU_DEEP_SLEEP:
		DEBUGSTR("-----------------------------------------------------------------\r\n");
		DEBUGSTR("     Entering DEEP SLEEP power setting\r\n");
		DEBUGOUT("       (System will exit DEEP SLEEP in %d seconds)\r\n", POWER_CYCLE_SEC_DELAY);
		DEBUGSTR("-----------------------------------------------------------------\r\n\r\n");

		/* Wait for all serial characters to be output */
		DelayForSerialOutput();

		/* We can optionally call Chip_SYSCTL_SetDeepSleepPD() to power down the
		   BOD and WDT if we aren't using them in deep sleep modes. */
		Chip_SYSCTL_SetDeepSleepPD(SYSCTL_DEEPSLP_BOD_PD | SYSCTL_DEEPSLP_WDTOSC_PD);

		/* We should call Chip_SYSCTL_SetWakeup() to setup any peripherals we want
		   to power back up on wakeup. For this example, we'll power back up the IRC,
		   FLASH, the system oscillator, and the PLL */
		Chip_SYSCTL_SetWakeup(~(SYSCTL_SLPWAKE_IRCOUT_PD | SYSCTL_SLPWAKE_IRC_PD |
								SYSCTL_SLPWAKE_FLASH_PD | SYSCTL_SLPWAKE_SYSOSC_PD | SYSCTL_SLPWAKE_SYSPLL_PD));

		/* Enter MCU Deep Sleep mode */
		Chip_PMU_DeepSleepState(LPC_PMU);

		/* Power anything back up here that isn't powered up on wakeup. The example
		   code below powers back up the BOD and WDT oscillator, which weren't setup to
		   power up in the Chip_SYSCTL_SetWakeup() function. */
		Chip_SYSCTL_SetDeepSleepPD(0);

		break;

	case PMU_MCU_POWER_DOWN:
		DEBUGSTR("-----------------------------------------------------------------\r\n");
		DEBUGSTR("     Entering POWER DOWN power setting\r\n");
		DEBUGOUT("       (System will exit POWER DOWN in %d seconds)\r\n", POWER_CYCLE_SEC_DELAY);
		DEBUGSTR("-----------------------------------------------------------------\r\n\r\n");

		/* Wait for all serial characters to be output */
		DelayForSerialOutput();

		/* We can optionally call Chip_SYSCTL_SetDeepSleepPD() to power down the
		   BOD and WDT if we aren't using them in deep sleep modes. */
		Chip_SYSCTL_SetDeepSleepPD(SYSCTL_DEEPSLP_BOD_PD | SYSCTL_DEEPSLP_WDTOSC_PD);

		/* We should call Chip_SYSCTL_SetWakeup() to setup any peripherals we want
		   to power back up on wakeup. For this example, we'll power back up the IRC,
		   FLASH, the system oscillator, and the PLL */
		Chip_SYSCTL_SetWakeup(~(SYSCTL_SLPWAKE_IRCOUT_PD | SYSCTL_SLPWAKE_IRC_PD |
								SYSCTL_SLPWAKE_FLASH_PD | SYSCTL_SLPWAKE_SYSOSC_PD | SYSCTL_SLPWAKE_SYSPLL_PD));

		/* Enter MCU Power down mode */
		Chip_PMU_PowerDownState(LPC_PMU);

		break;

	case PMU_MCU_DEEP_PWRDOWN:
		DEBUGSTR("-----------------------------------------------------------------\r\n");
		DEBUGSTR("     Entering DEEP POWER DOWN power setting\r\n");
		DEBUGOUT("       (System will exit DEEP POWER DOWN in %d seconds)\r\n", POWER_CYCLE_SEC_DELAY);
		DEBUGSTR("-----------------------------------------------------------------\r\n\r\n");

		/* Wait for all serial characters to be output */
		DelayForSerialOutput();

		/* Enter MCU Deep Power down mode - chip will reset on wakeup */
		Chip_PMU_DeepPowerDownState(LPC_PMU);

		break;
	}
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/**
 * @brief	RealTimeClock (RTC) Interrupt Handler
 * @return	None
 */
void RTC_IRQHandler(void)
{
	uint32_t rtcStatus;

	/* Get RTC status register */
	rtcStatus = Chip_RTC_GetStatus(LPC_RTC);

	/* Clear only latched RTC status */
	Chip_RTC_EnableOptions(LPC_RTC,
						   (rtcStatus & (RTC_CTRL_WAKE1KHZ | RTC_CTRL_ALARM1HZ)));
}

/**
 * @brief	Main program body
 * @return	int
 */
int main(void)
{
	CHIP_PMU_MCUPOWER_T crntPowerSetting;

	/* Setup SystemCoreClock and any needed board code */
	SystemCoreClockUpdate();
	Board_Init();

	/* Enable the RTC oscillator, oscillator rate can be determined by
	   calling Chip_Clock_GetRTCOscRate() */
	Chip_Clock_EnableRTCOsc();

	/* Initialize RTC driver (enables RTC clocking) */
	Chip_RTC_Init(LPC_RTC);

	/* Enable RTC as a peripheral wakeup event */
	Chip_SYSCTL_EnablePeriphWakeup(SYSCTL_WAKEUP_RTCINT);

	/* RTC reset */
	Chip_RTC_Reset(LPC_RTC);

	/* Start RTC at a count of 0 when RTC is disabled. If the RTC is enabled, you
	   need to disable it before setting the initial RTC count. */
	Chip_RTC_Disable(LPC_RTC);
	Chip_RTC_SetCount(LPC_RTC, 0);

	/* Set a long alarm time so the interrupt won't trigger */
	Chip_RTC_SetAlarm(LPC_RTC, 1000);

	/* Enable RTC and high resolution timer - this can be done in a single
	   call with Chip_RTC_EnableOptions(LPC_RTC, (RTC_CTRL_RTC1KHZ_EN | RTC_CTRL_RTC_EN)); */
	Chip_RTC_Enable1KHZ(LPC_RTC);
	Chip_RTC_Enable(LPC_RTC);

	/* Clear latched RTC interrupt statuses */
	Chip_RTC_ClearStatus(LPC_RTC, (RTC_CTRL_OFD | RTC_CTRL_ALARM1HZ | RTC_CTRL_WAKE1KHZ));

	/* Enable RTC interrupt */
	NVIC_EnableIRQ(RTC_IRQn);

	/* Enable RTC alarm interrupt */
	Chip_RTC_EnableWakeup(LPC_RTC, (RTC_CTRL_ALARMDPD_EN | RTC_CTRL_WAKEDPD_EN));

	/* Output example's activity banner */
	DEBUGSTR("\r\n");
	DEBUGSTR("-----------------------------------------------------------------\r\n");
	DEBUGOUT("Power Control Example   Cycle Count: %d\r\n", ProcessCycleCounter());
	DEBUGSTR("  System will cycle through SLEEP, DEEP SLEEP, POWER\r\n");
	DEBUGSTR("  DOWN, and DEEP POWER DOWN power states\r\n");
	DEBUGSTR("-----------------------------------------------------------------\r\n\r\n");

	/* Setup alarm, process next power state then wait for alarm to wake-up system */
	crntPowerSetting = PMU_MCU_SLEEP;
	while (1) {
		/* Set alarm to wakeup in POWER_CYCLE_SEC_DELAY seconds */
		Chip_RTC_SetAlarm(LPC_RTC, Chip_RTC_GetCount(LPC_RTC) + POWER_CYCLE_SEC_DELAY);

		/* Enter first (or next) power state */
		ProcessPowerState(crntPowerSetting);

		/* Inc current power setting and test for overflow */
		if (crntPowerSetting == PMU_MCU_DEEP_PWRDOWN) {
			/* Reset to lowest power setting */
			crntPowerSetting = PMU_MCU_SLEEP;
		}
		else {
			crntPowerSetting++;
		}
	}

	return 0;
}
