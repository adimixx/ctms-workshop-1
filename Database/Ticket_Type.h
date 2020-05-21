//
// Created by Adi Iman on 27/04/2020.
//

#ifndef WORKSHOP1_TICKET_TYPE_H
#define WORKSHOP1_TICKET_TYPE_H

#include <string>
#include <boost/optional.hpp>
using namespace std;

class Ticket_Type
{
public:
	Ticket_Type(long id, string name, boost::optional<long> discountId, boost::optional<double> discountValue)
		: ID(id), name(name), discountID(discountId), discountValue(discountValue)
	{
	};
	Ticket_Type()
	{
	};
	long ID;
	string name;
	boost::optional<long> discountID;
	boost::optional<double> discountValue;
};

#endif //WORKSHOP1_TICKET_TYPE_H
