/*
 * Quadromania
 * (c) 2002/2003/2009/2010 by Matthias Arndt <marndt@asmsoftware.de> / ASM Software
 *
 * File: gui.c - handles drawing the GUI and dialogues to the screen + verifies input on the GUI
 * last Modified: 21.01.2010 : 18:45
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

#include "gui.h"
#include "graphics.h"
#include "SFont.h"
#include "quadromania.h"

/**************************************
 * FUNCTIONS for drawing GUI elements *
 **************************************/

/* draw the mainmenu with all screen elements like logos, decals and game configuration on screen */
void GUI_DrawMainmenu(SDL_Surface* screen, Uint8 nr_of_dots,
		Uint8 selected_level)
{
	Uint8 i;
	char nstr[20];

	Graphics_DrawBackground(screen, 9);
	Graphics_DrawOuterFrame(screen);
	Graphics_DrawTitle(screen);

	Graphics_DrawText(screen, 128, 240, "Start the game");
	Graphics_DrawText(screen, 128, 272, "Select colors");

	for (i = 0; i < nr_of_dots; ++i)
		Graphics_DrawDot(screen, 450 + i * 32, 268, i);

	Graphics_DrawText(screen, 128, 304, "Select amount of initial turns");
	sprintf(nstr, "%d", Quadromania_GetRotationsPerLevel(selected_level));
	Graphics_DrawText(screen, 480, 304, nstr);

	Graphics_DrawText(screen, 128, 372, "Instructions");

	Graphics_DrawText(screen, 128, 420, "Quit");

	SDL_Flip(screen);
	return;
}

/* show the "you have won!" message */
void GUI_DrawWinMessage(SDL_Surface* screen)
{
	const Uint16 factor = (SCREEN_WIDTH / 320);
	SDL_Rect base, dest;
	/* draw some message ...*/

	base.x = (SCREEN_WIDTH / 64 );
	base.y = (SCREEN_HEIGHT / 2) - 20;
	base.w = (SCREEN_WIDTH - 2 * base.x);
	base.h = 40;
	SDL_FillRect(screen, &base, 0);

	dest.x = base.x + factor;
	dest.y = base.y + factor;
	dest.w = base.w - 2 * factor;
	dest.h = base.h - 2 * factor;
	SDL_FillRect(screen, &dest, SDL_MapRGB(screen->format, 0, 64, 200));

	XCenteredString(screen, (SCREEN_HEIGHT / 2) - 18 , "Congratulations! You've won!");
	SDL_Flip(screen);
}

/* show the "you have lost" message */
void GUI_DrawGameoverMessage(SDL_Surface* screen)
{
	const Uint16 factor = (SCREEN_WIDTH / 320);
	SDL_Rect base, dest;
	/* draw some message ...*/

	base.x = (SCREEN_WIDTH / 64 );
	base.y = (SCREEN_HEIGHT / 2) - 20;
	base.w = (SCREEN_WIDTH - 2 * base.x);
	base.h = 40;
	SDL_FillRect(screen, &base, 0);

	dest.x = base.x + factor;
	dest.y = base.y + factor;
	dest.w = base.w - 2 * factor;
	dest.h = base.h - 2 * factor;
	SDL_FillRect(screen, &dest, SDL_MapRGB(screen->format, 200, 64, 0));

	XCenteredString(screen, (SCREEN_HEIGHT / 2) - 18 , "GAME OVER! You hit the turn limit!");
	SDL_Flip(screen);
}
