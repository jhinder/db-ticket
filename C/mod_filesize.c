/* dbticket/mod_filesize.c
 * (c) jan (dfragment.net) 2015
 * Licensed under MIT licence.
 */

#include <stdio.h>

#include "modules.h"

/* The file size is a quite good indicator, as both tickets and seat
 * reservations share some similarities. A valid ticket file is around
 * 500 kB (500 000 bytes), but no more than 600 kB. The smallest recent
 * ticket used while testing was 438 kB in size.
 * Note that some older tickets (from before 2010) are only ~15 kB in size.
 * We won't handle those here; this library is designed for recent tickets.
 */

#define MIN_SIZE 400000 // 400 kB
#define MAX_SIZE 600000 // 600 kB

short hasAcceptableSize(FILE *file)
{
	fseek(file, 0, SEEK_END);
	long fileSize = ftell(file);
	return (MIN_SIZE <= fileSize && fileSize <= MAX_SIZE);
}
