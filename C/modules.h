/* dbticket/modules.h
 * (c) jan (dfragment.net) 2015
 * Licensed under MIT licence.
 */

#ifndef MODULES_H
#define MODULES_H

#include <stdio.h>

// mod_producer
short producerStringPresent(FILE *file);
short producerStringWithXrefEntries(FILE *file, int xrefEntries);

// mod_filesize
short hasAcceptableSize(FILE *file);

// mod_pdfsig
short hasCorrectPDFVersion(FILE *file);
short hasCorrectSignature(FILE *file);

// mod_imghex
short hasImageHexcode(FILE *file);

// mod_html
short trailerContainsHTML(FILE *file);
int findXrefOffsetInMalformedFile(FILE *file);

// mod_xref75
int getXrefLocation(FILE *file);
int checkXrefTable(FILE *file);

// mod_trailer
int * readXrefTable(FILE *file, int *count);

// mod_travelinfo
int getTravelStreamLength(FILE *file, int offset);
void * getTravelStream(FILE *file, int streamOffset, int lengthOffset);
struct trip_information parseTravelStream(char *block);

#endif
