/*
 * Quadromania
 * (c) 2002/2003/2009/2010 by Matthias Arndt <marndt@asmsoftware.de> / ASM Software
 *
 * File: graphics.h - header file for the graphics API
 * last Modified: 23.01.2010 : 12:30
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 *
 * THIS SOFTWARE IS SUPPLIED AS IT IS WITHOUT ANY WARRANTY!
 *
 */
#ifndef __GRAPHICS_H
	#define __GRAPHICS_H

    #include "version.h"
	#include <SDL/SDL.h>

	/************
	 * DEFINES  *
	 ************/

	#define NR_OF_TEXTURES 	10
	#define NR_OF_DOTS     	5

	/**********
	 * MACROS *
	 **********/
	#define SCREEN_WIDTH 	(screen->w)
	#define SCREEN_HEIGHT 	(screen->h)

	/**************
	 * PROTOTYPES *
	 **************/

	void Graphics_DrawBackground(SDL_Surface *, Uint8);
	void Graphics_DrawDot(SDL_Surface *, Uint16, Uint16, Uint8);
	void Graphics_DrawOuterFrame(SDL_Surface *);
	void Graphics_DrawText(SDL_Surface *, Uint16, Uint16, char *);
	void Graphics_DrawTitle(SDL_Surface *);
	void Graphics_DrawInstructions(SDL_Surface *);
	void Graphics_Init(void);
	void Graphics_CleanUp(void);
	Uint16 Graphics_GetDotWidth(void);
	Uint16 Graphics_GetDotHeight(void);
	Uint16 Graphics_GetFontHeight(void);
#endif
