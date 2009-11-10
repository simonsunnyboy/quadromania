#
# Quadromania
# (c) 2002/2003 by Matthias Arndt <marndt@asmsoftware.de> / ASM Software
#
# File: Makefile
# last Modified: 17.12.2002 : 16:42
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
#
# THIS SOFTWARE IS SUPPLIED AS IT IS WITHOUT ANY WARRANTY!
#
#

CFLAGS=`sdl-config --cflags` -W -Wall -ggdb -O3
SDL_LIB=`sdl-config --libs`

# the compiler to use - please use the GNU C Compiler if possible
CC=gcc
	
all: quadromania

quadromania: main.c graphics.o random.o SFont.o quadromania.o
	$(CC) $(CFLAGS) main.c -o quadromania graphics.c random.o SFont.o quadromania.o $(SDL_LIB) -lSDL_image

graphics.o: graphics.c graphics.h

random.o: random.c random.h

quadromania.o: quadromania.c quadromania.h boolean.h

SFont.o: SFont.c SFont.h


# default make rule for .c files - compile to .o
.c.o: 
	$(CC) $(CFLAGS) -c $<

# cleanup
clean:
	rm -f *.o quadromania

