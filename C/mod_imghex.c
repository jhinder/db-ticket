/* dbticket/mod_imghex.c
 * (c) jan (dfragment.net) 2015
 * Licensed under MIT licence.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* At offset 0x1A6 a stream (which I suppose is the "DB" image in the head)
 * begins, and it has the following hex code (length 0x11)
 * 78 DA ED DC 5F 48 1C 47 18 00 F0 F5 0F 1C 82 14 1F
 */

#define IMAGE_HEX "\x78\xDA\xED\xDC\x5F\x48\x1C\x47\x18\x00\xF0\xF5\x0F\x1C\x82\x14\x1F"

short hasImageHexcode(FILE *file)
{
	fseek(file, 0x1A6, SEEK_SET);
	char *imgStreamBuf = (char*)calloc(0x11, 1);
	fread(imgStreamBuf, 0x11, 1, file);
	int compResult = strcmp(imgStreamBuf, IMAGE_HEX);
	free(imgStreamBuf);
	
	return (compResult == 0);
}