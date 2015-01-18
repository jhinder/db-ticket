/* dbticket/modules.h
 * (c) jan (dfragment.net) 2015
 * Licensed under MIT licence.
 */

#ifndef MODULES_H
#define MODULES_H

#include <stdio.h>

// mod_producer
short producerStringPresent(FILE *file);

// mod_filesize
short hasAcceptableSize(FILE *file);

// mod_pdfsig
short hasCorrectPDFVersion(FILE *file);
short hasCorrectSignature(FILE *file);

// mod_imghex
short hasImageHexcode(FILE *file);

// mod_html
short trailerContainsHTML(FILE *file);

// mod_xref75
int checkXrefTable(FILE *file);

#endif