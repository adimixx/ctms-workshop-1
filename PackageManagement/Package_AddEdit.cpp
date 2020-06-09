//
// Created by Adi Iman on 18/04/2020.
//

#include "PackageManagement.h"
#include <algorithm>

void PackageManagement::AddEdit(int PackageID)
{
	if (db->route.empty() || db->vessel.empty())
	{
		if (db->route.empty()) cout << "No Routes Available. Please add routes first !" << endl;
		if (db->vessel.empty()) cout << "No Vessel Available. Please add routes first !" << endl;
	}

	else
	{
		string header = "Package Management :: ";
		header += (PackageID == 0) ? "Add new Package" : "Edit Package";
		Package package;
		vector<Package_Route> routes;
		if (PackageID != 0)
		{
			package = from(db->package) >> first_or_default([&](Package const& a)
			{ return a.ID == PackageID; });
			routes = from(db->package_route) >> where([&](Package_Route const& a)
			{ return a.PackageID == PackageID; })
											 >> to_vector();
			package.start_date = input::ConvertDateAndTime(package.start_date, db->db);
			package.end_date = input::ConvertDate(package.end_date, db->db);
		}
		else
		{
			Dependency::ClearScreen(header);

			cout << "Please enter the following details\n\n";
			package.Name = Package_Name(0);
			Package_Date(package.start_date, package.end_date, 0, package.vesselID);
			package.price = input::InputDouble("Base Price", 0);
			routes = Package_Routes(0);
		}

		do
		{
			auto vessel =
				from(db->vessel) >> first_or_default([&](Vessel const& a)
				{ return a.ID == package.vesselID; });

			Dependency::ClearScreen(header);
			detail_package(package, true);
			Dependency::EndLine();
			detail_route(routes, true);
			Dependency::EndLine();
			detail_vessel(vessel, false);

			Dependency::EndLine();
			Dependency::EndLine();
			InputInt = input::InputInt("Pick number to make changes\n\e[32m9 - Save\n\e[31m0 - Cancel\e[39;49m");

			if (InputInt == 1) package.Name = Package_Name(package.ID);
			else if (InputInt == 2) Package_Date(package.start_date, package.end_date, 0, package.vesselID);
			else if (InputInt == 3) package.price = input::InputDouble("Base Price", 0);
			else if (InputInt == 4) routes = Package_Routes(package.ID);
			else if (InputInt == 5)
			{
				package.vesselID = Package_Vessel(package.start_date, package.end_date, package.ID);
				if (package.vesselID == 0)
				{
					Package_Date(package.start_date, package.end_date, 0, package.vesselID);
				}
			}
		} while (InputInt != 9 && InputInt != 0);

		Dependency::ClearScreen(header);
		Dependency::EndLine();
		if (InputInt == 9)
		{
			bool inserted = false;
			if (PackageID == 0)
			{
				inserted = db->Insert(package, routes);
			}
			else
			{
				inserted = db->Update(package, routes);
			}

			if (!inserted)
			{
				cout << "DATABASE ERROR\n";
			}
			else
			{
				cout << ((PackageID == 0) ? "PACKAGE ADDED SUCESSFULLY" : "PACKAGE UPDATED SUCESSFULLY") << endl;
			}
			Dependency::SleepCommand(1000);
		}
	}
}

string PackageManagement::Package_Name(int packID)
{
	string name;
	bool check;

	do
	{
		check = true;
		name = input::getInput("Package Name");

		if ((from(db->package)
			>> where([&](Package const& a)
			{
			  bool b = boost::to_lower_copy(a.Name) == boost::to_lower_copy(name);
			  if (packID != 0) return (a.ID != packID) && b;
			  else return b;
			})
			>> count()) != 0)
		{
			cout << "Package name already exists. Please enter different name\n";
			check = false;
		}
	} while (!check);
	return name;
}

long PackageManagement::Package_Vessel(string StartDate, string EndDate, int packageID)
{
	auto activeVessels = from(db->vessel) >> where([&](Vessel const& a)
	{ return a.isActive; }) >> to_vector();
	auto occupiedVessels = from(db->package) >> where([&](Package const& a)
	{
	  return input::compareDate(StartDate, input::ConvertDate(a.end_date, db->db), db->db, false) &&
		  input::compareDate(input::ConvertDate(a.start_date, db->db), EndDate, db->db, false) && a.ID != packageID;
	}) >> select([&](Package const& a)
	{ return a.vesselID; }) >> to_vector();

	auto Vessels = from(activeVessels) >> skip_while([&](Vessel const& a)
	{ return (find(occupiedVessels.begin(), occupiedVessels.end(), a.ID) != occupiedVessels.end()); })
									   >> to_vector();
	if ((Vessels.size()) == 0)
	{
		cout << "There are no available vessels on the date. Please select other date\n\n";
		return 0;
	}
	else
	{
		Vessel selectedves;
		do
		{
			cout << "Available Vessels : \n";
			for (int j = 0; j < Vessels.size(); ++j)
			{
				cout << j + 1 << " : " << Vessels.at(j).name << endl;
			}
			Dependency::EndLine();
			InputInt = input::InputInt("Select number of vessel. 0 to re-enter date");

			if (InputInt != 0 && InputInt <= Vessels.size())
			{
				selectedves = Vessels.at(InputInt - 1);
				detail_vessel(selectedves, false);

				InputInt = input::InputInt("Confirm this Vessel? 1- Yes, 0- No", 0, 1);
				if (InputInt == 0) InputInt = 2;
			}
		} while (InputInt != 0 && InputInt != 1);

		if (InputInt == 1) return selectedves.ID;
		else return 0;
	}
}

void PackageManagement::Package_Date(string& StartDate, string& EndDate, int packageID, long& VesselID)
{
	do
	{
		bool checkDate;
		do
		{
			checkDate = true;
			StartDate = input::InputDateWithTime("Departure Date (dd/MM/yyyy)", db->db);
			if (!input::compareDate(input::AddDay("", 3, db->db), StartDate, db->db, false))
			{
				cout << "Departure Date must be at least 3 days from today" << endl << endl;
				checkDate = false;
			}
		} while (!checkDate);
		do
		{
			checkDate = true;
			EndDate = input::InputDate("Return Date (dd/MM/yyyy)", db->db);
			if (!input::compareDate(StartDate, EndDate, db->db, false))
			{
				cout << "Arrival Date must be greater than Departure Date" << endl << endl;
				checkDate = false;
			}
		} while (!checkDate);

		VesselID = Package_Vessel(StartDate, EndDate, packageID);
	} while (VesselID == 0);
}

vector<Package_Route> PackageManagement::Package_Routes(int packageID)
{
	vector<Package_Route> route;
	int selectedRoute;
	if (packageID != 0)
		route = from(db->package_route) >> where([&](Package_Route const& a)
		{ return a.PackageID == packageID; })
										>> to_vector();
	do
	{
		auto haveRoutes = from(route) >> select([&](Package_Route const& a)
		{ return a.RouteID; }) >> to_vector();
		auto availableRoutes = from(db->route) >> where([&](Route const& a)
		{ return a.ID != *find(haveRoutes.begin(), haveRoutes.end(), a.ID); }) >> to_vector();

		detail_route(route, false);
		cout << endl << endl << "Available Routes : " << endl;
		for (int j = 0; j < availableRoutes.size(); j++)
		{
			cout << j + 1 << " : " << availableRoutes.at(j).name << endl;
		}
		InputString = input::getInput("Select Route Number, a : Delete selected Route, 0 : done making changes");
		try
		{
			selectedRoute = stoi(InputString);

			if (selectedRoute > 0 && selectedRoute <= availableRoutes.size())
			{
				auto sel = availableRoutes.at(selectedRoute - 1);
				if ((from(route) >> first_or_default([&](Package_Route const& a)
				{ return a.RouteID == sel.ID; })).ID > 0)
				{
					cout << "The route is already added in the list!" << endl << endl;
				}
				else
				{
					route.emplace_back(0, sel.ID, packageID);
				}
			}

			else if (selectedRoute == 0 && route.empty()){
				cout << "Route list is empty. Please add at least one route!" << endl << endl;
				Dependency::SleepCommand(1000);
				selectedRoute = 1;
			}
		}
		catch (std::exception e)
		{
			if (boost::to_lower_copy(InputString) == "a")
			{
				bool found;
				InputInt = input::InputInt("Select Route Number to delete, 0 to cancel", 0, route.size());

				if (InputInt != 0)
				{
					route.erase(route.begin() + (InputInt - 1));
				}
			}
		}
	} while (selectedRoute != 0);
	return route;
}

