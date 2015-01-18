/* dbticket/mod_producer.c
 * (c) jan (dfragment.net) 2015
 * Licensed under MIT licence.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* The string "/Producer (Antenna House PDF Output Library" always begins
 * 1748 bytes before the EOF. (The string is 43 bytes long, for reference.)
 * For compatibility, we stop reading before the version number.
 */
#define PRODUCER_OFFSET 1748
#define PRODUCER_CONST  "/Producer (Antenna House PDF Output Library"

short producerStringPresent(FILE *file)
{
	fseek(file, -(PRODUCER_OFFSET), SEEK_END);
	char *prodBuf = (char*)calloc(43, 1);
	fread(prodBuf, 43, 1, file);
	int compResult = strcmp(prodBuf, PRODUCER_CONST);
	free(prodBuf);
	
	return (compResult == 0);
}