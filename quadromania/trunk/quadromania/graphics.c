/*
 * Quadromania
 * (c) 2002/2003/2009/2010 by Matthias Arndt <marndt@asmsoftware.de> / ASM Software
 *
 * File: graphics.c - implements the graphics API
 * last Modified: 13.06.2010 : 18:20
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
#include <string.h>

#include "graphics.h"
#include "SFont.h"
#include "highscore.h"

static SDL_Surface *screen;

static SDL_Surface *textures, *frame, *dots, *font, *titel, *copyright ,*window_icon;

static Uint16 frame_width, frame_height, dot_width, dot_height, texture_width,
		texture_height, font_height;

/* texture the complete screen with 1 out of 10 textures... */
void Graphics_DrawBackground(Uint8 texture)
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
void Graphics_DrawDot(Uint16 x, Uint16 y, Uint8 number)
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
void Graphics_DrawTitle()
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
void Graphics_DrawInstructions()
{
	const char *continue_msg = "Click here to continue!";
	const Uint16 instruction_y = ((SCREEN_HEIGHT * 120) / 480);

	SDL_Rect src, dest;
	SDL_Surface *instructions_gfx;

	instructions_gfx  = Graphics_LoadGraphicsResource("instructions.png");

	Graphics_DrawBackground(0);
	Graphics_DrawOuterFrame();
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
	/* draw instructions */
	src.x = 0;
	src.y = 0;
	src.w = instructions_gfx->w;
	src.h = instructions_gfx->h;
	dest.x = ((SCREEN_WIDTH / 2) - (instructions_gfx->w / 2));
	dest.y = instruction_y + (dot_height/2);
	dest.w = 0;
	dest.h = 0;
	SDL_BlitSurface(instructions_gfx, &src, screen, &dest);

	Graphics_DrawText((SCREEN_WIDTH - TextWidth((char *)continue_msg)),(SCREEN_HEIGHT - font_height),(char *)continue_msg);
	Graphics_UpdateScreen();

	SDL_FreeSurface(instructions_gfx);
}

/* draw given highscore table  */
void Graphics_ListHighscores(Uint16 nr_of_table)
{
	Uint16 i,y;
	char txt[30];
	SDL_Rect src,dest;
	const Uint16 highscore_y = ((SCREEN_HEIGHT * 120) / 480);

	HighscoreEntry *entry;

	Graphics_DrawBackground(1);
	Graphics_DrawOuterFrame();
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
	sprintf(txt,"High scores for Level %d",nr_of_table+1);
	XCenteredString(screen, highscore_y, txt);

	for(i = 0; i < HIGHSCORE_NR_OF_ENTRIES_PER_TABLE; i++)
	{
		entry=Highscore_GetEntry(nr_of_table, i);
#ifdef _DEBUG
		fprintf(stderr, "%s %d\n",entry->name, entry->score);
#endif

		y = highscore_y + ((i + 2) * font_height);

		sprintf(txt,"%s",entry->name);
		Graphics_DrawText(dot_width, y,txt);
		sprintf(txt,"%d",entry->score);
		Graphics_DrawText(((SCREEN_WIDTH * 3) / 4), y,txt);
	}

	Graphics_UpdateScreen();
}

/* draw the outer frame... */
void Graphics_DrawOuterFrame()
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
void Graphics_DrawText(Uint16 x, Uint16 y, char *text)
{
	PutString(screen, x, y, text);
}

/* show the "you have won!" message */
void Graphics_DrawWinMessage()
{
	const Uint16 factor = (SCREEN_WIDTH / 320);
	SDL_Rect base, dest;
	/* draw some message ...*/

	base.x = (SCREEN_WIDTH / 64);
	base.y = (SCREEN_HEIGHT / 2) - 20;
	base.w = (SCREEN_WIDTH - 2 * base.x);
	base.h = Graphics_GetFontHeight() + factor * 2;
	SDL_FillRect(screen, &base, 0);

	dest.x = base.x + factor;
	dest.y = base.y + factor;
	dest.w = base.w - 2 * factor;
	dest.h = base.h - 2 * factor;
	SDL_FillRect(screen, &dest, SDL_MapRGB(screen->format, 0, 64, 200));

	XCenteredString(screen,dest.y + factor,
			"Congratulations! You've won!");
	Graphics_UpdateScreen();
	return;
}

/* show the "you have lost" message */
void Graphics_DrawGameoverMessage()
{
	const Uint16 factor = (SCREEN_WIDTH / 320);
	SDL_Rect base, dest;
	/* draw some message ...*/

	base.x = (SCREEN_WIDTH / 64);
	base.y = (SCREEN_HEIGHT / 2) - 20;
	base.w = (SCREEN_WIDTH - 2 * base.x);
	base.h = Graphics_GetFontHeight() + factor * 2;
	SDL_FillRect(screen, &base, 0);

	dest.x = base.x + factor;
	dest.y = base.y + factor;
	dest.w = base.w - 2 * factor;
	dest.h = base.h - 2 * factor;
	SDL_FillRect(screen, &dest, SDL_MapRGB(screen->format, 200, 64, 0));

	XCenteredString(screen, dest.y + factor,
			"GAME OVER! You hit the turn limit!");
	SDL_Flip(screen);
	return;
}

/* to be able to use the graphics module, initialize it first... */
BOOLEAN Graphics_Init(BOOLEAN set_fullscreen)
{
	/* initialize SDL...  */
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		fprintf(stderr, "%s\n\nUnable to initialize SDL: %s\n", VERSION,
				SDL_GetError());
		return (FALSE);
	}
	/* make sure to shutdown SDL at program end... */
	atexit(SDL_Quit);

	Graphics_SetWindowIcon(); /* set window icon */

	/* Set an appropriate 16-bit video mode. */
#if(SCREENRES == _HIGH)
	if ((screen = SDL_SetVideoMode(640, 480, 16,
			((set_fullscreen == TRUE) ? SDL_FULLSCREEN : 0) | SDL_HWSURFACE
					| SDL_DOUBLEBUF)) == NULL)
	{
		fprintf(stderr, "%s\n\nUnable to set 640x480x16 video mode: %s\n",
				VERSION, SDL_GetError());
		return (FALSE);
	}
#elif(SCREENRES == _LOW)
	if ((screen = SDL_SetVideoMode(320, 240, 16, ((set_fullscreen == TRUE) ? SDL_FULLSCREEN
									: 0) | SDL_HWSURFACE | SDL_DOUBLEBUF)) == NULL)
	{
		fprintf(stderr, "%s\n\nUnable to set 320x240x16 video mode: %s\n",
				VERSION, SDL_GetError());
		return (FALSE);
	}
#else
#error screen resolution is not properly defined
	return(FALSE);
#endif

	/* set window title.. */
	SDL_WM_SetCaption(VERSION, NULL);

#if(HAVE_MOUSE_POINTER == 0)
	/* disable mouse pointer if configured */
	SDL_ShowCursor(SDL_DISABLE);
#endif

	/* init graphic data for use by the game */
	textures  = Graphics_LoadGraphicsResource("texture.png");
	frame     = Graphics_LoadGraphicsResource("frame.png");
	dots      = Graphics_LoadGraphicsResource("dots.png");
	font      = Graphics_LoadGraphicsResource("font.png");
	titel     = Graphics_LoadGraphicsResource("titel.png");
	copyright = Graphics_LoadGraphicsResource("copyright.png");

#ifdef _DEBUG
	fprintf(stderr,"images loaded....\n");
#endif
	InitFont(font);
#ifdef _DEBUG
	fprintf(stderr,"font ready...\n");
#endif

	/* did our graphics load properly? (remember NULL pointers for surfaces means "no valid surface data loaded" */
	if((textures==NULL)||(frame==NULL)||(dots==NULL)||(titel==NULL)||(copyright==NULL)||(font==NULL)||(window_icon==NULL))
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

	return(TRUE);
}

/* at exit clean up the graphics module... */
void Graphics_CleanUp()
{
	SDL_FreeSurface(textures);
	SDL_FreeSurface(frame);
	SDL_FreeSurface(dots);
	SDL_FreeSurface(font);
	SDL_FreeSurface(titel);
	SDL_FreeSurface(copyright);
	SDL_FreeSurface(window_icon);

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

/* get width of the screen */
Uint16 Graphics_GetScreenWidth()
{
	return(screen->w);
}

/* get heigth of the screen  */
Uint16 Graphics_GetScreenHeight()
{
	return(screen->h);
}

/* get heigth of the main font */
Uint16 Graphics_GetFontHeight()
{
	return(font_height);
}

/* load a graphics ressource from disk or flash */
SDL_Surface* Graphics_LoadGraphicsResource(char* inputfilename)
{
	char filename[255]; /* temporary filename buffer */
	if(strcmp("*ICON*", inputfilename) == 0)
	{
		sprintf ( filename, "data/icons/quadromania32.png"); /* construct filename */
	}
	else
	{
		sprintf ( filename, "data/%s%s", GFXPREFIX, inputfilename); /* construct filename */
	}
#ifdef _DEBUG
	fprintf(stderr,"%s\n", filename);
#endif
	return(IMG_Load(filename));
}

/* load and set the window icon */
void Graphics_SetWindowIcon()
{
	window_icon = Graphics_LoadGraphicsResource("*ICON*");
	SDL_WM_SetIcon(window_icon, NULL);
}

/* make all changes to the screen visible */
void Graphics_UpdateScreen()
{
	SDL_Flip(screen);
}
