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

struct travel_information
{
	char *origin, *destination;
	unsigned int departureDate, arrivalDate;
	unsigned int departureTime, arrivalTime;
};

struct trip_information
{
	char *bookingID;
	train_type train;
	double price;
	payment_method payment;
	struct travel_information firstLeg, secondLeg;
};

#endif
