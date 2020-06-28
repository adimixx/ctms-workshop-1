//
// Created by Adi Iman on 07/06/2020.
//

#include "UserManagement.h"

void UserManagement::List()
{
	do
	{
		auto users = db->user;
		TextTable t('-', '|', '+');
		t.add("Num");
		t.add("Username");
		t.endOfRow();

		for (int i = 0; i < users.size(); i++)
		{
			t.add(to_string(i + 1));
			t.add(users.at(i).Username);
			t.endOfRow();
		}

		Dependency::ClearScreen("User Management : User List");
		cout << t << endl;
		InputInt = input::InputInt("\n\nNumber to select user, 0 - Back", 0, db->user.size());

		if (InputInt != 0)
		{
			Detail(users.at(InputInt - 1).Id);
			InputInt = 1;
		}
	} while (InputInt != 0);
}

void UserManagement::Detail(int id)
{
	do
	{
		auto user = from(db->user) >> first_or_default([&](User const& a)
		{ return a.Id == id; });
		auto roles = from(db->user_role) >> where([&](User_Role const& a)
		{ return a.UserID == user.Id; }) >> to_vector();
		Dependency::ClearScreen("User Management : User Details");

		detail_user(user, roles, false);
		string actDel = (user.IsActive) ? "Delete / Deactivate User" : "Activate User";
		InputInt = input::InputInt("\n\n1- Edit, 2-" + actDel + " , 0-Back", 0, 2);

		if (InputInt == 1)
		{
			AddEdit(id);
			InputInt = 1;
		}

		else if (InputInt == 2)
		{
			if (user.IsActive)
				DeactDelete(user);
			else
				Activate(user);
			InputInt = 1;
		}
	} while (InputInt != 0);
}

void UserManagement::DeactDelete(User user)
{
	if (user.Id == db->loggedInUserID)
	{
		cout << "Cannot delete your own account" << endl;
		Dependency::SleepCommand(1000);
	}

	else
	{
		InputInt = input::InputInt("Are you sure you want to Delete / Deactivate this user?\n1-Yes, 0-No", 0, 1);

		if (InputInt == 1)
		{
			auto checkReceipt = from(db->receipt) >> where([&](Receipt const& a)
			{ return a.purchaseUserID == user.Id; }) >> count();
			auto checkTicket = from(db->ticket) >> where([&](Ticket const& a)
			{ return a.checkInUserID == user.Id; }) >> count();

			if (checkReceipt != 0 || checkTicket != 0)
			{
				cout << "User has performed transaction in this system. User will be deactivated" << endl;
				user.IsActive = false;
				if (!db->Update(user))
				{
					cout << "DATABASE ERROR\n";
				}
				else
				{
					cout << "\nUser Deactivated SUCCESSFULLY\n";
					InputInt = 0;
				}
			}
			if (!db->Delete(user))
			{
				cout << "DATABASE ERROR\n";
			}
			else
			{
				cout << "\nUser DELETED SUCCESSFULLY\n";
				InputInt = 0;
			}
			Dependency::SleepCommand(1000);
		}
		else
		{
			InputInt = 1;
		}
	}
}

void UserManagement::Activate(User user)
{
	InputInt = input::InputInt("Are you sure you want to Activate this user?\n1-Yes, 0-No", 0, 1);

	if (InputInt == 1)
	{
		user.IsActive = true;
		if (!db->Update(user))
		{
			cout << "DATABASE ERROR\n";
		}
		else
		{
			cout << "\nUser Activated SUCCESSFULLY\n";
		}
		Dependency::SleepCommand(1000);
	}
	InputInt = 1;
}