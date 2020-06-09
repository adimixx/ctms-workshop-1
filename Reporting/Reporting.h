//
// Created by Adi Iman on 05/06/2020.
//

#ifndef WORKSHOP1_REPORTING_REPORTING_H_
#define WORKSHOP1_REPORTING_REPORTING_H_

#include "../Core.h"

class Reporting
{
 public:
	Reporting(CtmsDB*);
	void List();
 private:
	CtmsDB* db;
	int InputInt;
};

#endif //WORKSHOP1_REPORTING_REPORTING_H_
