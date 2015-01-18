/* dbticket/testtool.c
 * (c) jan (dfragment.net) 2015
 * Licensed under MIT licence.
 */

#include <stdio.h>
#include <string.h>

#include "modules.h"
#include "db-ticket.h"

void _mods(const char *path);
void _file(FILE *file);
void _path(const char *path);

// For displaying test results
void displayResult(short value, const char *testType)
{
	printf("%s: ", testType);
	if (value == 1)
		printf("pass!\n");
	else
		printf("fail!\n");
}

int main(int argc, char const *argv[])
{
	printf("db-ticket test tool 0.3\n\n");

	if (argc < 2) {
		fprintf(stderr, "Invalid parameters.\nUsage: %s file [mods|file|path]\n (default mode is 'mods')\n", argv[0]);
		return 1;
	}

	short mode = 0;
	if (argc == 3) {
		if (strcmp(argv[2], "file") == 0)
			mode = 1;
		else if (strcmp(argv[2], "path") == 0)
			mode = 2;
	}

	printf("File: %s\n\n", argv[1]);

	if (mode == 0) {
		_mods(argv[1]);
	} else if (mode == 1) {
		FILE *fptr = fopen(argv[1], "rb");
		_file(fptr);
	} else if (mode == 2) {
		_path(argv[1]);
	}

	return 0;
}

// Module testing
void _mods(const char *path)
{
	FILE *fptr = fopen(path, "rb");

	printf(" -- Module test --\n");

	short b = hasAcceptableSize(fptr);
	displayResult(b, "File size");
	if (b == 0) {
		printf("Wrong file size; ending before further checks.\n\n");
		return;
	}

	short a = producerStringPresent(fptr);
	displayResult(a, "Producer string");

	short h = 0;
	if (a == 0) {
		h = trailerContainsHTML(fptr);
		displayResult(h, " -> HTML trailer");
	}

	if (h == 0) {
		// only possible if no HTML header misplaces the xref table
		int xRefLen = checkXrefTable(fptr);
		printf("xref length: %d", xRefLen);
	}

	short c = hasCorrectPDFVersion(fptr);
	displayResult(c, "PDF 1.4");
	
	short d = hasCorrectSignature(fptr);
	displayResult(d, "PDF signature");

	short e = hasImageHexcode(fptr);
	displayResult(e, "Image hex");

	printf("Score: %d out of 5\n", (a+b+c+d+e));
}

// Path API testing
void _path(const char *path)
{
	printf(" -- API Test (path-based) --\n");

	float score = checkPDFFileByPath(path);
	if (score < 0) {
		fprintf(stderr, "An error occured in _path()!\n");
		return;
	}
	printf("API call (path): %.2f of %.2f points\n", score, kMaximumScore);
}

// FILE* API testing
void _file(FILE *file)
{
	printf(" -- API Test (FILE*-based) --\n");

	float score = checkPDFFileByFile(file);
	if (score < 0) {
		fprintf(stderr, "An error occured in _file()!\n");
		return;
	}
	printf("API call (file): %.2f of %.2f points\n", score, kMaximumScore);
}