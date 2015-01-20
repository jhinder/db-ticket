/* dbticket/intf_file_src.h
 * (c) jan (dfragment.net) 2015
 * Licensed under MIT licence.
 */

#ifndef INTF_FILE_SRC
#define INTF_FILE_SRC

#include <stdio.h>

short compareSection(FILE *file, long offset, size_t length, const char *comparison);

#endif
