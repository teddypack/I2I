/*
 * @brief Manley LCD demo
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
#include "stopwatch.h"
#include "font_render.h"
#include "lpc_winfreesystem14x16.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

#if !defined(BOARD_MANLEY_11U68)
#error "This demo is the for the Manley LPC11U68 board only"
#endif /* defined (BOARD_MANLEY_11U68) */

/* Red color component, intensitory 0 to 0x1F */
#define RED(inten)      ((inten) << 11)

/* Green color component, intensitory 0 to 0x3F */
#define GREEN(inten)    ((inten) << 5)

/* Blue color component, intensitory 0 to 0x1F */
#define BLUE(inten)     (inten)

static uint16_t xMax, yMax;

static const uint16_t rgb[3] = {RED(0x1F), GREEN(0x3F), BLUE(0x1F)};
static const uint16_t rgboff[3] = {0x1F, 0x3F, 0x1F};
static const uint16_t rgbidx[3] = {RED(1), GREEN(1), BLUE(1)};

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/
extern uint8_t image8[];
#define BMPWIDTH           128
#define BMPHEIGHT          47

/*****************************************************************************
 * Private functions
 ****************************************************************************/

static void dispExample(void)
{
	uint16_t x, y;
	uint16_t colr, regn, regnStart, xC, yC;
	int i, jMax;

	/* Get display resolution */
	Board_LCD_GetLCDSize(&xMax, &yMax);
	regn = xMax / 3;

	/* Red, green, and blue boxes */
	Board_LCD_FillRect(0, 0, regn, yMax, RED(0x1F));
	Board_LCD_FillRect(regn, 0, regn, yMax, GREEN(0x3F));
	Board_LCD_FillRect(regn << 1, 0, xMax - (regn << 1), yMax, BLUE(0x1F));

	/* Gradients with windows */
	regnStart = 0;
	for (i = 0; i < 3; i++) {
		jMax = (yMax / rgboff[i]) + 1;
		colr = 0;
		Board_LCD_SetWindowSize(regnStart, 0, regn, yMax);

		for (y = 0; y < yMax; y++) {
			for (x = 0; x < regn; x++) {
				Board_LCD_SetPixel(colr);
			}

			if (((y + 1) % jMax) == 0) {
				colr += rgbidx[i];
			}
		}

		if (i == 1) {
			regnStart = regn << 1;
			regn = xMax - regnStart;
		}
		else {
			regnStart += regn;
		}
	}
	StopWatch_DelayMs(1000);

	/* Boxes from the inside out */
	xC = xMax / 2;
	yC = yMax / 2;
	for (i = 0; i < 14; i++) {
		Board_LCD_FillRect((xC - (5 * i)), (yC - (5 * i)), (10 * i), (10 * i), rgb[i % 3]);
		StopWatch_DelayMs(200);
	}

	StopWatch_DelayMs(800);
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/**
 * @brief    main routine for example
 * @return    Function should not exit
 */
int main(void)
{
	SystemCoreClockUpdate();
	Board_Init();

	/* Initialize stopwatch */
	StopWatch_Init();

	/* Setup display for portrait mode and run demo */
	Board_LCD_Init(true);
	dispExample();

	/* Setup display for landscape mode and run demo */
	Board_LCD_Init(false);
	dispExample();

	Board_LCD_Clear(0xFFFF);

	/* Get display resolution */
	Board_LCD_GetLCDSize(&xMax, &yMax);

	/* NXP logo in center of display */
	Board_LCD_CopyImage(image8, xMax - BMPWIDTH - 10,
						yMax - BMPHEIGHT - 10, BMPWIDTH, BMPHEIGHT);

	LCD_SetFontColor(0xFFFF);
	LCD_SetFontBgColor(BLUE(0x1F));	/* White background */
	LCD_SetFont(&font_winfreesys14x16);
	LCD_SetFontCharSpace(3);

	LCD_PutStrXY(2, 0, "*************************************");
	LCD_PutStrXY(3, 18, "*********   LPC11U6X   ***********");

	LCD_SetFontCharSpace(1);
	LCD_SetFontBgColor(0xFFFF);
	LCD_SetFontColor(BLUE(0x1F));	/* White background */
	LCD_PutStrXY(85, 66, "MANLEY LCD DEMO");

	/* Demo is done, endless loop */
	while (1) {
		__WFI();
	}

	return 0;
}
