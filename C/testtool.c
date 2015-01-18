/* dbticket/testtool.c
 * (c) jan (dfragment.net) 2015
 * Licensed under MIT licence.
 */

#include <stdio.h>
#include "modules.h"
#include "db-ticket.h"

#define COMPOUT(i, s) printf(s ": "); if (i == 1) { printf("pass!\n"); } else { printf("fail!\n"); }

int main(int argc, char const *argv[])
{

	printf("db-ticket test tool\n\n");

	if (argc != 2) {
		fprintf(stderr, "Invalid parameter count\n");
		return 1;
	}

	FILE *fptr = fopen(argv[1], "rb");

	printf("File: %s\n\n", argv[1]);

	printf(" -- TEST ONE: Module test --\n");

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

	printf("Score: %d out of 5\n", (a+b+c+d+e));

	printf(" -- TEST TWO: API Test --\n");

	float score = checkPDFFileByPath(argv[1]);
	if (score < 0) {
		perror("An error occured");
		return 3;
	}
	printf("API call: %.2f of %.2f points\n", score, kMaximumScore);

	return 0;
}