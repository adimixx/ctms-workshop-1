//
// Created by Adi Iman on 27/04/2020.
//

#ifndef WORKSHOP1_PACKAGE_ROUTE_H
#define WORKSHOP1_PACKAGE_ROUTE_H
#include <string>
#include <boost/optional.hpp>
using namespace std;

class Package_Route{
public:
    Package_Route(long id, long routeId, long packageId)
            : ID(id), RouteID(routeId), PackageID(packageId){};
    Package_Route(){};

    long ID;
    long RouteID;
    long PackageID;
    int Arrangement;
};
#endif //WORKSHOP1_PACKAGE_ROUTE_H
