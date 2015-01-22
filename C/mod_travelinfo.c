/* dbticket/mod_travelinfo.c
 * (c) jan (dfragment.net) 2015
 * Licensed under MIT licence.
 */

#include <stdio.h>
#include <stdlib.h>

#include "modules.h"

void *tinfl_decompress_mem_to_heap(const void *pSrc_buf, size_t src_buf_len, size_t *pOut_len, int flags);

/* Okay, so much for "can't read information on the ticket".
 * We can, we just didn't choose to.
 * This requires an Deflate decoder. I'll write one later (hopefully).
 */

/* What we need to know:
 * The actual stream is always obj 4 0, and Flate-encoded.
 * The stream's length is always stored in obj 34 0.
 * Please take the values from the xref table. No guesses here!
 */

int getTravelStreamLength(FILE *file, int offset)
{
	fseek(file, offset, SEEK_SET);
	char *readBuf = (char *)calloc(24, 1);
	fread(readBuf, 23, 1, file);
	int streamLength;
	sscanf(readBuf, "34 0 obj\x0a%d\x0aendobj", &streamLength);
	free(readBuf);
	return streamLength;
}

void * createInflatedStream(void *data, int streamLength)
{
	size_t inflate;
	void *inflatedData = tinfl_decompress_mem_to_heap(data, streamLength, &inflate, 1);
	free(data);
	if (inflate <= 2)
		data = NULL; // some error.
	return inflatedData;
}

void * getTravelStream(FILE *file, int streamOffset, int streamLength)
{
	if (file == NULL || streamOffset == 0)
		return NULL;

	fseek(file, streamOffset+(0x39), SEEK_SET); // move to start of stream
	void *wholeStream = malloc(streamLength);
	size_t readBytes = fread(wholeStream, 1, streamLength, file);
	if (readBytes != streamLength)
		return NULL; // maybe some error handling

	return createInflatedStream(wholeStream, streamLength);
	//return wholeStream;
}
