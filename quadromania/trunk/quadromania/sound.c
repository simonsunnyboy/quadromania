/*
 * Quadromania
 * (c) 2002/2003/2009/2010 by Matthias Arndt <marndt@asmsoftware.de> / ASM Software
 *
 * File: sound.c - implements the sound and music API
 * last Modified: 13.06.2010 : 18:05
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sound.h"
#include "datatypes.h"
#include "boolean.h"

/* data structures... */
BOOLEAN sound_initialized = FALSE;

/*************
 * FUNCTIONS *
 *************/

/* initialize sound subsystem */
void Sound_Init()
{
	/* TODO: do actual sound system init here */
	atexit(Sound_Exit);
	return;
}

/* terminate sound subsystem properly */
void Sound_Exit()
{
	sound_initialized = FALSE;
	return;
}

/* play a sound by symbolic name */
void Sound_PlayEffect(SoundEffect snd)
{
	/* TODO: add actual sound effect replay */
	if(sound_initialized == TRUE)
	{
	}
	return;
}
