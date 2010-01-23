/*
 * Quadromania
 * (c) 2002/2003/2009/2010 by Matthias Arndt <marndt@asmsoftware.de> / ASM Software
 *
 * File: gui.h - header file for the GUI handling
 * last Modified: 23.01.2010 : 19:08
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

#ifndef __GUI_H
#define __GUI_H

	/************
	 * DEFINES  *
	 ************/

	#define MAX_NR_OF_MENU_ENTRIES 10

	/***************
	 * DATA TYPES  *
	 ***************/

	typedef enum GUI_MenuEntries {
		MENU_UNDEFINED = 0,
		MENU_START_GAME = 1,
		MENU_CHANGE_NR_OF_COLORS = 2,
		MENU_CHANGE_NR_OF_ROTATIONS = 3,
		MENU_INSTRUCTIONS = 4,
		MENU_QUIT = 5
	} tGUI_MenuEntries;


	/**************
	 * PROTOTYPES *
	 **************/
	void GUI_DrawMainmenu(SDL_Surface*, Uint8, Uint8);
	void GUI_DrawWinMessage(SDL_Surface*);
	void GUI_DrawGameoverMessage(SDL_Surface*);

	Uint16 GUI_GetMenuColumnLeft(SDL_Surface*);
	Uint16 GUI_GetMenuColumnRight(SDL_Surface*);

	void GUI_InitMenuCoordinates(SDL_Surface*);
	tGUI_MenuEntries GUI_GetClickedMenuEntry(SDL_Surface*);

#endif /* __GUI_H */
