/* dbticket/mod_travelinfo.c
 * (c) jan (dfragment.net) 2015
 * Licensed under MIT licence.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "modules.h"
//#include "traveldata.h"

// Forward declaration for tinfl.c
void *tinfl_decompress_mem_to_heap(const void *pSrc_buf, size_t src_buf_len, size_t *pOut_len, int flags);

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

// Now comes the fun part: actually parsing the (now decoded) text stream.
// Warning: this function destroys the input parameter.

#define SEEK_BLOCK_CR() strchr(block, '\n')

struct trip_information parseTravelStream(char *block)
{
	// First, set up all variables and storage
	char **lines = (char **)malloc(256 * sizeof(char*));
	for (int i=0; i<256; i++)
		lines[i] = calloc(512, 1);
	// 512b*256b = 131 kB -- we can still reduce this if less is enough
	unsigned int lineIndex = 0;

	long loc;
	char *cr = SEEK_BLOCK_CR();
	while (cr != NULL)
	{
		loc = cr - block + 1; // get location of \n
		if (block[0] == '(' && block[1] != '-') {
			// Every string line ends with ") Tj\n"
			// Also, we can skip the '(' at the start of the line
			memcpy(lines[lineIndex], block+1, loc-6);
			lineIndex++;
		}

		block = (block + loc); // Move block pointer beyond \n
		cr = SEEK_BLOCK_CR();
	}

	// At this point, the entire PDF string array is inside lines.
	// Let's get to parsing.
}
