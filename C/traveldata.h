/* dbticket/traveldata.h
 * (c) jan (dfragment.net) 2015
 * Licensed under MIT licence.
 */

#ifndef TRAVELDATA_H
#define TRAVELDATA_H

typedef enum train_t
{
	ICE,
	IC_EC,
	LOCAL_TRAIN,
	OTHER_TRAIN
} train_type;

typedef enum payment_t
{
	EC_CARD,
	CREDIT_CARD,
	DIRECT_DEBIT,
	OTHER_PAYMENT
} payment_method;

// This serves as sort of a simple linked list.
struct itinerary_section
{
	char *start, *destination;
	char *startPlatform, *destinationPlatform;
	char *trainIdentifier;
	unsigned int departureDate, arrivalDate;
	unsigned int departureTime, arrivalTime;
	struct itinerary_section *nextSection;
};

struct trip_information
{
	char *bookingID;
	double price;
	train_type train;
	payment_method payment;
	struct itinerary_section *trip_ab, *trip_ba;
};

#endif
