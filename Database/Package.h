//
// Created by Adi Iman on 27/04/2020.
//

#ifndef WORKSHOP1_PACKAGE_H
#define WORKSHOP1_PACKAGE_H
#include <string>
#include <boost/optional.hpp>
using namespace std;

class Package{
public:
    Package(long id, string name, string startDate, string endDate, double price, long vesselId)
            : ID(id), Name(name), start_date(startDate), end_date(endDate), price(price), vesselID(vesselId) {};
    Package() : ID(0) {};

    long ID;
    string Name;
    string start_date;
    string end_date;
    double price;
    long vesselID;
};
#endif //WORKSHOP1_PACKAGE_H
