/*
 * Quadromania
 * (c) 2002/2003 by Matthias Arndt <marndt@asmsoftware.de> / ASM Software
 *
 * File: main.c - the main module handling input and game control
 * last Modified: 26.1.2003 : 10:09
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

#define __VERSION "quadromania January 26th 2003"

#include <SDL/SDL.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "random.h"
#include "graphics.h"
#include "quadromania.h"
#include "boolean.h"

enum GAMESTATE {UNINITIALIZED,NONE,TITLE,SETUPCHANGED,GAME,WON,QUIT};
struct MOUSE { int x,y,button; };

/* the main function - program execution starts here... */
int main(int argc, char *argv[])
{
  Uint16 fullscreen=0,i,ok=FALSE,exit=FALSE,mouseclick=FALSE;

  Uint8 maxrotations=1; /* setup variable for the maximum amount of possible colors... */
  Uint8 level=1; /* game level - to setup the desired amount of initial rotations... */

  char nstr[10];

  SDL_Surface *screen; /* SDL Surface for video memory */
  SDL_Event event; /* SDL event for keyboard, mouse and focus actions... */
  enum GAMESTATE status,oldstatus; /* for the event driven automata... */
  struct MOUSE mouse;

  status=UNINITIALIZED;
  oldstatus=status;

  /* parse command line and set startup flags accordingly... */
  for(i=1; i<argc; i++)
    {
      ok=FALSE;
      if((strcmp(argv[i],"-f")==0)||(strcmp(argv[i],"--fullscreen")==0))
	{
	  /* fullscreen mode requested... */
	  fullscreen=TRUE;
	  ok=TRUE;
	}

      if((strcmp(argv[i],"-v")==0)||(strcmp(argv[i],"--version")==0))
	{
	  /* print version information.... */
	  fprintf(stderr,"%s\n\n",__VERSION);
	  fprintf(stderr,"(c) 2002 by Matthias Arndt <marndt@asmsoftware.de>\na game by ASM Software http://www.asmsoftware.de/\nThe GNU General Public License applies. See the file COPYING for details.\n");
	  fprintf(stderr,"Compiled on %s at %s\n\n",__DATE__,__TIME__);
	  return(1);	
	}

      if((strcmp(argv[i],"-h")==0)||(strcmp(argv[i],"--help")==0)||(strcmp(argv[i],"-?")==0))
	{
	  /* general program help.... */
	  fprintf(stderr,"%s\n\n",__VERSION);
	  fprintf(stderr,"Usage: %s [-f|--fullscreen] [-v|--version] [-h|-?|--help] \n",argv[0]);
	  fprintf(stderr,"              -f  activate fullscreen mode\n");
	  fprintf(stderr,"              -v  prints version information\n\n");
	  return(1);	
	}

      /* check for error in command line... */
      if(!ok)
	{
	  fprintf(stderr,"%s\n\n",__VERSION);
	  fprintf(stderr,"Unknown command line option: %s\n",argv[i]);
	  return(2);
	}
    }

  /* initialize SDL...  */
  if (SDL_Init(SDL_INIT_VIDEO) < 0) 
    {
	fprintf(stderr,"%s\n\nUnable to initialize SDL: %s\n", __VERSION, SDL_GetError());
	return(1);
    }
  /* make sure to shutdown SDL at program end... */
  atexit(SDL_Quit);

  /* Set an appropriate 16-bit video mode. */
  if ((screen=SDL_SetVideoMode(640, 480, 16,(fullscreen ? SDL_FULLSCREEN : 0)|SDL_HWSURFACE|SDL_DOUBLEBUF)) == NULL) 
    {
      fprintf(stderr,"%s\n\nUnable to set 640x480x16 video mode: %s\n", __VERSION,SDL_GetError());
      return(1);
    }
   
  /* set window title.. */
  SDL_WM_SetCaption("Quadromania",NULL);

  /* initialize random number generator... */
  initrandom();
  /* initialize graphics module... */
  initgraphics();
  clearplayfield();

  /* the main loop - event and automata driven :) */
  while(!exit)
    {
      /* Event reading and parsing.... */
      if(SDL_PollEvent(&event))
	{
	  switch( event.type )
	    {
	    /* mouse handling... */
	    case SDL_MOUSEBUTTONUP:
	      mouseclick=FALSE;
	      break;
	    case SDL_MOUSEBUTTONDOWN:
	      /* collect the mouse data */
	      mouse.x=event.button.x;
	      mouse.y=event.button.y;
	      mouse.button=event.button.button;
	      mouseclick=TRUE;
	      /* fprintf(stderr,"click %d at %d,%d\n",mouse.button,mouse.x,mouse.y); */
	      break;
	    /* keyboard handling... */
	    case SDL_KEYDOWN:
	      switch( event.key.keysym.sym)
		{
		case SDLK_ESCAPE:
		  /* ESC pressed? */
		  if(status==GAME)
		    {
		      /* is there a game running? if yes then back to title screen...*/
		      status=TITLE;
		    }
		  else
		    {
		      status = QUIT;
		    }
		  break;
	        default:
	          break;
	        }
		break;
	    /* SDL_QUIT event (window close) */
	    case SDL_QUIT:
	      status = QUIT;
              break;
	    default:
	      /* default is an unhandled event... */
              break;
	    }
	}

      /* act upon the state of the event driven automata... */
      switch(status)
	{
	case SETUPCHANGED:
	case TITLE:
	  if(oldstatus!=status) /* recently switched to the title screen? */
	    {
	      /* then we have to redraw it....*/
	      drawbackground(screen,9);
	      drawframe(screen);
	      drawtitel(screen);

	      text(screen,128,240,"Start the game");
	      text(screen,128,272,"Select colors");

	      for(i=0;i<maxrotations+1;++i)
		drawdot(screen,450+i*32,268,i);

	      text(screen,128,304,"Select amount of initial turns");
	      sprintf(nstr,"%d",rotationsperlevel(level));
	      text(screen,480,304,nstr);

	      text(screen,128,372,"Instructions");

	      text(screen,128,420,"Quit");

	      SDL_Flip(screen);
	      if(status==SETUPCHANGED)
		status=TITLE;
	      oldstatus=status;
	    }

	  /* check for clicks in the menu */
	  if(mouseclick==TRUE)
	    {
	      if(mouse.button==1)
		{
		  if((mouse.x>128)&&(mouse.x<450))
		    {
		      /* "start a new game" ? */
		      if((mouse.y>240)&&(mouse.y<264))
			{
			  status=GAME;
			  initplayfield(rotationsperlevel(level), maxrotations);
			  drawplayfield(screen);
			  SDL_Flip(screen);

			}

		      /* "Select Colors" ? */
		      if((mouse.y>272)&&(mouse.y<296))
			{
			  status=SETUPCHANGED;
			  ++maxrotations;
			  if(maxrotations>4)
			    maxrotations=1;
			}

		      /* "Select Colors" ? */
		      if((mouse.y>304)&&(mouse.y<328))
			{
			  status=SETUPCHANGED;
			  ++level;
			  if(level>10)
			    level=1;
			}

		      /* Quit? */
		      if((mouse.y>420)&&(mouse.y<444))
			{
			  status=QUIT;
			}
		    }
		}
	      mouseclick=FALSE;
	    }
	  break;
	case GAME:
	  /* is there a game running? */
	  if(oldstatus!=status)
	    oldstatus=status;

	  /* mousebutton clicked?*/
	  if(mouseclick==TRUE)
	    {
	      if(mouse.button==1)
		{
		  Uint16 xraster, yraster;
		  xraster=(Uint16) ((mouse.x-32)/32);
		  yraster=(Uint16) ((mouse.y-32)/32);

#ifdef _DEBUG
		  fprintf(stderr,"click at %d,%d\n",xraster,yraster);
#endif		  
		  /* valid click on playfield? */
		  if((xraster>0)&&(xraster<17)&&(yraster>0)&&(yraster<12))
		    {
		      /* then rotate the correct 3x3 part... */
		      rotate(xraster,yraster);
		      drawplayfield(screen);
		      SDL_Flip(screen);

		      /* check for succeful end... */
		      if(won())
			status=WON;  /* if yes (board cleared to red) - well go to end screen :) */
		      
		    }

		}
	      mouseclick=FALSE;
	    }

	  break;
	case WON:
	  if(status!=oldstatus)
	    {
	      SDL_Rect dest;
	      oldstatus=status;
	      /* draw some message ...*/

	      dest.x=10;
	      dest.y=220;
	      dest.w=629;
	      dest.h=40;
	      SDL_FillRect(screen,&dest,0);

	      dest.x=12;
	      dest.y=222;
	      dest.w=617;
	      dest.h=36;
	      SDL_FillRect(screen,&dest,SDL_MapRGB(screen->format,0,64,200));

	      text(screen,150,227,"Congratulations! You've won!");
	      SDL_Flip(screen);
	    }

	  if(mouseclick==TRUE)
	    {
	      mouseclick=FALSE;
	      status=TITLE;
	    }
	  break;
	case QUIT:
	  /* so you want to quit? */
	  exit=TRUE;
	  break;
	default:
	  /* unknown or undefined state - then go to the title screen...*/
	  status=TITLE;
	  oldstatus=NONE;
	  break;
	}

    }
  
  return(0);
}
