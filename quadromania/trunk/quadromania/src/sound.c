/*
 * Quadromania
 * (c) 2002/2003/2009/2010 by Matthias Arndt <marndt@asmsoftware.de> / ASM Software
 *
 * File: sound.c - implements the sound and music API
 * last Modified: 27.06.2010 : 14:50
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

#if(HAVE_AUDIO == 1)
#include <SDL/SDL_mixer.h>
#endif

/* data structures... */
BOOLEAN sound_initialized = FALSE;

#if(HAVE_AUDIO == 1)
/* actual mixer chunks for our sound effects */
Mix_Chunk *sound_menu   = NULL;
Mix_Chunk *sound_turn   = NULL;
Mix_Chunk *sound_win    = NULL;
Mix_Chunk *sound_loose  = NULL;
#endif
/*************
 * CONSTANTS *
 *************/
const int audio_rate = 22050;
const Uint16 audio_format = AUDIO_S16; /* 16-bit stereo */
const int audio_channels = 2;
const int audio_buffers = 4096;

/*************
 * FUNCTIONS *
 *************/

/* initialize sound subsystem */
void Sound_Init()
{
#if(HAVE_AUDIO == 1)
	SDL_InitSubSystem(SDL_INIT_AUDIO);

	/* open audio devices and setup basic parameters */
	if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers))
	{
		fprintf(stderr,"Unable to open audio!\n");
	}
	else
	{
		/* load actual sound files.... */
		sound_menu  = Mix_LoadWAV("data/sound/menu.wav");
		sound_turn  = Mix_LoadWAV("data/sound/turn.wav");
		sound_win   = Mix_LoadWAV("data/sound/win.wav");
		sound_loose = Mix_LoadWAV("data/sound/loose.wav");

		sound_initialized = TRUE;
	}

	atexit(Sound_Exit);

#ifdef _DEBUG
	fprintf(stderr,"audio initialized....\n");
#endif
#endif
	return;
}

/* terminate sound subsystem properly */
void Sound_Exit()
{
#if(HAVE_AUDIO == 1)
	Mix_CloseAudio();
#endif
	sound_initialized = FALSE;
	return;
}

/* play a sound by symbolic name */
void Sound_PlayEffect(SoundEffect snd)
{
	if (sound_initialized == TRUE)
	{
#if(HAVE_AUDIO == 1)
		switch(snd)
		{
		case SOUND_MENU:
			Mix_PlayChannel(-1, sound_menu, 0);
			break;
		case SOUND_TURN:
			Mix_PlayChannel(-1, sound_turn, 0);
			break;
		case SOUND_WIN:
			Mix_PlayChannel(-1, sound_win, 0);
			break;
		case SOUND_LOOSE:
			Mix_PlayChannel(-1, sound_loose, 0);
			break;
		default:
			/* do nothing */
			break;
		}
#endif
	}
	return;
}
