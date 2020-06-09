//
// Created by Adi Iman on 06/06/2020.
//

#include "UserManagement.h"

UserManagement::UserManagement(CtmsDB* db)
{
	this->db = db;
}

void UserManagement::Menu()
{
	do
	{
		Dependency::ClearScreen("User Management");

		cout << "1. Add New User\n";
		cout << "2. List Users\n\n";

		cout << "0. Main Menu\n\n";

		InputInt = input::InputInt("Enter your input", 0, 2);

		if (InputInt == 1)
		{
			AddEdit(0);
		}

		else if (InputInt == 2)
		{
			List();
		}
	} while (InputInt != 0);
}

void UserManagement::detail_user(User user, vector<User_Role> userRole, bool isEdit)
{
	if (isEdit)
	{
		cout << "User Details : " << endl;
		cout << "1 : " << "Username : \t" + user.Username + "\n";
		cout << "2 : " << "Credentials : \n";
		for (int i = 0; i < userRole.size(); i++)
		{
			auto role = from(db->role) >> first_or_default([&](Role const& a)
			{ return a.ID == userRole.at(i).RoleID; });
			cout << "\t\t" << role.name << endl;
		}
		cout << "3 : " << "Change Password : \n";
	}

	else{
		cout << "User Details : " << endl;
		cout <<"Username : \t" + user.Username + "\n";
		cout << "Credentials : \n";
		for (int i = 0; i < userRole.size(); i++)
		{
			auto role = from(db->role) >> first_or_default([&](Role const& a)
			{ return a.ID == userRole.at(i).RoleID; });
			cout << "\t\t" << role.name << endl;
		}

		if (user.AddedBy.has_value())
		{
			auto aded = from(db->user) >> first_or_default([&](User const& a)
			{ return a.Id == user.AddedBy.value(); });
			cout << "Added by : \t" + aded.Username << endl;
			cout << ((user.IsActive) ? "User is active" : "User Has been disabled") << endl;
		}
	}

}