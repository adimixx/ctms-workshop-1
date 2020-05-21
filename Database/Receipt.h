//
// Created by Adi Iman on 27/04/2020.
//

#ifndef WORKSHOP1_RECEIPT_H
#define WORKSHOP1_RECEIPT_H

#include <string>
#include <boost/optional.hpp>
using namespace std;

class Receipt
{
public:
	Receipt(long id, string date, double total, double cashIn, double cashOut, long purchaseUserId)
		: ID(id), date(date), total(total), cashIn(cashIn), cashOut(cashOut), purchaseUserID(purchaseUserId)
	{
	};
	Receipt()
	{
	};

	long ID;
	string date;
	double total;
	double cashIn;
	double cashOut;
	long purchaseUserID;
};

#endif //WORKSHOP1_RECEIPT_H
