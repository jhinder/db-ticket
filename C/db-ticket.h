/* dbticket/db-ticket.h
 * (c) jan (dfragment.net) 2015
 * Licensed under MIT licence.
 */

/* This is the header file for the db-ticket library.
 * For information on usage, read the documentation that came with the library,
 * or see the online documentation at http://github.com/jhinder/db-ticket.
 */

#ifndef DB_TICKET_H
#define DB_TICKET_H

#include <stdio.h>

#define kMaximumScore 7.5f

float checkPDFFileByFile(FILE *file);
float checkPDFFileByPath(const char *filename);

#endif
