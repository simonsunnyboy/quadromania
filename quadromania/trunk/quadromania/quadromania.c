/*
 * Quadromania
 * (c) 2002/2003/2009 by Matthias Arndt <marndt@asmsoftware.de> / ASM Software
 *
 * File: quadromania.c - handles the game logic and the playfield
 * last Modified: 10.11.2009 : 19:23
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

/* for data types... */
#include <SDL/SDL.h>

#include "random.h"
#include "graphics.h"
#include "quadromania.h"
#include "boolean.h"

/* data structures... */
static Uint8 playfield[18][13];
/* flags for the actual game state */
static Uint8 rotations, backgroundart;

/*************
 * FUNCTIONS *
 *************/

/* function that clears the playfield */
void clearplayfield()
{
	Uint8 i, j;
	for (i = 0; i < 18; i++)
		for (j = 0; j < 13; j++)
			playfield[i][j] = 0;
}

/* this function initializes the playfield and rotates squares around
 basically preparing the playfield for a game to be played */
void initplayfield(Uint16 initialrotations, Uint8 maxrotations)
{
	Uint16 i;

	rotations = maxrotations;
	backgroundart = ((Uint32) getrandom() % 10);
	clearplayfield();

	/* rotate the squares....*/
	for (i = 1; i < initialrotations; ++i)
		rotate(((getrandom() % 16) + 1), ((getrandom() % 11) + 1));

}

/* this method rotates the dots around a central point...
 actually the key algorithm in a Quadromania game ;)
 border and range checking has to be done in the calling routine... */
void rotate(Uint32 x, Uint32 y)
{
	Uint8 i, j;
	for (i = x - 1; i < (x + 2); ++i)
		for (j = y - 1; j < (y + 2); ++j)
		{
			++playfield[i][j];
			if (playfield[i][j] > rotations)
				playfield[i][j] = 0;
		}
}

/* this method/procedure draws the complete playfield */
void drawplayfield(SDL_Surface *screen)
{
	Uint16 i, j;

	drawbackground(screen, backgroundart);
	drawframe(screen);

	for (i = 0; i < 18; i++)
		for (j = 0; j < 13; j++)
			drawdot(screen, i * 32 + 32, j * 32 + 32, playfield[i][j]);

	text(screen, 0, 450, "Quadromania v0.2");
}

/* this function tells you wether you have won or not... */
BOOLEAN won()
{
	BOOLEAN ok = TRUE;
	Uint8 i, j;

	for (i = 0; i < 18; i++)
	{
		for (j = 0; j < 13; j++)
		{
			if (playfield[i][j] > 0)
			{
				ok = FALSE;
				break;
			}
		}
		/* break out of the loops if not won.... */
		if (ok == FALSE)
			break;
	}

	return (ok);
}

/* this function calculates the amount of initial rotatiosn for a given start level... */
Uint16 rotationsperlevel(Uint8 level)
{
	return (56 + level * 13);
}

