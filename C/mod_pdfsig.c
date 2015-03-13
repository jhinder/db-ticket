/* dbticket/mod_pdfsig.c
 * (c) jan (dfragment.net) 2015
 * Licensed under MIT licence.
 */

#include <stdio.h>
#include "modules.h"
#include "intf_file_src.h"

/* Every PDF file has a bunch of binary characters after the PDF header
 * to indicate that the file is binary. This used to be relevant for
 * file transmission over FTP.
 * All tickets happen to have the same binary characters.
 */

// PDF file version
#define PDF_VERSION "%PDF-1.4"
// Binary bytes
#define PDF_BINARY "\x25\xE2\xE3\xCF\xD3"

short hasCorrectSignature(FILE *file)
{
	return compareSection(file, 9, 5, PDF_BINARY);
}

short hasCorrectPDFVersion(FILE *file)
{
	return compareSection(file, 0, 8, PDF_VERSION);
}
