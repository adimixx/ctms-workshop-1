#include "Core.h"

CtmsDB* db;

User Login(), LoggedInUser;

long MainMenu();

int InputInt;

int main()
{
	Database DatabaseConn;

	if (!DatabaseConn.ConnectionFunction())
	{
		return 0;
	}

	do
	{
		db = new CtmsDB(DatabaseConn);
		db->loggedInUserID = 0;
		LoggedInUser = Login();
	} while (MainMenu() != 0);
}

User Login()
{
	string username, password;
	bool LoggedIn = false;
	User checkUser;
	Dependency::ClearScreen("CTMS :: LOGIN");
	do
	{
		username = input::getInput("Username");

		Dependency::SetStdinEcho(false);
		password = input::getInput("Password");
		Dependency::SetStdinEcho(true);

		Dependency::ClearScreen("CTMS :: LOGIN");

		checkUser = from(db->user)
			>> where([&](User const& a)
			{ return a.Username == username && a.Password == md5(password); })
			>> first_or_default();
		if (checkUser.Id == 0)
		{
			cout << "Invalid Username or Password. Please Try Again\n\n";
		}
		else if (checkUser.IsActive == false)
		{
			cout << "Your account has been disabled. Please contact Administrator\n\n";
		}
		else
		{
			db->loggedInUserID = checkUser.Id;
			LoggedIn = true;
		}
	} while (!LoggedIn);
	return checkUser;
}

long MainMenu()
{
	do
	{
		TicketManagement ticketManagement(db);
		bool isStaff, isAdmin;
		Dependency::ClearScreen("CRUISE TICKETING MANAGEMENT SYSTEM\n");
		cout << "WELCOME " + LoggedInUser.Username + "\n\n\n";
		if ((from(db->user_role) >> where([&](User_Role const x)
		{ return x.RoleID == 2 && x.UserID == LoggedInUser.Id; })
								 >> count()) == 1)
		{
			isStaff = true;
			cout << "TICKETING MENU\n";
			cout << "1. Open Ticket Counter\n";
			cout << "2. Check - In Ticket\n\n";
		}

		if ((from(db->user_role) >> where([&](User_Role const x)
		{ return x.RoleID == 1 && x.UserID == LoggedInUser.Id; })
								 >> count()) == 1)
		{
			isAdmin = true;
			cout << "ADMINISTRATOR MENU\n";
			cout << "3. Route Management\n";
			cout << "4. Vessel Management\n";
			cout << "5. Package Management\n";
			cout << "6. User Management\n";
			cout << "7. System Reporting\n";
			cout << "\n\n";
		}

		cout << "0. Logout\n";

		InputInt = input::InputInt("Enter your input");

		if (isStaff && InputInt == 1)
		{
			ticketManagement.TicketCounter();
		}
		else if (isStaff && InputInt == 2)
		{
			ticketManagement.TicketCheckIn();
		}
		else if (isAdmin && InputInt == 3)
		{
			RouteManagement route(db);
			route.Menu();
		}

		else if (isAdmin && InputInt == 4)
		{
			VesselManagement vessel(db);
			vessel.Menu();
		}
		if (isAdmin && InputInt == 5)
		{
			PackageManagement packageManagement(db);
			packageManagement.Menu();
		}
		if (isAdmin && InputInt == 6)
		{
			UserManagement userManagement(db);
			userManagement.Menu();
		}
		if (isAdmin && InputInt == 7)
		{
			Reporting reporting(db);
			reporting.List();
		}
	} while (InputInt != 0);

	Dependency::ClearScreen("LOGGING OUT :: CTMS\n");
	string Input = input::getInput("Press any key to Re-Login, 0 to Exit");
	if (Input == "0") return 0;
	return 1;
}
