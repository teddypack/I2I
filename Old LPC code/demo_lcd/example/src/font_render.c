/*
 * @brief Manley LCD font render code
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
#include "font_render.h"

static struct {
	int fcolor;
	int bgcolor;
	int spacing;
	int fwidth;
	const FONT_T *font;
} cfont, *cf = &cfont;

/* Get the width of a given char index (in pixels) */
static int LCD_GetCharWidth(int index)
{
	if (cf->font->font_width_table) {
		return cf->font->font_width_table[index];
	}
	else {
		return cf->fwidth;
	}
}

/* Sets the foreground color of the font */
void LCD_SetFontColor(int color)
{
	cf->fcolor = color;
}

/* Set the background color of the font */
void LCD_SetFontBgColor(int color)
{
	cf->bgcolor = color;
}

/* Set width of the font for fixed width fonts */
void LCD_SetFontWidth(int width)
{
	cf->fwidth = width;
}

/* Sets the current font */
void LCD_SetFont(const FONT_T *font)
{
	cf->font = font;
}

/* Sets the space between two characters */
void LCD_SetFontCharSpace(int space)
{
	cf->spacing = space;
}

/* Prints an ASCII character at a given position */
uint32_t LCD_PutCharXY(int xPos, int yPos, int ch)
{
	uint8_t cmd = 0x2C;
	int w, h, r, c;
	uint8_t buf[36];
	uint16_t *fp;

	if (!cf->font) {
		return xPos | (yPos << 16);
	}
	if ((ch < cf->font->first_char) || (ch > cf->font->last_char)) {
		return xPos | (yPos << 16);
	}
	ch -= cf->font->first_char;
	w = LCD_GetCharWidth(ch) + cf->spacing;
	h = cf->font->font_height;
	fp = cf->font->font_table + (ch * h);
	Board_LCD_SetWindowSize(xPos, yPos, w, h);
	for (r = 0; r < h; r++, fp++) {
		uint16_t t = 0x8000;
		for (c = 0; c < (w << 1); c += 2, t >>= 1) {
			uint16_t clr = (*fp & t) ? cf->fcolor : cf->bgcolor;
			buf[c] = clr >> 8;
			buf[c + 1] = clr & 0xFF;
		}
		Board_LCD_WriteData(cmd, buf, w << 1);
		cmd = 0x3C;
	}
	return (xPos + w) | ((yPos + h) << 16);
}

/* Prints and ASCII string at a given position */
void LCD_PutStrXY(int xPos, int yPos, const char *str)
{
	uint32_t xp = xPos;
	while (str && *str) {
		xp = LCD_PutCharXY(xp & 0xFFFF, yPos, *str++);
	}
}
