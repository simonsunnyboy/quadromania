/*
 * Quadromania
 * (c) 2002/2003/2009/2010 by Matthias Arndt <marndt@asmsoftware.de> / ASM Software
 *
 * File: graphics.c - implements the graphics API
 * last Modified: 18.11.2010 : 19:12
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

/**
 * This function textures the complete screen with 1 out of 10 textures.
 */
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

/**
 * This function draws one of the coloured dots at the given playfield coordinate.
 */
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

/**
 * This function draws the title logo onscreen.
 */
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

/**
 * This function draws the "instructions" screen.
 */
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

/**
 * This function draws the given highscore table and its associated entries.
 */
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

/**
 * This functions draws the wooden outer frame onscreen.
 */
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

/**
 * This utility function writes some text with the default font on the screen at the given coordinates.
 */
void Graphics_DrawText(Uint16 x, Uint16 y, char *text)
{
	PutString(screen, x, y, text);
}

/**
 * This function shows the "you have won!" message.
 */
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

/**
 * This function shows the "you have lost" message.
 */
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

/**
 * This function initializes the graphics subsystem.
 * A fullscreen mode maybe requested.
 * @return TRUE if initialization was successfull
 */
BOOLEAN Graphics_Init(BOOLEAN set_fullscreen)
{
	/* determine video scaling */
#if(SCREENRES == _HIGH)
	const int screen_factor = 2;
#elif(SCREENRES == _LOW)
	const int screen_factor = 1;
#else
#error screen resolution is not properly defined
#endif

	/* determine bit per pixel */
	const int SDL_video_bitdepth = 16;

	/* determine screen settings like fullscreen, windowed, etc.... */
	Uint32 SDL_video_flags = ((set_fullscreen == TRUE) ? SDL_FULLSCREEN : 0);
#if(USE_HARDWARESURFACE == 1)
	SDL_video_flags |= SDL_HWSURFACE;
#else
 	SDL_video_flags |= SDL_SWSURFACE;
#endif
#if(USE_DOUBLEBUFFEREDSURFACE == 1)
	SDL_video_flags |= SDL_DOUBLEBUF;
#endif

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
	if ((screen = SDL_SetVideoMode((320 * screen_factor), (240 * screen_factor), SDL_video_bitdepth, SDL_video_flags)) == NULL)
	{
		fprintf(stderr, "%s\n\nUnable to set %dx%dx%d video mode: %s\n",VERSION, (320 * screen_factor), (240 * screen_factor), SDL_video_bitdepth, SDL_GetError());
		return (FALSE);
	}
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

/**
 * This callback function frees all memory requested by the Graphics submodule.
 * It is called with the atexit() mechanism at any fault or a proper program exit.
 */
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

/**
 * @return width of a dot tile in pixels
 */
Uint16 Graphics_GetDotWidth()
{
	return(dot_width);
}

/**
 * @return height of a dot tile in pixels
 */
Uint16 Graphics_GetDotHeight()
{
	return(dot_height);
}

/**
 * @return width of the entire screen in pixels
 */
Uint16 Graphics_GetScreenWidth()
{
	return(screen->w);
}

/**
 * @return height of the entire screen in pixels
 */
Uint16 Graphics_GetScreenHeight()
{
	return(screen->h);
}

/**
 * @return height of the font in pixels
 */
Uint16 Graphics_GetFontHeight()
{
	return(font_height);
}

/**
 * This function loads a graphics resource from a given filename.
 * Any surfaces loaded with this call must be freed at exit with the Graphics_CleanUp() call.
 * @return pointer to SDL_Surface if loading was successfull
 */
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

/**
 * This function loads the window icon from a file and initializes it for use.
 */
void Graphics_SetWindowIcon()
{
	window_icon = Graphics_LoadGraphicsResource("*ICON*");
	SDL_WM_SetIcon(window_icon, NULL);
}

/**
 * This function updates any changes to the screen and makes them visible.
 */
void Graphics_UpdateScreen()
{
	SDL_Flip(screen);
}
