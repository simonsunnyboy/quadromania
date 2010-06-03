/*
 * Quadromania
 * (c) 2002/2003/2009/2010 by Matthias Arndt <marndt@asmsoftware.de> / ASM Software
 *
 * File: highscore.c - handles the highscore entries, loads and saves the highscore file
 * last Modified: 03.06.2010 : 17:00
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

#include <assert.h>
#include <string.h>
#include <time.h>

HighscoreFile hiscores;

BOOLEAN hiscore_entry = FALSE;
char buffer[]="YYYY-MM-DD hh:mm";
char filename_buffer[512];

HighscoreEntry empty =
{
	0,
	"Nobody"
};

/* module internal function prototypes */
tChecksum Highscore_CalculateChecksum(void);
char* Highscore_GetFilename(void);

/* loads the highscore table from disk and verifies it */
void Highscore_LoadTable(void)
{
	Uint16 i,j;
	FILE   *fp;

	/* try to read highscore data structure from disk */
	fp = fopen(Highscore_GetFilename(),"rb");
	if(fp != NULL)
	{
		rewind(fp);
		if(fread(&hiscores, sizeof(HighscoreFile), 1, fp) != 1)
		{
			/* make checksums mismatch in case file size was not correct */
			hiscores.checksum = (tChecksum) 0x00;
			hiscores.checksum_ = (tChecksum) 0x01;
#ifdef _DEBUG
			fprintf(stderr,"Quadromania: highscores loaded with error - scores resetted!\n");
#endif
		}
		else
		{
#ifdef _DEBUG
			fprintf(stderr,"Quadromania: highscores loaded!\n");
#endif
		}
		fclose(fp);
	}

	/* verify highscore file */

	if(  ((tChecksum)(hiscores.checksum ^ hiscores.checksum_) != 0xFF)
	   ||(hiscores.checksum != Highscore_CalculateChecksum())
	  )
	{
		if(hiscores.checksum != Highscore_CalculateChecksum())
		{
#ifdef _DEBUG
				fprintf(stderr,"Quadromania: saved highscore checksum does not match the actual checksum!\n");
#endif
		}
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
		hiscores.checksum_ = (tChecksum)~hiscores.checksum;
#ifdef _DEBUG
				fprintf(stderr,"Quadromania: highscore checksumming did not match - reinit scores!\n");
#endif
	}
	else
	{
#ifdef _DEBUG
				fprintf(stderr,"Quadromania: highscore checksumming ok!\n");
#endif
	}
	return;
}

/* saves the highscore table to disk */
void Highscore_SaveTable(void)
{
	FILE *fp;

	if(hiscore_entry == TRUE)
	{
		/* update highscore file checksums */
		hiscores.checksum = Highscore_CalculateChecksum(); /* build checksum and it's complement */
		hiscores.checksum_ = (tChecksum)~hiscores.checksum;
		/* ensure checksum matches before saving to disk */
		assert((hiscores.checksum ^ hiscores.checksum_) == 0xFF);

		/* write highscore data structure to disk */
		fp = fopen(Highscore_GetFilename(),"wb");
		rewind(fp);
		fwrite(&hiscores, sizeof(HighscoreFile), 1, fp);
		fclose(fp);

#ifdef _DEBUG
		fprintf(stderr,"Quadromania: highscore saved to disk\n");
#endif
		/* only save scores if we really had an entry */
		hiscore_entry = FALSE;
	}
	else
	{
#ifdef _DEBUG
		fprintf(stderr,"Quadromania: highscore not saved - no changes!\n");
#endif
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
	Uint8 crc; /* the calculated CRC we will return */
	Uint16 i;
	Uint8 *ptr; /* pointer to the byte we want to checksum */

	const Uint8 crc8_Table[ ] =
	{
	     0,  94, 188, 226,  97,  63, 221, 131, 194, 156, 126,  32, 163, 253,  31,  65,
	   157, 195,  33, 127, 252, 162,  64,  30,  95,   1, 227, 189,  62,  96, 130, 220,
	    35, 125, 159, 193,  66,  28, 254, 160, 225, 191,  93,   3, 128, 222,  60,  98,
	   190, 224,   2,  92, 223, 129,  99,  61, 124,  34, 192, 158,  29,  67, 161, 255,
	    70,  24, 250, 164,  39, 121, 155, 197, 132, 218,  56, 102, 229, 187,  89,   7,
	   219, 133, 103,  57, 186, 228,   6,  88,  25,  71, 165, 251, 120,  38, 196, 154,
	   101,  59, 217, 135,   4,  90, 184, 230, 167, 249,  27,  69, 198, 152, 122,  36,
	   248, 166,  68,  26, 153, 199,  37, 123,  58, 100, 134, 216,  91,   5, 231, 185,
	   140, 210,  48, 110, 237, 179,  81,  15,  78,  16, 242, 172,  47, 113, 147, 205,
	    17,  79, 173, 243, 112,  46, 204, 146, 211, 141, 111,  49, 178, 236,  14,  80,
	   175, 241,  19,  77, 206, 144, 114,  44, 109,  51, 209, 143,  12,  82, 176, 238,
	    50, 108, 142, 208,  83,  13, 239, 177, 240, 174,  76,  18, 145, 207,  45, 115,
	   202, 148, 118,  40, 171, 245,  23,  73,   8,  86, 180, 234, 105,  55, 213, 139,
	    87,   9, 235, 181,  54, 104, 138, 212, 149, 203,  41, 119, 244, 170,  72,  22,
	   233, 183,  85,  11, 136, 214,  52, 106,  43, 117, 151, 201,  74,  20, 246, 168,
	   116,  42, 200, 150,  21,  75, 169, 247, 182, 232,  10,  84, 215, 137, 107,  53
	} ;

	/* The initial value is usually zero, but non-zero values are better.
		   You can pick any byte you want, just be sure it's the same every time. */
	crc = 0x1e;

	ptr = (Uint8 *)&hiscores;
	ptr++;

	for (i = 0; i < (sizeof(HighscoreFile)-2); i++ )
	{
	   crc = crc8_Table[ crc ^ *ptr ] ;
	   ptr++;
	}

	/* Some CRC implementations xor the final value with some other value. */
	/* crc = crc ^ 0xA5; */

#ifdef _DEBUG
	fprintf(stderr,"Quadromania: highscore checksum from RAM content = %x\n",crc);
#endif

	/* FIXME: CRC algorithm does not work properly yet */
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

/* construct filename with $HOME in use or not */
char* Highscore_GetFilename()
{
#if(USE_HOMEDIR == 1)
	sprintf(filename_buffer,"%s/%s",getenv("HOME"),HIGHSCORE_FILENAME);
#else
	sprintf(filename_buffer,"./%s",HIGHSCORE_FILENAME);
#endif

#ifdef _DEBUG
	fprintf(stderr,"Quadromania: highscore file = %s\n",&filename_buffer[0]);
#endif
	return(filename_buffer);
}
