//
// Created by Adi Iman on 27/04/2020.
//

#ifndef WORKSHOP1_VALUE_TYPE_H
#define WORKSHOP1_VALUE_TYPE_H

#include <string>
#include <boost/optional.hpp>
using namespace std;

class Value_Type
{
public:
	Value_Type(long id, string name) : ID(id), name(name)
	{
	};
	Value_Type()
	{
	};

	long ID;
	string name;
};

#endif //WORKSHOP1_VALUE_TYPE_H
