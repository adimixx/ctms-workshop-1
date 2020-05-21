//
// Created by Adi Iman on 18/04/2020.
//
#include "PackageManagement.h"

PackageManagement::PackageManagement(CtmsDB* db)
{
	this->db = db;
}

void PackageManagement::Menu()
{
	do
	{
		Dependency::ClearScreen("Package Management");

		cout << "1. Add New Package\n";
		cout << "2. List Packages\n\n";

		cout << "0. Main Menu\n\n";

		InputInt = input::InputInt("Enter your input");

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
