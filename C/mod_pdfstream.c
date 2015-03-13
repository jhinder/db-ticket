/* dbticket/mod_pdfstream.c
 * (c) jan (dfragment.net) 2015
 * Licensed under MIT licence.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

// Check for return value of NULL; in that case, abort all reading
void * createInflatedStream(void *data, int streamLength)
{
	size_t inflate;
	void *inflatedData = tinfl_decompress_mem_to_heap(data, streamLength, &inflate, 1);
	free(data);
	if (inflate <= 2)
		inflatedData = NULL; // some error.
	return inflatedData;
}

char * getTravelStream(FILE *file, int streamOffset, int streamLength)
{
	if (file == NULL || streamOffset == 0)
		return NULL;

	fseek(file, streamOffset+(0x39), SEEK_SET); // move to start of stream
	void *wholeStream = malloc(streamLength);
	size_t readBytes = fread(wholeStream, 1, streamLength, file);
	if (readBytes != streamLength)
		return NULL; // maybe some error handling

	return (char*)createInflatedStream(wholeStream, streamLength);
	//return wholeStream;
}

char ** extractTextObjectsFromStream(char *block)
{
	
	/* According to the PDF specification, text objects are structures as
	 * follows:
	 * BT *** T[m|d] (...) Tj ET
	 * The actual string is within parentheses. Escaping is possible via '\'.
	 * Because matched parentheses are allowed and becaue strings may be
	 * arbitrary in length, we need a pushdown automaton for poor people
	 * (with token counting in place of a stack).
	 */

	char **lines = (char **)malloc(256 * sizeof(char*));
	for (int i=0; i<256; i++)
		lines[i] = calloc(512, 1);
	int lineIndex = 0;
	
	// First step: search the buffer until we meet a BT.
	char *head = strstr(block, "BT");

	while ((head = strstr(head, "T")) != NULL) {
		char TxP1 = *(head + 1); // Either Td or Tm end the transformation.
		// In our case, a string will always follow.

		// T precedes f and j as well, but those are useless here.
		// T is also present in BT and ET, so filter those as well...
		char TxM1 = *(head - 1);
		if (TxP1 == 'j' || TxP1 == 'f' || TxM1 == 'B' || TxM1 == 'E') {
			head++; // Advance beyond the T
			continue; // Text printed; keep looking!
		}

		// Text transformation. This is what we want.
		if (TxP1 == 'm' || TxP1 == 'd') {
			// Yay!
			head += 2; // Skips m and the opening parenthesis
			int parenthesesCount = 0;
			char *membuf = (char*) calloc(1, 1024);
			// 1k per iteration should be enough.
			int subIndex = 0;
			do {
				char nextChar = *(head + subIndex);

				if (nextChar == '(')
					parenthesesCount++;
				else if (nextChar == ')') {
					parenthesesCount--;
					if (parenthesesCount == 0) // skip the closing bracket
						break;
				}

				membuf[subIndex] = nextChar;

				if (nextChar == '\\') {
					if (*(head + subIndex + 1) == '\r') {
						// Special case: Carriage Return -> Space
						membuf[subIndex] = ' '; // TODO Fix; low importance
					} else {
						// Special case: escaping -> no par. count
						membuf[subIndex + 1] = *(head + subIndex + 1);
					}
					subIndex++;
				}

				subIndex++;
			} while (parenthesesCount > 0);

			membuf[subIndex + 1] = '\0';
			membuf = (char*) realloc(membuf, strlen(membuf));
			membuf = (membuf + 1); // Remove opening bracket

			if (membuf[0] != '-') {
				//printf("%s\n", membuf);
				lines[lineIndex] = membuf;
				lineIndex++;	
			}

			// Don't free membuf here, we need it on the heap!

			head = strstr((head + subIndex), "Tj"); // jump to text print
		} else {
			head++;
			// head = strstr(head, "\n"); // jump to next line
			continue;
		}
	}

	lines = (char**) realloc(lines, (lineIndex + 1));
	free(head);
	return lines;
}
