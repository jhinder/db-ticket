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
	
	return checkPDFFileByFile(fptr);
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
	if (producerStringPresent(fptr)) {
		scorepoints += 1.0f;
	} else {
		// might be a ticket with an HTML tail
		if (trailerContainsHTML(fptr))
			scorepoints += 0.25f;
	}

	if (hasCorrectPDFVersion(fptr))
		scorepoints += 1.0f;

	if (hasCorrectSignature(fptr))
		scorepoints += 2.0f;

	if (hasImageHexcode(fptr))
		scorepoints += 2.0f;

	return scorepoints;

}