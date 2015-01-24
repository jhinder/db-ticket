/* dbticket/libmain.c
 * (c) jan (dfragment.net) 2015
 * Licensed under MIT licence.
 */

#include <stdio.h>
#include "modules.h"
#include "db-ticket.h"

float checkPDFFileByPath(const char *filename)
{
	FILE *fptr = fopen(filename, "rb");
	if (fptr == NULL)
		return -1;
	
	float retVal = checkPDFFileByFile(fptr);
	fclose(fptr);
	return retVal;
}

float checkPDFFileByFile(FILE *fptr)
{
	if (fptr == NULL)
		return -1;

	float scorepoints = 0.0f;

	// Check 1: file size.
	if (hasAcceptableSize(fptr))
		scorepoints += 0.5f;
	else
		return -2;

	// Check 2: the /Producer string.
	short producerPresent = producerStringPresent(fptr);
	short hasHTMLTail = 0;
	if (producerPresent) {
		scorepoints += 1.0f;
	} else {
		// might be a ticket with an HTML tail
		hasHTMLTail = trailerContainsHTML(fptr);
		if (hasHTMLTail)
			scorepoints += 0.25f;
	}

	if (hasCorrectPDFVersion(fptr))
		scorepoints += 1.0f;

	if (hasCorrectSignature(fptr))
		scorepoints += 2.0f;

	if (hasImageHexcode(fptr))
		scorepoints += 2.0f;

	int xRefLen = 0;
	if (!hasHTMLTail) {
		// if it's got HTML, the xref table might be anywhere.
		checkXrefTable(fptr);
		if (xRefLen >= 75 && xRefLen <= 80)
			scorepoints += 1.0f;
	}

	if (!producerPresent && !hasHTMLTail && xRefLen != 0) {
		// might be a ticket with a weird xref offset.
		if (producerStringWithXrefEntries(fptr, xRefLen))
			scorepoints += 1.0f;
	}

	return scorepoints;

}
