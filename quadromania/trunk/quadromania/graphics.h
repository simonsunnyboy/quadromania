/*
 * Quadromania
 * (c) 2002/2003/2009 by Matthias Arndt <marndt@asmsoftware.de> / ASM Software
 *
 * File: graphics.h - header file for the graphics API
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
#ifndef __GRAPHICS_H
	#define __GRAPHICS_H
	/**************
	 * PROTOTYPES *
	 **************/

	void drawbackground(SDL_Surface *, Uint8);
	void drawdot(SDL_Surface *, Uint16, Uint16, Uint8);
	void drawframe(SDL_Surface *);
	void text(SDL_Surface *, Uint16, Uint16, char *);
	void drawtitel(SDL_Surface *);
	void initgraphics(void);
	void exitgraphics(void);
#endif
