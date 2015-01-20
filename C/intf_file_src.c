/* dbticket/intf_file_src.c
 * (c) jan (dfragment.net) 2015
 * Licensed under MIT licence.
 */

// Before you ask: the file name stands for
// Internal functions: FILE section read & compare.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "intf_file_src.h"

// This code is used all the time, so it seemed like a good idea to separate it.
short compareSection(FILE *file, long offset, size_t length, const char *comparison)
{
	fseek(file, offset, SEEK_SET);
	char *buf = (char*)calloc(length, 1);
	fread(buf, length, 1, file);
	int compResult = strcmp(buf, comparison);
	free(buf);
	return (compResult == 0);
}