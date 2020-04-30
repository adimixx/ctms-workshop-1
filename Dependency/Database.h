//
// Created by Adi Iman on 25/04/2020.
//

#ifndef WORKSHOP1_DATABASE_H
#define WORKSHOP1_DATABASE_H

#include <iostream>
#include <string>
#include <mysql/mysql.h>
using namespace std;

class Database {
public:
    string statement;
    MYSQL_RES* res;
    MYSQL_ROW row, FetchRow();
    MYSQL* conn;
    int data;
    bool ConnectionFunction();
    void select();
    bool cud();
    int RowCount();
    int LastInsertedID();
};


#endif //WORKSHOP1_DATABASE_H
