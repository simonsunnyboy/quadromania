/*
 * Quadromania
 * (c) 2002/2003/2009/2010 by Matthias Arndt <marndt@asmsoftware.de> / ASM Software
 *
 * File: graphics.c - implements the graphics API
 * last Modified: 23.01.2010 : 18:23
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

static Uint16 frame_width, frame_height, dot_width, dot_height, texture_width,
		texture_height, font_height;

/* texture the complete screen with 1 out of 10 textures... */
void Graphics_DrawBackground(SDL_Surface *screen, Uint8 texture)
{
	Uint8 i, j;
	SDL_Rect src, dest;

	const Uint8 x_blits = (SCREEN_WIDTH / texture_width);
	const Uint8 y_blits = (SCREEN_HEIGHT / texture_height);

	/* draw the textured background... */
	for (j = 0; j < y_blits; j++)
		for (i = 0; i < x_blits; i++)
		{
			src.x = (texture % NR_OF_TEXTURES) * texture_width;
			src.y = 0;
			src.w = texture_width;
			src.h = texture_height;
			dest.x = i * texture_width;
			dest.y = j * texture_height;
			dest.w = 0;
			dest.h = 0;
			SDL_BlitSurface(textures, &src, screen, &dest);
		}
}

/* draw one of the coloured dots for the playfield... */
void Graphics_DrawDot(SDL_Surface *screen, Uint16 x, Uint16 y, Uint8 number)
{
	SDL_Rect src, dest;

	src.x = (number % NR_OF_DOTS) * dot_width;
	src.y = 0;
	src.w = dot_width;
	src.h = dot_height;
	dest.x = x;
	dest.y = y;
	dest.w = 0;
	dest.h = 0;
	SDL_BlitSurface(dots, &src, screen, &dest);
}

/* draw the titel string */
void Graphics_DrawTitle(SDL_Surface *screen)
{
	SDL_Rect src, dest;

	src.x = 0;
	src.y = 0;
	src.w = titel->w;
	src.h = titel->h;
	dest.x = ((SCREEN_WIDTH / 2) - (titel->w / 2));
	dest.y = dot_height;
	dest.w = 0;
	dest.h = 0;
	SDL_BlitSurface(titel, &src, screen, &dest);
	src.x = 0;
	src.y = 0;
	src.w = copyright->w;
	src.h = copyright->h;
	dest.x = ((SCREEN_WIDTH / 2) - (copyright->w / 2));
	dest.y = ((SCREEN_HEIGHT * 120) / 480);
	dest.w = 0;
	dest.h = 0;
	SDL_BlitSurface(copyright, &src, screen, &dest);

}

/* draws the instructions screen */
void Graphics_DrawInstructions(SDL_Surface *screen)
{

	const char *instructions_text[] =
	{
	 "Your task is to restore all stones to red." ,
	 "You select the amount of colours to use and" ,
	 "the amount of initial rotations." ,
	 "The computer will rotate a named amount of" ,
	 "3x3 tile sets and will flip their colours." ,
	 "The last possible color will turn red again." ,
	 "In game click on the center point of a 3x3 tile" ,
	 "set to exchange the tiles in the order." ,
	 "Restore the board with limited turns.",
	 NULL
	};
	const Uint16 instruction_y = ((SCREEN_HEIGHT * 120) / 480);
	Uint16 y;
	Uint8 i = 0;
	SDL_Rect src, dest;

	Graphics_DrawBackground(screen, 0);
	Graphics_DrawOuterFrame(screen);
	/* draw logo */
	src.x = 0;
	src.y = 0;
	src.w = titel->w;
	src.h = titel->h;
	dest.x = ((SCREEN_WIDTH / 2) - (titel->w / 2));
	dest.y = dot_height;
	dest.w = 0;
	dest.h = 0;
	SDL_BlitSurface(titel, &src, screen, &dest);
	XCenteredString(screen, instruction_y, "Instructions");
	i = 0;
	for (y = instruction_y + font_height; y < (SCREEN_HEIGHT - font_height); y
			= y + font_height)
	{
		if (instructions_text[i] == NULL)
		{
			break; /* stop drawing instruction text*/
		}
		Graphics_DrawText(screen, dot_width, y, (char *) instructions_text[i]);
		i++;
	}

	Graphics_DrawText(screen,((SCREEN_WIDTH * 380) / 640),(SCREEN_HEIGHT - font_height),"Click here to continue!");
	SDL_Flip(screen);
}

/* draw the outer frame... */
void Graphics_DrawOuterFrame(SDL_Surface *screen)
{
	SDL_Rect src, dest;

	src.x = 0;
	src.y = 0;
	src.w = frame_width;
	src.h = frame_height;
	dest.x = 0;
	dest.y = 0;
	dest.w = frame_width;
	dest.h = frame_height;
	SDL_BlitSurface(frame, &src, screen, &dest);
}

/* write some text with the default font on the screen... */
void Graphics_DrawText(SDL_Surface *screen, Uint16 x, Uint16 y, char *text)
{
	PutString(screen, x, y, text);
}

/* to be able to use the graphics module, initialize it first... */
void Graphics_Init()
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

	/* did our graphics load properly? (remember NULL pointers for surfaces means "no valid surface data loaded" */
	if((textures==NULL)||(frame==NULL)||(dots==NULL)||(titel==NULL)||(copyright==NULL))
	{
		fprintf (stderr, "%s initgraphics(): One or more image files failed to load properly!\n\n",PACKAGE);
		exit(2);
	}

	/* collect information of sizes of the various graphics */
	frame_width = (Uint16) frame->w;
	frame_height = (Uint16) frame->h;
	dot_width = (Uint16) (dots->w / NR_OF_DOTS);
	dot_height = (Uint16) (dots->h);
	texture_width = (Uint16) (textures->w / NR_OF_TEXTURES);
	texture_height = (Uint16) (textures->h);
	font_height = (Uint16) (font->h);

	atexit(Graphics_CleanUp);
}

/* at exit clean up the graphics module... */
void Graphics_CleanUp()
{
	SDL_FreeSurface(textures);
	SDL_FreeSurface(frame);
	SDL_FreeSurface(dots);
	SDL_FreeSurface(font);

#ifdef _DEBUG
	fprintf(stderr,"image surfaces successfully freed....\n");
#endif

}

/* get width of a dot tile */
Uint16 Graphics_GetDotWidth()
{
	return(dot_width);
}

/* get heigth of a dot tile */
Uint16 Graphics_GetDotHeight()
{
	return(dot_height);
}

/* get heigth of the main font */
Uint16 Graphics_GetFontHeight()
{
	return(font_height);
}
