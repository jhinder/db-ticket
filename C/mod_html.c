/* dbticket/mod_html.c
 * (c) jan (dfragment.net) 2015
 * Licensed under MIT licence.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "modules.h"

/* Some tickets contain HTML at the end. This confuses mod_producer.
 * HTML at the end can replace the producer string if the rest of the
 * criteria is met.
 */

#define HTML_TAIL "</html>"

short trailerContainsHTML(FILE *file)
{
	// Read the last 512 bytes of the file and check for HTML_TAIL
	fseek(file, -512, SEEK_END);
	char *htmlBuf = (char*)calloc(512, 1);
	fread(htmlBuf, 512, 1, file);
	short tailExists = (strstr(htmlBuf, HTML_TAIL) != NULL);
	free(htmlBuf);
	return tailExists;
}

// Try finding the startxref somewhere after 450K.
// (Adobe had their reasons why an startxref + EOF ends the file...)

#define SEEK_BEGIN_BLOCKS 450
#define SEEK_BLOCK_SIZE 1000
#define SEEK_OFFSET (SEEK_BEGIN_BLOCKS * SEEK_BLOCK_SIZE)

int findXrefOffsetInMalformedFile(FILE *file)
{
	fseek(file, SEEK_OFFSET, SEEK_SET);
	char *readBuffer = (char*)calloc(SEEK_BLOCK_SIZE, 1);
	int xrefOffset = 0;
	do {
		fread(readBuffer, SEEK_BLOCK_SIZE, 1, file);
		char *xrbuf = strstr(readBuffer, "startxref");
		if (xrbuf != NULL) {
			// we found an startxref section
			char* number = (char*)calloc(7, 1); // don't forget extra byte for 0x00
			strncpy(number, xrbuf+9+1, 6);
			xrefOffset = (int)strtol(number, (char **)NULL, 10);
			free(number);
			if (xrefOffset < 400000 || xrefOffset > 525000)
				xrefOffset = 0; // unprobable values or errors
		}
		// no need to advance, freadf does that already
	} while (xrefOffset == 0 && !feof(file));

	free(readBuffer);
	return xrefOffset;
}
