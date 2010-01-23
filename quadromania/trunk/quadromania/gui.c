/*
 * Quadromania
 * (c) 2002/2003/2009/2010 by Matthias Arndt <marndt@asmsoftware.de> / ASM Software
 *
 * File: gui.c - handles drawing the GUI and dialogues to the screen + verifies input on the GUI
 * last Modified: 23.01.2010 : 19:19
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
#include "event.h"
#include "SFont.h"
#include "quadromania.h"

/*******************************
 * data structures for the GUI *
 *******************************/

Uint16 GUI_MenuPosition[MAX_NR_OF_MENU_ENTRIES];

/**************************************
 * FUNCTIONS for drawing GUI elements *
 **************************************/

/* draw the mainmenu with all screen elements like logos, decals and game configuration on screen */
void GUI_DrawMainmenu(SDL_Surface* screen, Uint8 nr_of_dots,
		Uint8 selected_level)
{
	Uint8 i;
	const Uint16 menu_column = GUI_GetMenuColumnLeft(screen);
	char nstr[20];

	if (GUI_MenuPosition[MENU_START_GAME] == 0)
	{
		GUI_InitMenuCoordinates(screen);
	}

	Graphics_DrawBackground(screen, 9);
	Graphics_DrawOuterFrame(screen);
	Graphics_DrawTitle(screen);

	Graphics_DrawText(screen, menu_column, GUI_MenuPosition[MENU_START_GAME],
			"Start the game");
	Graphics_DrawText(screen, menu_column,
			GUI_MenuPosition[MENU_CHANGE_NR_OF_COLORS], "Select colors");

	for (i = 0; i < nr_of_dots; ++i)
		Graphics_DrawDot(screen, ((SCREEN_WIDTH * 450) / 640) + i
				* Graphics_GetDotWidth(),
				GUI_MenuPosition[MENU_CHANGE_NR_OF_COLORS], i);

	Graphics_DrawText(screen, menu_column,
			GUI_MenuPosition[MENU_CHANGE_NR_OF_ROTATIONS],
			"Select amount of initial turns");
	sprintf(nstr, "%d", Quadromania_GetRotationsPerLevel(selected_level));
	Graphics_DrawText(screen, ((SCREEN_WIDTH * 480) / 640),
			GUI_MenuPosition[MENU_CHANGE_NR_OF_ROTATIONS], nstr);

	Graphics_DrawText(screen, menu_column, GUI_MenuPosition[MENU_INSTRUCTIONS],
			"Instructions");

	Graphics_DrawText(screen, menu_column, GUI_MenuPosition[MENU_QUIT], "Quit");

	SDL_Flip(screen);
	return;
}

/* show the "you have won!" message */
void GUI_DrawWinMessage(SDL_Surface* screen)
{
	const Uint16 factor = (SCREEN_WIDTH / 320);
	SDL_Rect base, dest;
	/* draw some message ...*/

	base.x = (SCREEN_WIDTH / 64);
	base.y = (SCREEN_HEIGHT / 2) - 20;
	base.w = (SCREEN_WIDTH - 2 * base.x);
	base.h = 40;
	SDL_FillRect(screen, &base, 0);

	dest.x = base.x + factor;
	dest.y = base.y + factor;
	dest.w = base.w - 2 * factor;
	dest.h = base.h - 2 * factor;
	SDL_FillRect(screen, &dest, SDL_MapRGB(screen->format, 0, 64, 200));

	XCenteredString(screen, (SCREEN_HEIGHT / 2) - 18,
			"Congratulations! You've won!");
	SDL_Flip(screen);
	return;
}

/* show the "you have lost" message */
void GUI_DrawGameoverMessage(SDL_Surface* screen)
{
	const Uint16 factor = (SCREEN_WIDTH / 320);
	SDL_Rect base, dest;
	/* draw some message ...*/

	base.x = (SCREEN_WIDTH / 64);
	base.y = (SCREEN_HEIGHT / 2) - 20;
	base.w = (SCREEN_WIDTH - 2 * base.x);
	base.h = 40;
	SDL_FillRect(screen, &base, 0);

	dest.x = base.x + factor;
	dest.y = base.y + factor;
	dest.w = base.w - 2 * factor;
	dest.h = base.h - 2 * factor;
	SDL_FillRect(screen, &dest, SDL_MapRGB(screen->format, 200, 64, 0));

	XCenteredString(screen, (SCREEN_HEIGHT / 2) - 18,
			"GAME OVER! You hit the turn limit!");
	SDL_Flip(screen);
	return;
}

/* get the left x coordinate of menu points */
Uint16 GUI_GetMenuColumnLeft(SDL_Surface* screen)
{
	return ((SCREEN_WIDTH * 48) / 320);
}

/* get the right x coordinate of menu points */
Uint16 GUI_GetMenuColumnRight(SDL_Surface* screen)
{
	return (SCREEN_WIDTH - GUI_GetMenuColumnLeft(screen));
}

/* initialize position list of menu entries */
void GUI_InitMenuCoordinates(SDL_Surface* screen)
{
	const Uint16 start_pos = ((SCREEN_HEIGHT * 240) / 480);
	const Uint16 offset_per_line = Graphics_GetFontHeight() + (SCREEN_HEIGHT
			/ 120);

	GUI_MenuPosition[MENU_START_GAME] = start_pos;
	GUI_MenuPosition[MENU_CHANGE_NR_OF_COLORS] = start_pos + offset_per_line;
	GUI_MenuPosition[MENU_CHANGE_NR_OF_ROTATIONS] = start_pos + offset_per_line
			* 2;
	GUI_MenuPosition[MENU_INSTRUCTIONS] = start_pos + offset_per_line * 4;
	GUI_MenuPosition[MENU_QUIT] = start_pos + offset_per_line * 5;
	return;
}

/* determines the currently pointed to mainmenu entry via the Event interface */
tGUI_MenuEntries GUI_GetClickedMenuEntry(SDL_Surface* screen)
{
	const Uint16 font_height = Graphics_GetFontHeight();
	const Uint16 mouse_y = Event_GetMouseY();

	if ((Event_GetMouseX() > GUI_GetMenuColumnLeft(screen)) && (Event_GetMouseX() < GUI_GetMenuColumnRight(screen)))
	{
		if ((mouse_y > GUI_MenuPosition[MENU_START_GAME]) && (mouse_y
				< GUI_MenuPosition[MENU_START_GAME] + font_height))
		{
			return MENU_START_GAME;
		}
		else if ((mouse_y > GUI_MenuPosition[MENU_CHANGE_NR_OF_COLORS])
				&& (mouse_y < GUI_MenuPosition[MENU_CHANGE_NR_OF_COLORS]
						+ font_height))
		{
			return MENU_CHANGE_NR_OF_COLORS;
		}
		else if ((mouse_y > GUI_MenuPosition[MENU_CHANGE_NR_OF_ROTATIONS])
				&& (mouse_y < GUI_MenuPosition[MENU_CHANGE_NR_OF_ROTATIONS]
						+ font_height))
		{
			return MENU_CHANGE_NR_OF_ROTATIONS;
		}
		else if ((mouse_y > GUI_MenuPosition[MENU_INSTRUCTIONS]) && (mouse_y
				< GUI_MenuPosition[MENU_INSTRUCTIONS] + font_height))
		{
			return MENU_INSTRUCTIONS;
		}
		else if ((mouse_y > GUI_MenuPosition[MENU_QUIT]) && (mouse_y
				< GUI_MenuPosition[MENU_QUIT] + font_height))
		{
			return MENU_QUIT;
		}
		else
		{
			return MENU_UNDEFINED;
		}
	}
	else
	{
		return MENU_UNDEFINED;
	}
}
