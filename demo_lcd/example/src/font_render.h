/*
 * @brief LPC11u6x Manley board LPC font renderer
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

#ifndef __FONT_RENDER_H_
#define __FONT_RENDER_H_

#include "lpc_fonts.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief	Sets the foreground color of font
 * @param	color	: Color in RGB565 format
 * @return	Nothing
 */
void LCD_SetFontColor(int color);

/**
 * @brief	Sets the background color of font
 * @param	color	: Color in RGB565 format
 * @return	Nothing
 */
void LCD_SetFontBgColor(int color);

/**
 * @brief	Sets the current font
 * @param	font	: Pointer to a font structure (swim font)
 * @return	Nothing
 */
void LCD_SetFont(const FONT_T *font);

/**
 * @brief	Sets the space between two chars
 * @param	space	: Space in pixels
 * @return	Nothing
 * @note	This functions sets the space that will
 * be left between chars in addition to the default
 * space provided by the font itself
 */
void LCD_SetFontCharSpace(int space);

/**
 * @brief	Sets width for fixed width fonts
 * @param	width	: Width of a single char in pixels
 * @return	Nothing
 * @note	This width is effective only for fonts
 * that has fixed with and having the width table
 * initialized to null
 */
void LCD_SetFontWidth(int width);

/**
 * @brief	Prints ASCII character at given position
 * @param	xPos	: x Position in pixels
 * @param	yPos	: y Position in pixels
 * @param	ch		: ASCII character (int casted)
 * @return	Next char's x Position in lower half word and
 * next char's y position in upper half word
 */
uint32_t LCD_PutCharXY(int xPos, int yPos, int ch);

/**
 * @brief	Prints ASCII string at given position
 * @param	xPos	: x Position in pixels
 * @param	yPos	: y Position in pixels
 * @param	str		: Pointer to NUL terminated ASCII string
 * @return	nothing
 */
void LCD_PutStrXY(int xPos, int yPos, const char *str);

#ifdef __cplusplus
}
#endif

#endif /* __FONT_RENDER_H_ */
