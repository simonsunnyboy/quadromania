/*
 * Quadromania
 * (c) 2002/2003/2009 by Matthias Arndt <marndt@asmsoftware.de> / ASM Software
 *
 * File: graphics.c - implements the graphics API
 * last Modified: 10.11.2009 : 19:22
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

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>

#include "graphics.h"
#include "SFont.h"

static SDL_Surface *textures, *frame, *dots, *font, *titel, *copyright;

/* texture the complete screen with 1 out of 10 textures... */
void drawbackground(SDL_Surface *screen, Uint8 texture)
{
	Uint8 i, j;
	SDL_Rect src, dest;

	/* draw the textured background... */
	for (j = 0; j < 6; j++)
		for (i = 0; i < 8; i++)
		{
			src.x = (texture % 10) * 80;
			src.y = 0;
			src.w = 80;
			src.h = 80;
			dest.x = i * 80;
			dest.y = j * 80;
			dest.w = 0;
			dest.h = 0;
			SDL_BlitSurface(textures, &src, screen, &dest);
		}
}

/* draw one of the coloured dots for the playfield... */
void drawdot(SDL_Surface *screen, Uint16 x, Uint16 y, Uint8 number)
{
	SDL_Rect src, dest;

	src.x = (number % 5) * 32;
	src.y = 0;
	src.w = 32;
	src.h = 32;
	dest.x = x;
	dest.y = y;
	dest.w = 0;
	dest.h = 0;
	SDL_BlitSurface(dots, &src, screen, &dest);
}

/* draw the titel string */
void drawtitel(SDL_Surface *screen)
{
	SDL_Rect src, dest;

	src.x = 0;
	src.y = 0;
	src.w = titel->w;
	src.h = titel->h;
	dest.x = ((screen->w / 2) - (titel->w / 2));
	dest.y = 32;
	dest.w = 0;
	dest.h = 0;
	SDL_BlitSurface(titel, &src, screen, &dest);
	src.x = 0;
	src.y = 0;
	src.w = copyright->w;
	src.h = copyright->h;
	dest.x = ((screen->w / 2) - (copyright->w / 2));
	dest.y = 120;
	dest.w = 0;
	dest.h = 0;
	SDL_BlitSurface(copyright, &src, screen, &dest);

}

/* draw the outer frame... */
void drawframe(SDL_Surface *screen)
{
	SDL_Rect src, dest;

	src.x = 0;
	src.y = 0;
	src.w = frame->w;
	src.h = frame->h;
	dest.x = 0;
	dest.y = 0;
	dest.w = frame->w;
	dest.h = frame->h;
	SDL_BlitSurface(frame, &src, screen, &dest);
}

/* write some text with the default font on the screen... */
void text(SDL_Surface *screen, Uint16 x, Uint16 y, char *text)
{
	PutString(screen, x, y, text);
}

/* to be able to use the graphics module, initialize it first... */
void initgraphics()
{
	/* FIXME: implement error handlers...*/
	textures = IMG_Load("data/texture.png");
	frame = IMG_Load("data/frame.png");
	dots = IMG_Load("data/dots.png");
	font = IMG_Load("data/font.png");
	titel = IMG_Load("data/titel.png");
	copyright = IMG_Load("data/copyright.png");

#ifdef _DEBUG
	fprintf(stderr,"images loaded....\n");
#endif
	InitFont(font);
#ifdef _DEBUG
	fprintf(stderr,"font ready...\n");
#endif
	atexit(exitgraphics);
}

/* at exit clean up the graphics module... */
void exitgraphics()
{
	SDL_FreeSurface(textures);
	SDL_FreeSurface(frame);
	SDL_FreeSurface(dots);
	SDL_FreeSurface(font);

#ifdef _DEBUG
	fprintf(stderr,"image surfaces successfully freed....\n");
#endif

}
