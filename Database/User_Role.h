//
// Created by Adi Iman on 27/04/2020.
//

#ifndef WORKSHOP1_USER_ROLE_H
#define WORKSHOP1_USER_ROLE_H
#include <string>
#include <boost/optional.hpp>
using namespace std;

class User_Role
{
public:
	long Id, UserID, RoleID;
	User_Role()
	{
	};
	User_Role(long Id, long UserID, long RoleID)
	{
		this->Id = Id;
		this->UserID = UserID;
		this->RoleID = RoleID;
	};
};
#endif //WORKSHOP1_USER_ROLE_H
