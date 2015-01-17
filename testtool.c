/* dbticket/testtool.c
 * (c) jan (dfragment.net) 2015
 * Licensed under MIT licence.
 */

#include <stdio.h>
#include "modules.h"

#define COMPOUT(i, s) printf(s ": "); if (i == 1) { printf("pass!\n"); } else { printf("fail!\n"); }

int main(int argc, char const *argv[])
{
	if (argc != 2) {
		fprintf(stderr, "Invalid parameter count\n");
		return 1;
	}

	FILE *fptr = fopen(argv[1], "rb");

	short b = hasAcceptableSize(fptr);
	COMPOUT(b, "File size");
	if (b == 0) {
		printf("Wrong file size; ending before further checks.\n\n");
		return 2;
	}

	short a = producerStringPresent(fptr);
	COMPOUT(a, "Producer string");

	if (a == 0) {
		short h = trailerContainsHTML(fptr);
		COMPOUT(h, " -> HTML trailer");
	}

	short c = hasCorrectPDFVersion(fptr);
	COMPOUT(c, "PDF 1.4");
	
	short d = hasCorrectSignature(fptr);
	COMPOUT(d, "PDF signature");

	short e = hasImageHexcode(fptr);
	COMPOUT(e, "Image hex");

	printf("Score: %d out of 5\n\n", (a+b+c+d+e));

	return 0;
}