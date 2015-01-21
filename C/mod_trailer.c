/* dbticket/mod_trailer.c
 * (c) jan (dfragment.net) 2015
 * Licensed under MIT licence.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "modules.h"

/* So far all calculated offsets were a good guess, and it was sufficient
 * in all cases. Now we want more -- official counts and offsets.
 * For that, we need the trailer and the xref table.
 *
 * Recommended procedure: find the last startxref, parse xref table, then
 * read the following trailer dict. Every PDF library on this planet uses
 * the recommended section order, so we'll rely on it.
 */

void fillXrefTable(FILE *file)
{
	// TODO Deal with these damn HTML appendices.
	int xrefStart = getXrefLocation(file) + 5; // skips "xref\n"
	fseek(file, xrefStart, SEEK_SET);
	char nums[6];
	fread(nums, 5, 1, file);
	nums[5] = '\0';
	printf("%s", nums);
	int start, count;
	sscanf(nums, "%d %d\x0a", &start, &count);
	if (start == 0 && count == 0) {
		/*Error -- later*/
	}
	
	printf("%d entries starting at %d\n", count, start);

	int offsets[count];
	char nextEntry[20]; // Xref entrys are ALWAYS 20 bytes
	for (int i=0; i<count; i++) {
		fread(nextEntry, 20, 1, file);
		// We don't care about generation and free/used flag
		sscanf(nextEntry, "%d", &(offsets[i]));
	}

	// Test: display the whole table
	for (int i=0; i<count; i++)
		printf("obj %d 0 R\toffset %d\n", i, offsets[i]);

}
