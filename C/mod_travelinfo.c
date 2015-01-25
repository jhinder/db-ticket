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

void * getTravelStream(FILE *file, int streamOffset, int streamLength)
{
	if (file == NULL || streamOffset == 0)
		return NULL;

	fseek(file, streamOffset+(0x39), SEEK_SET); // move to start of stream
	void *wholeStream = malloc(streamLength);
	size_t readBytes = fread(wholeStream, 1, streamLength, file);
	if (readBytes != streamLength)
		return NULL; // maybe some error handling

	return createInflatedStream(wholeStream, streamLength);
	//return wholeStream;
}

// Now comes the fun part: actually parsing the (now decoded) text stream.

// Yes, the missing semicolons are intended.
#define SEEK_BLOCK_CR() strchr(block, '\n')
#define READ_UNTIL_FOUND(s) while (strcmp(lines[lineIndex], s) != 0) lineIndex++

// Warning: this function destroys the input parameter.
struct trip_information parseTravelStream(char *block)
{
	// First, set up all variables and storage
	char **lines = (char **)malloc(256 * sizeof(char*));
	for (int i=0; i<256; i++)
		lines[i] = calloc(512, 1);
	// 512b*256b = 131 kB -- we can still reduce this if less is enough
	unsigned int lineIndex = 0;

	long loc;
	char *cr = SEEK_BLOCK_CR();
	while (cr != NULL)
	{
		loc = cr - block + 1; // get location of \n
		if (block[0] == '(' && block[1] != '-') {
			// Every string line ends with ") Tj\n"
			// Also, we can skip the '(' at the start of the line
			memcpy(lines[lineIndex], block+1, loc-6);
			lineIndex++;
		}

		block = (block + loc); // Move block pointer beyond \n
		cr = SEEK_BLOCK_CR();
	}

	// At this point, the entire PDF string array is inside lines.
	// Let's get to parsing.

	/* Unlike other parts of the PDF, the line offsets are quite variable.
	 * Still, the whole stream is really to parse and interpret.
	 */

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

	// => ACTUAL CODE WILL COME SOON

	// After this, there is nothing to parse.
	// Let's construct the returning struct.

	struct travel_information tripAB = {
		.origin = startCity,
		.destination = endCity
		//unsigned int departureDate, arrivalDate;
		//unsigned int departureTime, arrivalTime;
	};

	struct travel_information tripBA;
	if (trips == 2) {
		tripBA = (struct travel_information) {
			.origin = returnStartCity,
			.destination = returnEndCity
		};
	}

	struct trip_information trip = {
		.bookingID = bookingId,
		.train = ticketType,
		.price = price,
		.payment = paymentType,
		.firstLeg = tripAB,
		.secondLeg = tripBA
	};

	return trip;

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
