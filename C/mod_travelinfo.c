/* dbticket/mod_travelinfo.c
 * (c) jan (dfragment.net) 2015
 * Licensed under MIT licence.
 */

#include <stdio.h>
#include <stdlib.h>

#include "modules.h"

/* Okay, so much for "can't read information on the ticket".
 * We can, we just didn't choose to.
 * This requires an Deflate decoder. I'll write one later (hopefully).
 */

/* What we need to know:
 * The actual stream is always obj 4 0, and Flate-encoded.
 * The stream's length is always stored in obj 34 0.
 * Please take the values from the xref table. No guesses here!
 */

char * getRawTravelStream(FILE *file, int streamOffset, int lengthOffset)
{
	if (file == NULL || streamOffset == 0 || lengthOffset == 0)
		return NULL;

	// Step 1: skip to the length offset and get that first.
	fseek(file, lengthOffset, SEEK_SET);
	char *readBuf = (char *)calloc(24, 1);
	fread(readBuf, 23, 1, file);
	int streamLength;
	sscanf(readBuf, "34 0 obj\x0a%d\x0aendobj", streamLength);
	free(readBuf);

	if (streamLength == 0)
		return NULL; // error handling? no idea

	fseek(file, streamOffset+(0x38), SEEK_SET); // move to start of stream
	char *wholeStream = (char *)calloc(streamLength, 1);
	fread(wholeStream, streamLength, 1, file);
	// Yes, it's a 4-6 kB block most of the time. We can handle that in 2015.

	return wholeStream;

}
