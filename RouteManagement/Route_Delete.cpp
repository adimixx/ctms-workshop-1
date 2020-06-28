//
// Created by Adi Iman on 18/04/2020.
//
#include "RouteManagement.h"

void RouteManagement::Delete()
{
	Dependency::ClearScreen("Route Management : Delete Route");
	cout << "Route Name : " << db->route.at(SelectedIndex).name << endl;

	auto used = from(db->package_route) >> where([&](Package_Route const& a)
	{ return a.RouteID == db->route.at(SelectedIndex).ID; }) >> to_vector();

	if (!used.empty())
	{
		cout << "ROUTE CANNOT BE DELETED!. IT IS CURRENTLY BEEN USED BY OTHER PACKAGE\n";
	}

	else
	{
		do
		{
			InputInt = input::InputInt("Are you sure you want to Delete this route?\n1-Yes, 0-No");
		} while (InputInt != 1 && InputInt != 0);

		if (InputInt == 1)
		{
			if (!db->Delete(db->route.at(SelectedIndex)))
			{
				Dependency::ClearScreen("Route Management : Delete Route");
				cout << "DATABASE ERROR\n";
			}

			else
			{
				cout << "\nROUTE DELETED SUCCESSFULLY\n";
			}
			Dependency::SleepCommand(1000);
		}
	}
}



