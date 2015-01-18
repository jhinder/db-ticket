/* dbticket/mod_filesize.c
 * (c) jan (dfragment.net) 2015
 * Licensed under MIT licence.
 */

#include <stdio.h>

/* The file size is another very good indicator, as both tickets and seat
 * reservations share some similarities. A valid ticket file is around
 * 500 kB (500 000 bytes), but no more than 600 kB; to be honest, the largest
 * ticket I encountered was 525 kB.
 */

#define MIN_SIZE 500000
#define MAX_SIZE 550000

short hasAcceptableSize(FILE *file)
{
	fseek(file, 0, SEEK_END);
	long fileSize = ftell(file);
	return (MIN_SIZE <= fileSize && fileSize <= MAX_SIZE);
}