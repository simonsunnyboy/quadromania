/*
 * Quadromania
 * (c) 2002/2003/2009 by Matthias Arndt <marndt@asmsoftware.de> / ASM Software
 *
 * File: event.h - symbolic constants, data types and function prototypes for event handling
 * last Modified: 15.11.2009 : 17:44
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

#ifndef __EVENT_H
#define __EVENT_H
    #include "boolean.h"


	/**************************
	 * DATA TYPE DECLARATIONS *
     **************************/
	typedef struct
	{
		Uint16 x, y;
		Uint8 button;
		BOOLEAN clicked;
	} MOUSE;

	/**************
	 * PROTOTYPES *
	 **************/
	void Event_Init(void);
	void Event_ProcessInput(void);
	BOOLEAN Event_QuitRequested(void);
	BOOLEAN Event_IsESCPressed(void);
	Uint16 Event_GetMouseX(void);
	Uint16 Event_GetMouseY(void);
	Uint8 Event_GetMouseButton(void);
	BOOLEAN Event_MouseClicked(void);
	void Event_DebounceMouse(void);
	void Event_DebounceKeys(void);

#endif /* __EVENT_H */
