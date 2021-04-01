/*
 * @brief DMA with ADC example
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

#include <stdlib.h>
#include <string.h>
#include "board.h"
#include "stopwatch.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

#if defined(BOARD_MANLEY_11U68)
#define BOARD_ADC_CH 8

#elif defined(BOARD_NXP_LPCXPRESSO_11U68)
#define BOARD_ADC_CH 0

#else
#warning "Using ADC channel 8 for this example, please select for your board"
#define BOARD_ADC_CH 8
#endif

/* Number of ADC samples per burst */
#define ADCBURSTSAMPLES 16

/* ADC sample storage area (samples x 2 buffers) */
static uint32_t adcBuff[2][ADCBURSTSAMPLES];

/* DMA descriptors must be aligned to 16 bytes */
#if defined(__CC_ARM)
__align(16) static DMA_CHDESC_T dmaDesc[2][ADCBURSTSAMPLES];
#endif /* defined (__CC_ARM) */

/* IAR support */
#if defined(__ICCARM__)
#pragma data_alignment=16
static DMA_CHDESC_T dmaDesc[2][ADCBURSTSAMPLES];
#endif /* defined (__ICCARM__) */

#if defined( __GNUC__ )
static DMA_CHDESC_T dmaDesc[2][ADCBURSTSAMPLES]  __attribute__ ((aligned(16)));
#endif /* defined (__GNUC__) */

static volatile int curCh, lastCh;
static volatile bool dmaerr;
static volatile uint32_t samplesCount1K;

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/* Setup ADC channel(s) and sequencer A */
static void adcSetup(void)
{
	/* Setup ADC for 12-bit mode and normal power */
	Chip_ADC_Init(LPC_ADC, 0);

	/* Setup for maximum ADC clock rate. This is the ADC rate, not the sample
	   rate. If you go too fast, the DMA interrupt will comsume all the
	   CPU cycles and the DMA descriptor handling will consume almost all
	   memory bandwidth.	*/
	Chip_ADC_SetClockRate(LPC_ADC, 250000);

	/* Setup sequencer A for ADC channel 0, EOS interrupt */
#if defined(BOARD_MANLEY_11U68)
	/* Setup a sequencer to do the following:
	   Perform continuous ADC conversion of ADC channel 8 only.
	   Trigger ADC int on end of conversion. */
	Chip_ADC_SetupSequencer(LPC_ADC, ADC_SEQA_IDX,
							(ADC_SEQ_CTRL_CHANSEL(BOARD_ADC_CH) | ADC_SEQ_CTRL_BURST));

	/* ADC input 8 is on PIO0_12 mapped to FUNC2 */
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 12, (IOCON_FUNC3 | IOCON_MODE_INACT |
											IOCON_ADMODE_EN));

	/* Use higher voltage trim */
	Chip_ADC_SetTrim(LPC_ADC, ADC_TRIM_VRANGE_HIGHV);

#elif defined(BOARD_NXP_LPCXPRESSO_11U68)
	/* Setup a sequencer to do the following:
	   Perform continuous ADC conversion of ADC channel 0 only.
	   Trigger ADC int on end of conversion. */
	Chip_ADC_SetupSequencer(LPC_ADC, ADC_SEQA_IDX,
							(ADC_SEQ_CTRL_CHANSEL(BOARD_ADC_CH) | ADC_SEQ_CTRL_BURST));

	/* ADC input 0 is on PIO0_12 mapped to FUNC2 */
	Chip_IOCON_PinMuxSet(LPC_IOCON, 1, 9, (IOCON_FUNC1 | IOCON_MODE_INACT |
										   IOCON_ADMODE_EN));

	/* Use higher voltage trim */
	Chip_ADC_SetTrim(LPC_ADC, ADC_TRIM_VRANGE_HIGHV);

#else
#warning "No ADC setup for this example"
#endif

	/* Need to do a calibration after initialization and trim */
	Chip_ADC_StartCalibration(LPC_ADC);
	while (!(Chip_ADC_IsCalibrationDone(LPC_ADC))) {}

	/* Clear all pending interrupts */
	Chip_ADC_ClearFlags(LPC_ADC, Chip_ADC_GetFlags(LPC_ADC));

	/* Enable ADC sequence A interrupt (needed for DMA). No point in enabling
	   other ADC interrupts when used with DMA trigger. */
	Chip_ADC_EnableInt(LPC_ADC, ADC_INTEN_SEQA_ENABLE);

	/* Enable sequencer */
	Chip_ADC_EnableSequencer(LPC_ADC, ADC_SEQA_IDX);
}

/* Setup DMA for ADC services with ADC seqeuncer A trigger */
static void dmaSetup(void)
{
	int i, j;

	/* DMA initialization - enable DMA clocking and reset DMA if needed */
	Chip_DMA_Init(LPC_DMA);

	/* Enable DMA controller and use driver provided DMA table for
	   current descriptors */
	Chip_DMA_Enable(LPC_DMA);
	Chip_DMA_SetSRAMBase(LPC_DMA, DMA_ADDR(Chip_DMA_Table));

	/* Use ADC sequencer A interrupt for DMA trigger */
	Chip_DMA_SetHWTrigger(LPC_DMATRIGMUX, DMA_CH0, DMATRIG_ADC0_SEQA_IRQ);

	/* Setup channel 0 for the following configuration:
	   Hardware trigger on high edge */
	Chip_DMA_EnableChannel(LPC_DMA, DMA_CH0);
	Chip_DMA_EnableIntChannel(LPC_DMA, DMA_CH0);
	Chip_DMA_SetupChannelConfig(LPC_DMA, DMA_CH0,
								(DMA_CFG_HWTRIGEN | DMA_CFG_TRIGPOL_HIGH | DMA_CFG_TRIGTYPE_EDGE));

	/* Setup descriptor list for buffer 1. A single descriptor is needed for
	   each ADC sample read in this example. A more complex example can be
	     setup using all ADC channels with a single descriptor if needed. */
	for (i = 0; i < 2; i++) {
		/* Setup transfer descriptor common to all DMA descriptors for this buffer.
		   The transfer will reload the next descriptor in the list when done and
		   generate a single DMA interrupt at the completion of the list. */
		dmaDesc[i][0].xfercfg =
			(DMA_XFERCFG_CFGVALID | DMA_XFERCFG_RELOAD |
			 DMA_XFERCFG_CLRTRIG | DMA_XFERCFG_WIDTH_32 | DMA_XFERCFG_SRCINC_0 |
			 DMA_XFERCFG_DSTINC_1 | DMA_XFERCFG_XFERCOUNT(1));

		/* Setup full buffer dewscriptor list */
		for (j = 0; j < ADCBURSTSAMPLES; j++) {
			dmaDesc[i][j].source = DMA_ADDR(&LPC_ADC->SEQ_GDAT[ADC_SEQA_IDX]) + 3;
			dmaDesc[i][j].dest = DMA_ADDR(&adcBuff[i][j]) + 3;

			if (j == (ADCBURSTSAMPLES - 1)) {
				/* DMA interrupt on last descriptor */
				dmaDesc[i][j].xfercfg = dmaDesc[i][0].xfercfg | DMA_XFERCFG_SETINTA;
				dmaDesc[i][j].next = DMA_ADDR(&dmaDesc[1 - i][0]);
			}
			else {
				dmaDesc[i][j].xfercfg = dmaDesc[i][0].xfercfg;
				/* Link to next descriptor */
				dmaDesc[i][j].next = DMA_ADDR(&dmaDesc[i][j + 1]);
			}
		}
	}

	/* Setup transfer descriptor and validate it */
	Chip_DMA_SetupTranChannel(LPC_DMA, DMA_CH0, &dmaDesc[0][0]);
	Chip_DMA_SetValidChannel(LPC_DMA, DMA_CH0);

	/* Setup data transfer */
	Chip_DMA_SetupChannelTransfer(LPC_DMA, DMA_CH0, dmaDesc[0][0].xfercfg);
}

/* Average and display sample data */
static void averageSamples(uint32_t *samples, int numSamples)
{
	int i, sampIdx = 0;
	uint32_t overrunSamples = 0, sample = 0;

	for (i = 0; i < numSamples; i++) {
		/* Verify no ADC overruns occurred in any samples */
		if ((samples[i] & ADC_SEQ_GDAT_OVERRUN) != 0) {
			overrunSamples++;
		}
		else {
			/* Justify ADC sample and strip off status bits */
			sample += (uint32_t) ADC_DR_RESULT(samples[i]);
			sampIdx++;
		}
	}

	if (sampIdx > 0) {
		DEBUGOUT("Average_lastDMA = %04x (Total samples = %08d)\r",
				 (sample / sampIdx), (samplesCount1K * 1000 * ADCBURSTSAMPLES));
	}

	/* Overrun on any samples? */
	if (overrunSamples > 0) {
		DEBUGOUT("\nADC overrun (%02d sample(s))\r",  overrunSamples);
	}
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/**
 * @brief	DMA Interrupt Handler
 * @return	None
 */
void DMA_IRQHandler(void)
{
	static int samplesCount = 0;

	/* Error interrupt on channel 0? */
	if ((Chip_DMA_GetIntStatus(LPC_DMA) & DMA_INTSTAT_ACTIVEERRINT) != 0) {
		/* This shouldn't normally happen, so set the LED to indicate an
		   error occurred. Fix the error in the background and clear the
		   LED. If the LED is lit often, something is wrong. */
		Board_LED_Set(0, true);
		NVIC_DisableIRQ(DMA_IRQn);
		dmaerr = true;
	}
	else {
		/* Clear DMA interrupt for the channel */
		Chip_DMA_ClearActiveIntAChannel(LPC_DMA, DMA_CH0);

		/* Tell background a new buffer is ready */
		curCh = 1 - curCh;

		/* Toggle LED 1 every 1K DMA blocks */
		samplesCount++;
		if (samplesCount > 1000) {
			samplesCount = 0;
			samplesCount1K++;
			Board_LED_Toggle(1);
		}
	}
}

/**
 * @brief	Main program body
 * @return	int
 */
int main(void)
{
	uint32_t startTime, lastSsamplesCount1K = 0;

	/* Setup SystemCoreClock and any needed board code */
	SystemCoreClockUpdate();
	Board_Init();

	/* Setup ADC and DMA as used in this example */
	adcSetup();
	dmaSetup();

	/* Enable DMA NVIC interrupt */
	NVIC_EnableIRQ(DMA_IRQn);

	curCh = lastCh = 1;

	/* Time ADC capture rate with DMA */
	StopWatch_Init();
	startTime = StopWatch_Start();

	/* Start continuous ADC conversion using sequence A. This never stops
	   and the ADC interrupts aren't used. The DMA controller will trigger
	   an interrupt when enough ADC data is received. */
	Chip_ADC_StartSequencer(LPC_ADC, ADC_SEQA_IDX);

	/* Sleep while waiting for DMA interrupt. The DMA interrupt is
	   used to wakeup the chip and handle the current ADC data received
	   via DMA. ADC processing occurs while the MCU is asleep. */
	while (1) {
		__WFI();

		/* If an error occured, clear it and restart DMA and ADC */
		if (dmaerr) {
			DEBUGSTR("\r\nDMA error has occurred.\r\n");

			/* Reset DMA channel */
			Chip_DMA_DisableChannel(LPC_DMA, DMA_CH0);
			while ((Chip_DMA_GetBusyChannels(LPC_DMA) & (1 << DMA_CH0)) != 0) {}
			Chip_DMA_AbortChannel(LPC_DMA, DMA_CH0);
			Chip_DMA_ClearErrorIntChannel(LPC_DMA, DMA_CH0);
			dmaSetup();
			Board_LED_Set(0, false);
			dmaerr = false;
			NVIC_EnableIRQ(DMA_IRQn);

			/* The ADC will halt burst mode on this error case, so manually pop
			     the ADC data register to clear overrun and restart burst mode */
			Chip_ADC_GetSequencerDataReg(LPC_ADC, ADC_SEQA_IDX);
		}
		else if (curCh != lastCh) {
			lastCh = lastCh;

			/* Process last received buffer */
			averageSamples(&adcBuff[curCh][0], ADCBURSTSAMPLES);

			/* Get an estimated ADC samlpe rate */
			if (lastSsamplesCount1K != samplesCount1K) {
				uint32_t ms = StopWatch_TicksToMs(StopWatch_Elapsed(startTime));

				lastSsamplesCount1K = samplesCount1K;

				/* Restart stopwatch */
				startTime = StopWatch_Start();

				DEBUGOUT("\n%d samples in %d mSecs\r\n", (1000 * ADCBURSTSAMPLES),
						 ms);
			}
		}
	}

	return 0;
}
