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

short producerStringPresent(FILE *file)
{
	fseek(file, -(PRODUCER_OFFSET), SEEK_END);
	long offset = ftell(file);
	return compareSection(file, offset, 43, PRODUCER_CONST);
}