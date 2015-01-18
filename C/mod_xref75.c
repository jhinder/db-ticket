/* dbticket/mod_xref75
 * (c) jan (dfragment.net) 2015
 * Licensed under MIT licence.
 */

/* Some tickets don't have two but three pages.
 * Every PDF has an xref table at the end. What's interesting is that
 * all 2 page tickets have "0 75" after the xref, while 3 page tickets
 * have "0 78" or something different.
 * As long as we don't have an HTML ticket (at which point the xref could
 * be anywhere) we can locate the xref and check for the numer of entries.
 * That is easy: PDF has a "startxref" field!
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STARTXREF_OFFSET 24
#define STARTXREF "startxref"

int getXrefLocation(FILE *file)
{
	fseek(file, -(STARTXREF_OFFSET), SEEK_END);
	char *xrefBuf = (char*)calloc(9, 1);
	fread(xrefBuf, 9, 1, file);
	int result = 0;
	if (strcmp(xrefBuf, STARTXREF) == 0) {
		memset(xrefBuf, 0x00, 8);
		fseek(file, 1, SEEK_CUR);
		fread(xrefBuf, 6, 1, file);
		result = atoi(xrefBuf);
	}
	free(xrefBuf);
	return result;
}

int checkXrefTable(FILE *file)
{
	int location = getXrefLocation(file);
	if (location == 0)
		return 0;
	fseek(file, (location+7), SEEK_SET); // skip the "xref \0A 0 \20"
	char result[2];
	fread(result, 2, 1, file);
	return atoi(result);
}