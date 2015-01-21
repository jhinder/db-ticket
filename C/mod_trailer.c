/* dbticket/mod_trailer.c
 * (c) jan (dfragment.net) 2015
 * Licensed under MIT licence.
 */

#include <stdio.h>
#include <stdlib.h>

#include "modules.h"

/* So far all calculated offsets were a good guess, and it was sufficient
 * in all cases. Now we want more -- official counts and offsets.
 * For that, we need the trailer and the xref table.
 *
 * Recommended procedure: find the last startxref, parse xref table, then
 * read the following trailer dict. Every PDF library on this planet uses
 * the recommended section order, so we'll rely on it.
 */

int * fillXrefTable(FILE *file, int *entryCount)
{
	// TODO Deal with these damn HTML appendices.
	int xrefStart = getXrefLocation(file) + 5; // skips "xref\n"
	fseek(file, xrefStart, SEEK_SET);
	char nums[6];
	fread(nums, 5, 1, file);
	nums[5] = '\0';

	int start, count;
	sscanf(nums, "%d %d\x0a", &start, &count);
	if (start == 0 && count == 0)
		return NULL;

	int *offsets = (int *)calloc(count, sizeof(int));
	char nextEntry[20]; // Xref entrys are ALWAYS 20 bytes
	int offset, generation;
	char usageFlag;
	for (int i=0; i<count; i++) {
		fread(nextEntry, 20, 1, file);
		// We don't care about generation and free/used flag
		sscanf(nextEntry, "%d %d %c", &offset, &generation, &usageFlag);
		if (usageFlag == 'n') // free entries confuse everything
			offsets[i] = offset;
	}

	*entryCount = count;
	return offsets;
}
