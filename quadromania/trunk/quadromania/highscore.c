/*
 * Quadromania
 * (c) 2002/2003/2009/2010 by Matthias Arndt <marndt@asmsoftware.de> / ASM Software
 *
 * File: highscore.c - handles the highscore entries, loads and saves the highscore file
 * last Modified: 10.04.2010 : 12:15
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
#include "datatypes.h"
#include "highscore.h"
#include "boolean.h"

#include <string.h>
#include <time.h>

HighscoreFile hiscores;

BOOLEAN hiscore_entry = FALSE;
char buffer[]="YYYY-MM-DD hh:mm";

HighscoreEntry empty =
{
	0,
	"Nobody"
};

tChecksum Highscore_CalculateChecksum(void);

/* loads the highscore table from disk and verifies it */
void Highscore_LoadTable(void)
{
	Uint16 i,j;

	/* TODO: load highscores from disk */

	/* verify highscore file */

	if(  (hiscores.checksum_ != (Uint8)~hiscores.checksum)
	   ||(hiscores.checksum != Highscore_CalculateChecksum())
	  )
	{
		/* init empty highscore table if either the checksum of the highscore file does not match its complement
		 * or the checksum over the tables contents does not match the highscore table contents
		 */
		for(j = 0; j < HIGHSCORE_NR_OF_TABLES; j++)
		{
			for(i = 0; i < HIGHSCORE_NR_OF_ENTRIES_PER_TABLE; i++)
			{
				hiscores.Entry[j][i] = empty;
			}
		}

		hiscores.checksum = Highscore_CalculateChecksum(); /* build checksum and it's complement */
		hiscores.checksum_ = (Uint8)~hiscores.checksum;
	}
	return;
}

/* saves the highscore table to disk */
void Highscore_SaveTable(void)
{
	/* TODO: save highscores with proper checksum to disk */
	if(hiscore_entry == TRUE)
	{
		/* only save scores if we really had an entry */
		hiscore_entry = FALSE;
	}
	return;
}

/* calculates score entry position for a given score */
Uint16 Highscore_GetPosition(Uint16 table, Uint32 score)
{
	Uint16 i;

	if(table >= HIGHSCORE_NR_OF_TABLES)
	{
		return HIGHSCORE_NO_ENTRY;
	}

	for(i=0;i < HIGHSCORE_NR_OF_ENTRIES_PER_TABLE; i++)
	{
		if(score > hiscores.Entry[table][i].score)
		{
			return i;
		}
	}

	return HIGHSCORE_NO_ENTRY;
}

/* add a given highscore entry to the highscore list at the given position */
void Highscore_EnterScore(Uint16 table, Uint32 score, char *name, Uint16 position)
{
	Uint16 i;

	if(  (table >= HIGHSCORE_NR_OF_TABLES)
	   ||(position >= HIGHSCORE_NR_OF_ENTRIES_PER_TABLE)
	  )
	{
		return;
	}

	if(position < (HIGHSCORE_NR_OF_ENTRIES_PER_TABLE-1))
	{
		for(i = (HIGHSCORE_NR_OF_ENTRIES_PER_TABLE-1); i > position; i--)
		{
			hiscores.Entry[table][i] = hiscores.Entry[table][i-1];
		}
	}

	hiscores.Entry[table][position].score = score;
	/* assert size of name string before writing it into the highscore table */
	if(strlen(name) < HIGHSCORE_MAX_LEN_OF_NAME)
	{
		strcpy(hiscores.Entry[table][position].name, name);
	}
	else
	{
		strncpy(hiscores.Entry[table][position].name, name, HIGHSCORE_MAX_LEN_OF_NAME);
	}

	hiscore_entry = TRUE;

	return;
}

/* returns the named entry from the highscore list for display */
HighscoreEntry* Highscore_GetEntry(Uint16 table, Uint16 rank)
{
	if(  (table>=HIGHSCORE_NR_OF_TABLES)
	   ||(rank>=HIGHSCORE_NR_OF_ENTRIES_PER_TABLE)
	  )
	{
		/* invalid table position returns an empty entry */
		return &empty;
	}
	else
	{
		return &hiscores.Entry[table][rank];
	}
}

/* calculates a checksum over the highscore table data */
tChecksum Highscore_CalculateChecksum()
{
	/* TODO: implement table driven CRC-8 algorithm */
	return 0;
}

/* uses the curent system time to create a name string with the format YYYY-MM-DD hh:mm */
char* Highscore_GetNameFromTimestamp()
{
	time_t now;
	struct tm *timestamp;

	time(&now);	/* get current time */
	timestamp = localtime(&now); /* convert time from accumulated seconds into a field of values */

	sprintf(buffer, "%4d-%02d-%02d %02d:%02d", (timestamp->tm_year)+1900, (timestamp->tm_mon)+1, timestamp->tm_mday, timestamp->tm_hour, timestamp->tm_min);

	return(buffer);
}
