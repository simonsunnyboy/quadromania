/*
 * Quadromania
 * (c) 2002/2003/2009/2010 by Matthias Arndt <marndt@asmsoftware.de> / ASM Software
 *
 * File: event.c - implements the input event API
 * last Modified: 25.01.2010 : 17:55
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
#include "event.h"
#include "boolean.h"

/* data structures... */
MOUSE mouse;
BOOLEAN ESCpressed       = FALSE;
BOOLEAN QUITrequest      = FALSE;
Uint8 debounce_tmr_mouse = 0;
Uint8 debounce_tmr_keys  = 0;

/*************
 * CONSTANTS *
 *************/
const Uint8 Event_Debounce_timeslices = 20;

/*************
 * FUNCTIONS *
 *************/

/* initialize event handler */
void Event_Init()
{
	mouse.x = 0;
	mouse.y = 0;
	mouse.button = 0;
	mouse.clicked = FALSE;
	debounce_tmr_mouse = Event_Debounce_timeslices;
	debounce_tmr_keys  = Event_Debounce_timeslices;
}

/* processes SDL Events and updates the event data structures */
void Event_ProcessInput()
{
	SDL_Event event; /* SDL event for keyboard, mouse and focus actions... */

	if (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		/* mouse handling... */
		case SDL_MOUSEBUTTONUP:
			mouse.clicked = FALSE;
			break;
		case SDL_MOUSEBUTTONDOWN:
			/* collect the mouse data and mouse click location */
			mouse.x = event.button.x;
			mouse.y = event.button.y;
			mouse.button = event.button.button;
			if(debounce_tmr_mouse == 0)
			{
				mouse.clicked = TRUE;
			}
			/* fprintf(stderr,"click %d at %d,%d\n",mouse.button,mouse.x,mouse.y); */
			break;
			/* keyboard handling... */
		case SDL_KEYUP:
			if(debounce_tmr_keys == 0)
			{
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					/* ESC pressed? */
					ESCpressed = TRUE;
					break;
				default:
					break;
				}
			}
			break;
			/* SDL_QUIT event (window close) */
		case SDL_QUIT:
			QUITrequest = TRUE;
			break;
		default:
			/* default is an unhandled event... */
			break;
		}
	}

	/* debounce input devices */
	if(debounce_tmr_mouse > 0)
		debounce_tmr_mouse--;

	if(debounce_tmr_keys > 0)
		debounce_tmr_keys--;
}

/* has a program shutdown been requested? */
BOOLEAN Event_QuitRequested()
{
	return(QUITrequest);
}

/* has ESC been pressed? */
BOOLEAN Event_IsESCPressed()
{
	return(ESCpressed);
}

/* return X position of mouse click */
Uint16 Event_GetMouseX()
{
	return(mouse.x);
}

/* return Y position of mouse click */
Uint16 Event_GetMouseY()
{
	return(mouse.y);
}

/* return which mouse button was pressed */
Uint8 Event_GetMouseButton()
{
	return(mouse.button);
}

/* return if the mouse button has been clicked */
BOOLEAN Event_MouseClicked()
{
	return(mouse.clicked);
}

/* callback to allow the event handler to debounce the mouse button */
void Event_DebounceMouse()
{
	mouse.clicked=FALSE;
	mouse.button=0;
	debounce_tmr_mouse = Event_Debounce_timeslices;
}

/* callback to allow the event handler to debounce key presses */
void Event_DebounceKeys()
{
	debounce_tmr_keys = Event_Debounce_timeslices;
	ESCpressed = FALSE;
}
