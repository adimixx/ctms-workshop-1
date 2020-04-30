//
// Created by Adi Iman on 27/04/2020.
//

#ifndef WORKSHOP1_VESSEL_H
#define WORKSHOP1_VESSEL_H

#include <string>
#include <boost/optional.hpp>
using namespace std;

class Vessel{
public:
    Vessel(long id, string name, double gt, double dwt, double length, double breadth, int year, string registerDate,
           bool isActive)
            : ID(id), name(name), gt(gt), dwt(dwt), length(length), breadth(breadth), year(year),
              registerDate(registerDate), isActive(isActive) {};
    Vessel(){};

    long ID;
    string name;
    double gt;
    double dwt;
    double length;
    double breadth;
    int year;
    string registerDate;
    bool isActive;
};

#endif //WORKSHOP1_VESSEL_H
