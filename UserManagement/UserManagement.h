//
// Created by Adi Iman on 06/06/2020.
//

#ifndef WORKSHOP1_USERMANAGEMENT_USERMANAGEMENT_H_
#define WORKSHOP1_USERMANAGEMENT_USERMANAGEMENT_H_

#include "../Core.h"

class UserManagement
{
 public:
	UserManagement(CtmsDB* db);
	void Menu(),List(),AddEdit(int),Detail(int id),DeactDelete(User user), Activate(User user);
 private:
	CtmsDB* db;

	void detail_user(User user,vector<User_Role> userRole,bool isEdit);
	string User_Username(int id);
	vector<User_Role> User_UserRole(vector<User_Role> us, int userID);
	int InputInt;
	string InputString;
};

#endif //WORKSHOP1_USERMANAGEMENT_USERMANAGEMENT_H_
