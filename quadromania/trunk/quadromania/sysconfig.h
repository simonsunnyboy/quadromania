/*
 * Quadromania
 * (c) 2002/2003/2009/2010 by Matthias Arndt <marndt@asmsoftware.de> / ASM Software
 *
 * File: sysconfig.h - target system specific settings at compile time
 * last Modified: 19.01.2010 : 18:30
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

#ifndef __SYSCONFIG_H
#define __SYSCONFIG_H

	/* global settings for switches */
	#define _LOW 10
	#define _HIGH 20

	#if(defined __GP2X_WIZ)
		/* GP2x Wiz target using SDL */
		#define SCREENRES 					_LOW
		#define HAVE_WINDOWED_MODE          0
	#else
		/* default SDL target for use with windowing GUI, e.q. X11/Windows/Mac OS */
		#define SCREENRES 					_HIGH
		#define HAVE_WINDOWED_MODE          1
	#endif

#endif /* __SYSCONFIG_H */