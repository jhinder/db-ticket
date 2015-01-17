/* dbticket/mod_html.c
 * (c) jan (dfragment.net) 2015
 * Licensed under MIT licence.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Some tickets contain HTML at the end. This confuses mod_producer.
 * HTML at the end can replace the producer string if the rest of the
 * criteria is met.
 */

#define HTML_TAIL "</html>"

short trailerContainsHTML(FILE *file)
{
	// Read the last 512 bytes of the file and check for HTML_TAIL
	fseek(file, -512, SEEK_END);
	char *htmlBuf = (char*)malloc(512);
	fread(htmlBuf, 512, 1, file);
	int tailExists = (strstr(htmlBuf, HTML_TAIL) != NULL);
	free(htmlBuf);
	return tailExists;
}