/* dbticket/mod_producer.c
 * (c) jan (dfragment.net) 2015
 * Licensed under MIT licence.
 */

#include <stdio.h>
#include "intf_file_src.h"

/* The string "/Producer (Antenna House PDF Output Library" always begins
 * 1748 bytes before the EOF. (The string is 43 bytes long, for reference.)
 * For compatibility, we stop reading before the version number.
 */
#define PRODUCER_OFFSET 1748
#define PRODUCER_CONST  "/Producer (Antenna House PDF Output Library"

short producerStringWithXrefEntries(FILE *file, int xrefEntries)
{
	// Every xref entry is always 20 bytes long, so PRODUCER_OFFSET gets moved
	// by 20 bytes every time there's an additional entry.
	// (PRODUCER_OFFSET is set to 75 xref entries.)
	
	int newOffset = PRODUCER_OFFSET + 20*(xrefEntries-75);
	fseek(file, -(newOffset), SEEK_END);
	long offset = ftell(file);
	return compareSection(file, offset, 43, PRODUCER_CONST);
}

short producerStringPresent(FILE *file)
{
	// It's the same result, and we avoid code duplication.
	return producerStringWithXrefEntries(file, 75);
}
