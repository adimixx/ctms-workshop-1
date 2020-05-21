//
// Created by Adi Iman on 27/04/2020.
//

#ifndef WORKSHOP1_ROUTE_H
#define WORKSHOP1_ROUTE_H

#include <string>
#include <boost/optional.hpp>
using namespace std;

class Route
{
public:
	Route(long id, string name) : ID(id), name(name)
	{
	};
	Route()
	{
	};

	long ID;
	string name;
};

#endif //WORKSHOP1_ROUTE_H
