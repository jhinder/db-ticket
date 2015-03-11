/* dbticket/mod_travelinfo.c
 * (c) jan (dfragment.net) 2015
 * Licensed under MIT licence.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "modules.h"
#include "traveldata.h"

// Forward declaration for tinfl.c
void *tinfl_decompress_mem_to_heap(const void *pSrc_buf, size_t src_buf_len, size_t *pOut_len, int flags);

char * stripCityTicket(char *station);

/* What we need to know:
 * The actual stream is always obj 4 0, and Flate-encoded.
 * The stream's length is always stored in obj 34 0.
 * Please take the values from the xref table. No guesses here!
 */

int getTravelStreamLength(FILE *file, int offset)
{
	fseek(file, offset, SEEK_SET);
	char *readBuf = (char *)calloc(24, 1);
	fread(readBuf, 23, 1, file);
	int streamLength;
	sscanf(readBuf, "34 0 obj\x0a%d\x0aendobj", &streamLength);
	free(readBuf);
	return streamLength;
}

// Check for return value of NULL; in that case, abort all reading
void * createInflatedStream(void *data, int streamLength)
{
	size_t inflate;
	void *inflatedData = tinfl_decompress_mem_to_heap(data, streamLength, &inflate, 1);
	free(data);
	if (inflate <= 2)
		inflatedData = NULL; // some error.
	return inflatedData;
}

char * getTravelStream(FILE *file, int streamOffset, int streamLength)
{
	if (file == NULL || streamOffset == 0)
		return NULL;

	fseek(file, streamOffset+(0x39), SEEK_SET); // move to start of stream
	void *wholeStream = malloc(streamLength);
	size_t readBytes = fread(wholeStream, 1, streamLength, file);
	if (readBytes != streamLength)
		return NULL; // maybe some error handling

	return (char*)createInflatedStream(wholeStream, streamLength);
	//return wholeStream;
}

// Yes, the missing semicolons are intended.
#define SEEK_BLOCK_CR() strchr(block, '\x0a')
#define READ_UNTIL_FOUND(s) while (strcmp(lines[lineIndex], s) != 0) lineIndex++

// Warning: this function destroys the input parameter.
struct trip_information parseTravelStream(char *block)
{
	// All strings as a char* array.
	char **lines = extractTextObjectsFromStream(block);

	int lineIndex = 0;

	// 1: Ticket type. Extract from index 2.
	train_type ticketType;
	char ticketString[4] = {0,0,0,0};
	strncpy(ticketString, lines[2], 3);
	if (strcmp(ticketString, "ICE") == 0)
		ticketType = ICE;
	else if (strcmp(ticketString, "IC/") == 0) // actually IC/EC
		ticketType = IC_EC;
	else if (strcmp(ticketString, "Fah") == 0) // Fahrkarte
		ticketType = LOCAL_TRAIN;
	else
		ticketType = OTHER_TRAIN;

	// 2: One-way or two-way ticket? Extract from index 4.
	short trips;
	char nWayString[4] = {0,0,0,0};
	strncpy(nWayString, lines[4], 3);
	if (strcmp(nWayString, "Hin") == 0)
		trips = 2;
	else if (strcmp(nWayString, "ab ") == 0)
		trips = 1;

	// TODO Check for trips == 1, also in regression

	// 2.1 Get travel years. This is only relevant for multiyear-trips.
	unsigned int abYear, baYear;
	if (trips == 1) {
		sscanf(lines[4], "ab %*d.%*d.%d", &abYear);
	} else if (trips == 2) {
		sscanf(lines[4], "Hinfahrt ab %*d.%*d.%d, R%*cckfahrt ab %*d.%*d.%d",
			&abYear, &baYear);
	}

	// 3: Determine trip termini.

	// First leg.
	lineIndex = 5; // Yay, variable reusal!
	READ_UNTIL_FOUND("Hinfahrt:");
	char *startCity = stripCityTicket(lines[lineIndex+1]);
	char *endCity = stripCityTicket(lines[lineIndex+2]);
	if (endCity[0] == ' ')
		endCity = (endCity+1); // strip space
	// TODO Umlaut restoring

	lineIndex += 3;

	// 3.1: Return trip termini, if applicable
	char *returnStartCity, *returnEndCity;
	if (trips == 2) {
		// Find and read the return trip
		// TODO Replace ? with \200, because of local encoding bug
		READ_UNTIL_FOUND("R?ckfahrt:");
		// Same procedure as above; sorry about the code duplication!
		returnStartCity = stripCityTicket(lines[lineIndex+1]);
		returnEndCity = stripCityTicket(lines[lineIndex+2]);
		if (returnEndCity[0] == ' ')
			returnEndCity = (returnEndCity+1);

		lineIndex += 3;
	}

	// 4: Ticket prices. All prices are given in Euro (EUR), btw.
	READ_UNTIL_FOUND("Betrag");
	char* total = lines[lineIndex+1];
	int euro, cent;
	sscanf(total, "%d,%d%*c", &euro, &cent);
	double price = euro + ((double)cent * 0.01);

	// 4.1 Payment method
	payment_method paymentType;
	char payment[5] = {0, 0, 0, 0, 0};
	strncpy(payment, lines[lineIndex-1], 4);
	if (strcmp(payment, "Kred") == 0)
		paymentType = CREDIT_CARD;
	else if (strcmp(payment, "Last") == 0)
		paymentType = DIRECT_DEBIT;
	else if (strcmp(payment, "PayP") == 0)
		paymentType = OTHER_PAYMENT;
	else
		paymentType = OTHER_PAYMENT;

	// 5: Booking ID
	READ_UNTIL_FOUND("Auftragsnummer:");
	char bookingId[7];
	strncpy(bookingId, lines[lineIndex+1], 6); // That was easy.

	// 6: Travel schedule.
	/* This is probably the hardest part to decode. (The data still follows
	 * a certain format, it was just harder to find a pattern that allowed
	 * me to write an extraction block.)
	 */

	struct itinerary_section *itineraries[2] = {NULL, NULL};

	for (int i=0; i<trips; i++) {
		READ_UNTIL_FOUND("Reservierung");

		short stop = 0;
		char *store;
		struct itinerary_section *startSection, *currentSection;

		do {
			char *startStation = lines[lineIndex+1];
			int dDay, dMonth;
			sscanf(lines[lineIndex+2], "%d.%d.", &dDay, &dMonth);
			int dHour, dMinute;
			sscanf(lines[lineIndex+3], "ab %d:%d", &dHour, &dMinute);
			char *startPlatform = lines[lineIndex+4];
			char *stopStation = lines[lineIndex+5];
			store = stopStation; // cache it
			int aDay, aMonth;
			sscanf(lines[lineIndex+6], "%d.%d.", &aDay, &aMonth);
			int aHour, aMinute;
			sscanf(lines[lineIndex+7], "an %d:%d", &aHour, &aMinute);
			char *stopPlatform = lines[lineIndex+8];
			char *trainIdentifier = lines[lineIndex+9];

			struct itinerary_section nextStore = {
				.start = startStation,
				.destination = stopStation,
				.startPlatform = startPlatform,
				.destinationPlatform = stopPlatform,
				.trainIdentifier = trainIdentifier,
				.departureTime = (dHour * 100) + dMinute,
				.arrivalTime = (aHour * 100) + aMinute,
				.departureDate = (dMonth * 100) + dDay,
				.arrivalDate = (aMonth * 100) + aDay,
				.nextSection = NULL
			};

			struct itinerary_section *nextSection = &nextStore;

			if (startSection == NULL) {
				// set first element
				startSection = nextSection;
				currentSection = startSection;
			} else {
				// append struct and move on
				currentSection->nextSection = nextSection;
				currentSection = currentSection->nextSection;
			}

			size_t eCityLen = strlen(endCity);
			if (eCityLen >= strlen(stopStation) && strncmp(endCity, stopStation, eCityLen) == 0) {
				stop = 1;
			} else {
				lineIndex += 9;
				READ_UNTIL_FOUND(store);
				lineIndex--;
			}
			
		} while (!stop);

		// Assign head of list to appropriate array index
		itineraries[i] = startSection;
	}

	// After this, there is nothing to parse.
	// Let's construct the returning struct.

	struct trip_information trip = {
		.bookingID = bookingId,
		.train = ticketType,
		.price = price,
		.payment = paymentType,
		.trip_ab = itineraries[0],
		.trip_ba = itineraries[1]
	};

	// free(block);

	return trip;
}

char ** extractTextObjectsFromStream(char *block)
{
	
	/* According to the PDF specification, text objects are structures as
	 * follows:
	 * BT *** T[m|d] (...) Tj ET
	 * The actual string is within parentheses. Escaping is possible via '\'.
	 * Because matched parentheses are allowed and becaue strings may be
	 * arbitrary in length, we need a pushdown automaton for poor people
	 * (with token counting in place of a stack).
	 */

	char **lines = (char **)malloc(256 * sizeof(char*));
	for (int i=0; i<256; i++)
		lines[i] = calloc(512, 1);
	int lineIndex = 0;
	
	// First step: search the buffer until we meet a BT.
	char *head = strstr(block, "BT");

	while ((head = strstr(head, "T")) != NULL) {
		char TxP1 = *(head + 1); // Either Td or Tm end the transformation.
		// In our case, a string will always follow.

		// T precedes f and j as well, but those are useless here.
		// T is also present in BT and ET, so filter those as well...
		char TxM1 = *(head - 1);
		if (TxP1 == 'j' || TxP1 == 'f' || TxM1 == 'B' || TxM1 == 'E') {
			head++; // Advance beyond the T
			continue; // Text printed; keep looking!
		}

		// Text transformation. This is what we want.
		if (TxP1 == 'm' || TxP1 == 'd') {
			// Yay!
			head += 2; // Skips m and the opening parenthesis
			int parenthesesCount = 0;
			char *membuf = (char*) calloc(1, 1024);
			// 1k per iteration should be enough.
			int subIndex = 0;
			do {
				char nextChar = *(head + subIndex);

				if (nextChar == '(')
					parenthesesCount++;
				else if (nextChar == ')') {
					parenthesesCount--;
					if (parenthesesCount == 0) // skip the closing bracket
						break;
				}

				membuf[subIndex] = nextChar;

				if (nextChar == '\\') {
					if (*(head + subIndex + 1) == '\r') {
						// Special case: Carriage Return -> Space
						membuf[subIndex] = ' '; // TODO Fix; low importance
					} else {
						// Special case: escaping -> no par. count
						membuf[subIndex + 1] = *(head + subIndex + 1);
					}
					subIndex++;
				}

				subIndex++;
			} while (parenthesesCount > 0);

			membuf[subIndex + 1] = '\0';
			membuf = (char*) realloc(membuf, strlen(membuf));
			membuf = (membuf + 1); // Remove opening bracket

			if (membuf[0] != '-') {
				//printf("%s\n", membuf);
				lines[lineIndex] = membuf;
				lineIndex++;	
			}

			// Don't free membuf here, we need it on the heap!

			head = strstr((head + subIndex), "Tj"); // jump to text print
		} else {
			head++;
			// head = strstr(head, "\n"); // jump to next line
			continue;
		}
	}

	lines = (char**) realloc(lines, (lineIndex + 1));
	free(head);
	return lines;
}

// Strips the "+City" from station names.
char * stripCityTicket(char *station)
{
	char *lastPlus = strrchr(station, '+');
	if (lastPlus == NULL)
		return station;
	int index = lastPlus-station;
	char *strippedStation = malloc(index+1);
	strncpy(strippedStation, station, index);
	strippedStation[index] = '\0';
	return strippedStation;
}
