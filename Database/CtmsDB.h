//
// Created by Adi Iman on 25/04/2020.
//

#ifndef WORKSHOP1_CTMSDB_H
#define WORKSHOP1_CTMSDB_H

#include <string>
#include "cpplinq.hpp"
#include <../Dependency/Database.h>

#include "Deck.h"
#include "Package.h"
#include "Package_Route.h"
#include "Receipt.h"
#include "Role.h"
#include "Route.h"
#include "Ticket.h"
#include "Ticket_Type.h"
#include "User.h"
#include "User_Role.h"
#include "Value_Type.h"
#include "Vessel.h"

#include <type_traits>
#include <iostream>
using namespace std;
using namespace cpplinq;

class CtmsDB
{
public:
	explicit CtmsDB(Database);
	long loggedInUserID;
	vector<Deck> deck;
	vector<Package> package;
	vector<Package_Route> package_route;
	vector<Receipt> receipt;
	vector<Role> role;
	vector<Route> route;
	vector<Ticket> ticket;
	vector<Ticket_Type> ticket_type;
	vector<User> user;
	vector<User_Role> user_role;
	vector<Value_Type> value_type;
	vector<Vessel> vessel;

	void initData();

	template<class T>
	bool Insert(T item);
	template<class T>
	bool Insert(vector<T> item);
	template<class T, class U>
	bool Insert(T item, vector<U> item2);
	template<class T>
	bool Update(T item);
	template<class T>
	bool Update(vector<T> item);
	template<class T, class U>
	bool Update(T item, vector<U> item2);
	template<class T>
	bool Delete(T item);
	Database db;
private:
	bool Operation(string Query, int Type);
};

#endif //WORKSHOP1_CTMSDB_H
