/*
 * Quadromania
 * (c) 2002/2003/2009/2010 by Matthias Arndt <marndt@asmsoftware.de> / ASM Software
 *
 * File: main.c - the main module handling input and game control
 * last Modified: 25.01.2010 : 17:56
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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "random.h"
#include "graphics.h"
#include "quadromania.h"
#include "boolean.h"
#include "SFont.h"

#include "main.h"
#include "event.h"
#include "gui.h"

SDL_Surface *screen; /* SDL Surface for video memory */

/* the main function - program execution starts here... */
int main(int argc, char *argv[])
{
	Uint16 i;
	BOOLEAN ok = FALSE;
#if(HAVE_WINDOWED_MODE == 1)
	BOOLEAN fullscreen = FALSE;
#else
	const BOOLEAN fullscreen = TRUE;
#endif

	/* parse command line and set startup flags accordingly... */
	for (i = 1; i < argc; i++)
	{
		ok = FALSE;
#if(HAVE_WINDOWED_MODE == 1)
		if ((strcmp(argv[i], "-f") == 0) || (strcmp(argv[i], "--fullscreen")
				== 0))
		{
			/* fullscreen mode requested... */
			fullscreen = TRUE;
			ok = TRUE;
		}
#endif

		if ((strcmp(argv[i], "-v") == 0) || (strcmp(argv[i], "--version") == 0))
		{
			/* print version information.... */
			fprintf(stderr, "%s\n\n", VERSION);
			fprintf(
					stderr,
					"(c) 2002/2003/2009/2010 by Matthias Arndt <marndt@asmsoftware.de>\na game by ASM Software http://www.asmsoftware.de/\nThe GNU General Public License applies. See the file COPYING for details.\n");
			fprintf(stderr, "Compiled on %s at %s\n\n", __DATE__, __TIME__);
			return (1);
		}

		if ((strcmp(argv[i], "-h") == 0) || (strcmp(argv[i], "--help") == 0)
				|| (strcmp(argv[i], "-?") == 0))
		{
			/* general program help.... */
			fprintf(stderr, "%s\n\n", VERSION);
			fprintf(
					stderr,
					"Usage: %s [-f|--fullscreen] [-v|--version] [-h|-?|--help] \n",
					argv[0]);
#if(HAVE_WINDOWED_MODE == 1)
			fprintf(stderr, "              -f  activate fullscreen mode\n");
#endif
			fprintf(stderr, "              -v  prints version information\n\n");
			return (1);
		}

		/* check for error in command line... */
		if (!ok)
		{
			fprintf(stderr, "%s\n\n", VERSION);
			fprintf(stderr, "Unknown command line option: %s\n", argv[i]);
			return (2);
		}
	}

	if (InitGameEngine(fullscreen) == TRUE)
	{
		MainHandler();
		return (0);
	}
	else
	{
		return (1);
	}
}

BOOLEAN InitGameEngine(BOOLEAN fullscreen)
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
			((fullscreen == TRUE) ? SDL_FULLSCREEN : 0) | SDL_HWSURFACE
					| SDL_DOUBLEBUF)) == NULL)
	{
		fprintf(stderr, "%s\n\nUnable to set 640x480x16 video mode: %s\n",
				VERSION, SDL_GetError());
		return (FALSE);
	}
#elif(SCREENRES == _LOW)
	if ((screen = SDL_SetVideoMode(320, 240, 16, ((fullscreen == TRUE) ? SDL_FULLSCREEN
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

	/* initialize random number generator... */
	Random_InitSeed();
	/* initialize graphics module... */
	Graphics_Init();
	/* initialize event handler */
	Event_Init();
	Quadromania_ClearPlayfield();

	return (TRUE);
}

void MainHandler()
{
	enum GAMESTATE status, oldstatus; /* for the event driven automata... */

	tGUI_MenuEntries menu; /* the current selected menu entry */

	Uint8 maxrotations = 1; /* setup variable for the maximum amount of possible colors... */
	Uint8 level = 1; /* game level - to setup the desired amount of initial rotations... */

	status = UNINITIALIZED;
	oldstatus = status;
	/* the main loop - event and automata driven :) */
	do
	{
		menu = MENU_UNDEFINED; /* safe guard menu selection */
		/* Event reading and parsing.... */
		Event_ProcessInput();
		if (Event_IsESCPressed() == TRUE)
		{
			if (status == GAME)
			{
				/* is there a game running? if yes then back to title screen...*/
				status = TITLE;
			}
			else
			{
				status = QUIT;
			}
			Event_DebounceKeys();
		}
		if (Event_QuitRequested() == TRUE)
		{
			status = QUIT;
		}

		/* act upon the state of the event driven automata... */
		switch (status)
		{
		case SETUPCHANGED:
		case TITLE:
			if (oldstatus != status) /* recently switched to the title screen? */
			{
				/* then we have to redraw it....*/
				GUI_DrawMainmenu(screen, maxrotations + 1, level);
				if (status == SETUPCHANGED)
					status = TITLE;
				oldstatus = status;
			}

			/* check for clicks in the menu */
			if (Event_MouseClicked() == TRUE)
			{
				if (Event_GetMouseButton() == 1)
				{
					menu = GUI_GetClickedMenuEntry(screen);
					switch (menu)
					{
					case MENU_START_GAME:
						/* "start a new game" ? */
						status = GAME;
						Quadromania_InitPlayfield(
								Quadromania_GetRotationsPerLevel(level),
								maxrotations);
						Quadromania_DrawPlayfield(screen);
						SDL_Flip(screen);
						break;
					case MENU_CHANGE_NR_OF_COLORS:
						/* "Select Colors" ? */
						status = SETUPCHANGED;
						++maxrotations;
						if (maxrotations > 4)
							maxrotations = 1;
						break;

					case MENU_CHANGE_NR_OF_ROTATIONS:
						/* "Select Colors" ? */
						status = SETUPCHANGED;
						++level;
						if (level > 10)
							level = 1;
						break;
					case MENU_INSTRUCTIONS:
						/* "Instructions" ? */
						status = INSTRUCTIONS;
						break;
					case MENU_QUIT:
						status = QUIT;
						break;
					default:
						/* undefined menu entry */
						break;
					};
				}
				Event_DebounceMouse();
			}
			break;
		case INSTRUCTIONS:
			/* shall we show the INSTRUCTIONS screen? */
			if (oldstatus != status)
			{
				oldstatus = status;
				/* redraw instructions screen */
				Graphics_DrawInstructions(screen);

			}
			if (Event_MouseClicked() == TRUE)
			{
				if ((Event_GetMouseButton() == 1) && (Event_GetMouseY()
						> (SCREEN_HEIGHT - Graphics_GetFontHeight())))
				{
					Event_DebounceMouse();
					status = TITLE;
				}
			}
			break;
		case GAME:
			/* is there a game running? */
			if (oldstatus != status)
				oldstatus = status;

			/* mousebutton clicked?*/
			if (Event_MouseClicked() == TRUE)
			{
				if (Event_GetMouseButton() == 1)
				{
					Uint16 xraster, yraster;
					xraster = (Uint16) ((Event_GetMouseX()
							- Graphics_GetDotWidth()) / Graphics_GetDotWidth());
					yraster = (Uint16) ((Event_GetMouseY()
							- Graphics_GetDotHeight())
							/ Graphics_GetDotHeight());

#ifdef _DEBUG
					fprintf(stderr,"click at %d,%d\n",xraster,yraster);
#endif
					/* valid click on playfield? */
					if ((xraster > 0) && (xraster < 17) && (yraster > 0)
							&& (yraster < 12))
					{
						/* then rotate the correct 3x3 part... */
						Quadromania_Rotate(xraster, yraster);
						Quadromania_DrawPlayfield(screen);
						SDL_Flip(screen);

						/* check for unsuccessful end*/
						if (Quadromania_IsTurnLimithit())
							status = GAMEOVER;

						/* check for successful game end... */
						if (Quadromania_IsGameWon())
							status = WON; /* if yes (board cleared to red) - well go to end screen :) */

					}

				}
				Event_DebounceMouse();
			}

			break;
		case WON:
			if (status != oldstatus)
			{
				oldstatus = status;
				GUI_DrawWinMessage(screen);
			}

			if (Event_MouseClicked() == TRUE)
			{
				Event_DebounceMouse();
				status = TITLE;
			}
			break;

		case GAMEOVER:
			if (status != oldstatus)
			{
				oldstatus = status;
				GUI_DrawGameoverMessage(screen);
			}

			if (Event_MouseClicked() == TRUE)
			{
				Event_DebounceMouse();
				status = TITLE;
			}
			break;
		case QUIT:
			/* so you want to quit? */
			break;
		default:
			/* unknown or undefined state - then go to the title screen...*/
			status = TITLE;
			oldstatus = NONE;
			break;
		}

	} while (status != QUIT);

}
