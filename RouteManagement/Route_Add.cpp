//
// Created by Adi Iman on 18/04/2020.
//
#include "RouteManagement.h"

void RouteManagement::Add()
{
	bool check;
	string name;

	Dependency::ClearScreen("Route Management : Add New Route");
	cout << "Please enter the following details\n\n";

	do
	{
		check = true;
		name = input::getInput("Route Name");

		if ((from(db->route) >> where([&](Route const& x)
		{ return boost::to_lower_copy(x.name) == boost::to_lower_copy(name); }) >> count()) != 0)
		{
			cout << "Vessel name already exists. Please enter different name\n";
			check = false;
		}
	} while (!check);

	Dependency::ClearScreen("Route Management : Add New Route");
	cout << "\nReview input : \n\n";
	cout << "Route Name : \t" + name + "\n";

	do
	{
		InputInt = input::InputInt("\nConfirm to add new Route?\n 1- Yes 0- Cancel");
	} while (InputInt != 1 && InputInt != 0);

	if (InputInt == 1)
	{
		name = input::sanitizeString(name);
		if (!db->Insert(Route(0, name)))
		{
			Dependency::ClearScreen("Route Management : Add New Route");
			cout << "DATABASE ERROR\n";
		}

		else
		{
			cout << "\nROUTE ADDED SUCCESSFULLY\n";
		}
		Dependency::SleepCommand(1000);
	}
}

