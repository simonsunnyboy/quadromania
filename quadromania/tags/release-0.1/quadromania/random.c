/*
 * Quadromania
 * (c) 2002/2003 by Matthias Arndt <marndt@asmsoftware.de> / ASM Software
 *
 * File: random.c - implements a random number generator
 * last Modified: 26.01.2003 : 10:14
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
#include <time.h>
#include "random.h"
/* This is a simple custom pseudorandom number generator. It's not a very
   good one, but it's sufficient for our purposes. Never trust the rand()
   included with the C library. Its quality varies between implementations,
   and it's easy to run into patterns within the generated numbers. At least
   this one is consistent. */
static Sint32 seed = 0;

void initrandom()
{
    seed = time(NULL);
}


/* FIXME: convert to a decent random number generator - preferrably aftern an algorithm my D.E. Knuth */
unsigned int getrandom()
{
    Sint32 p1 = 1103515245;
    Sint32 p2 = 12345;
    seed = (seed*p1+p2) % 2147483647;
    return (unsigned)seed/3;
}
