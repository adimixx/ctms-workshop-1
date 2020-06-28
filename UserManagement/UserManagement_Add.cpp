//
// Created by Adi Iman on 06/06/2020.
//

#include "UserManagement.h"
void UserManagement::AddEdit(int id)
{
	string header = "User Management :: ";
	header += (id == 0) ? "Add new User" : "Edit User";
	User user;
	vector<User_Role> selectedRoles;

	if (id != 0)
	{
		user = from(db->user) >> first_or_default([&](User const& a)
		{ return a.Id == id; });
		selectedRoles = from(db->user_role) >> where([&](User_Role const& a)
		{ return a.UserID == user.Id; }) >> to_vector();
	}
	else
	{
		Dependency::ClearScreen(header);

		cout << "Please enter the following details\n\n";
		user.Username = User_Username(user.Id);
		user.Password = md5(input::getInput("Please enter user password"));
		selectedRoles = User_UserRole(selectedRoles, user.Id);
		user.AddedBy = db->loggedInUserID;
	}

	do
	{
		Dependency::ClearScreen(header);
		detail_user(user, selectedRoles, true);

		Dependency::EndLine();
		Dependency::EndLine();
		InputInt = input::InputInt("Pick number to make changes\n\e[32m9 - Save\n\e[31m0 - Cancel\e[39;49m");

		if (InputInt == 1) user.Username = User_Username(user.Id);
		else if (InputInt == 2) selectedRoles = User_UserRole(selectedRoles, user.Id);
		else if (InputInt == 3) user.Password = md5(input::getInput("Please enter user password"));
	} while (InputInt != 9 && InputInt != 0);

	Dependency::ClearScreen(header);
	Dependency::EndLine();
	if (InputInt == 9)
	{
		bool inserted = (id == 0) ? db->Insert(user, selectedRoles) : db->Update(user, selectedRoles);
		if (!inserted)
		{
			cout << "DATABASE ERROR\n";
		}
		else
		{
			cout << ((user.Id == 0) ? "USER ADDED SUCESSFULLY" : "USER UPDATED SUCESSFULLY") << endl;
		}
		Dependency::SleepCommand(1000);
	}
	InputInt = 1;
}

string UserManagement::User_Username(int id)
{
	string name;
	bool check;
	do
	{
		check = true;
		name = input::getInput("Enter Username");
		if ((from(db->user) >> where([&](User const& a)
		{ return boost::to_lower_copy(a.Username) == boost::to_lower_copy(name) && a.Id != id; }) >> count()) != 0)
		{
			cout << "Username already exists. Please enter different Username\n";
			check = false;
		}
	} while (!check);
	return name;
}

vector<User_Role> UserManagement::User_UserRole(vector<User_Role> us, int userID)
{

	int selectedRole;
	do
	{
		auto haveRole = from(us) >> select([&](User_Role const& a)
		{ return a.RoleID; }) >> to_vector();
		auto availableRole = from(db->role) >> where([&](Role const& a)
		{ return a.ID != *find(haveRole.begin(), haveRole.end(), a.ID); }) >> to_vector();

		cout << endl << endl << "User Roles : " << endl;
		for (int j = 0; j < us.size(); j++)
		{
			cout << j + 1 << " : " << (from(db->role) >> first_or_default([&](Role const& a)
			{ return a.ID == us.at(j).RoleID; })).name << endl;
		}

		cout << endl << "Roles : " << endl;
		for (int j = 0; j < availableRole.size(); j++)
		{
			cout << j + 1 << " : " << availableRole.at(j).name << endl;
		}

		InputString = input::getInput("Select Role Number, a : Delete selected Role, 0 : done making changes");
		try
		{
			selectedRole = stoi(InputString);

			if (selectedRole > 0 && selectedRole <= availableRole.size())
			{
				auto sel = availableRole.at(selectedRole - 1);
				if ((from(us) >> first_or_default([&](User_Role const& a)
				{ return a.RoleID == sel.ID; })).Id > 0)
				{
					cout << "The role is already added in the list!" << endl << endl;
				}
				else
				{
					User_Role usrole;
					usrole.Id = 0;
					usrole.UserID = userID;
					usrole.RoleID = sel.ID;
					us.emplace_back(usrole);
				}
			}

			if (selectedRole == 0 && us.empty())
			{
				cout << "Please select at least a role" << endl << endl;
				selectedRole = 1;
			}
		}
		catch (std::exception e)
		{
			if (boost::to_lower_copy(InputString) == "a")
			{
				bool found;
				InputInt = input::InputInt("Select role number to delete, 0 to cancel", 0, us.size());

				if (InputInt != 0)
				{
					us.erase(us.begin() + (InputInt - 1));
				}
			}
		}
	} while (selectedRole != 0);
	return us;
}