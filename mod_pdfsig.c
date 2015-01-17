/* dbticket/mod_pdfsig.c
 * (c) jan (dfragment.net) 2015
 * Licensed under MIT licence.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
	fseek(file, 9, SEEK_SET);
	char *sigBuf = (char*)calloc(1, 5); // malloc throws errors for some reason
	fread(sigBuf, 5, 1, file);
	int pdfBinComp = strcmp(sigBuf, PDF_BINARY);
	free(sigBuf);
	return (pdfBinComp == 0);
}

short hasCorrectPDFVersion(FILE *file)
{
	fseek(file, 0, SEEK_SET);
	char *vBuf = (char*)malloc(8);
	fread(vBuf, 8, 1, file);
	int pdfVerComp = strcmp(vBuf, PDF_VERSION);
	free(vBuf);
	return (pdfVerComp == 0);
}