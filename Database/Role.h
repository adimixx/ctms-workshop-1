//
// Created by Adi Iman on 27/04/2020.
//

#ifndef WORKSHOP1_ROLE_H
#define WORKSHOP1_ROLE_H

#include <string>
#include <boost/optional.hpp>
using namespace std;

class Role{
public:
    Role(long id, string name) : ID(id), name(name) {};
    Role(){};

    long ID;
    string name;
};

#endif //WORKSHOP1_ROLE_H
