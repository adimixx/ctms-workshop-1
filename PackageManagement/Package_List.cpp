//
// Created by Adi Iman on 18/04/2020.
//
#include "PackageManagement.h"

void PackageManagement::List()
{
	Dependency::ClearScreen("Package Management :: Package List");

	if (db->package.empty())
	{
		cout << "No Packages Available" << endl;
		Dependency::SleepCommand(1000);
	}
	else
	{
		do
		{
			auto packages = db->package;

			for (int j = 0; j < packages.size(); j++)
			{
				auto pack = packages.at(j);
				cout << j + 1 << " : " << pack.Name << "\nDate : " << pack.start_date + " - " + pack.end_date << endl
					 << endl;
			}

			InputInt = input::InputInt("\n\nNumber to select package, 0 - Back");

			if (InputInt > packages.size())
			{
				Dependency::ClearScreen("Package Management :: Package List");
				cout << "Invalid package selected. Please enter the right number" << endl;
			}
			else if (InputInt > 0)
			{
				SelectedPackageID = packages.at(InputInt - 1).ID;
				Detail();
				Dependency::ClearScreen("Package Management :: Package List");
			}
		} while (InputInt != 0);
		InputInt = 1;
	}
}

void PackageManagement::Detail()
{
	do
	{
		auto pack = from(db->package) >> first_or_default([&](Package const& a)
		{ return a.ID == SelectedPackageID; });
		auto vessel = from(db->vessel) >> first_or_default([&](Vessel const& a)
		{ return a.ID == pack.vesselID; });
		auto route = from(db->package_route) >> where([&](Package_Route const& a)
		{ return a.PackageID == pack.ID; }) >> orderby_ascending([&](Package_Route const& a)
		{ return a.ID; }) >> to_vector();
		Dependency::ClearScreen("Package Management :: Package Details");

		detail_package(pack, false);
		Dependency::EndLine();
		detail_route(route, false);
		Dependency::EndLine();
		detail_vessel(vessel, false);

		Dependency::EndLine();
		Dependency::EndLine();

		InputInt = input::InputInt("1- Edit Package Details, 2- Delete Vessel, 0- Back");

		if (InputInt == 1)
		{
			AddEdit(pack.ID);
		}
		else if (InputInt == 2)
		{
			SelectedPackageID = pack.ID;
//            Vessel_Delete();
		}
	} while (InputInt != 0);
	InputInt = 1;
}

void PackageManagement::detail_package(Package pack, bool showInd)
{
	cout << "Package Details : " << endl;
	cout << ((showInd) ? "1 : " : "") << "Package Name : \t" + pack.Name + "\n";
	cout << ((showInd) ? "2 : " : "") << "Start Date : \t" + pack.start_date + " tonne\n";
	cout << ((showInd) ? "    " : "") << "End Date:  \t" + pack.end_date + " tonne\n";
	cout << ((showInd) ? "3 : " : "") << "Price :  RM\t" + to_string(pack.price) + " metre\n";
}

void PackageManagement::detail_route(vector<Package_Route> route, bool showInd)
{
	cout << ((showInd) ? "4 : " : "") << "Package Routes : " << endl;
	for (int j = 0; j < route.size(); ++j)
	{
		cout << j + 1 << " : "
			 << (from(db->route) >> first_or_default([&](Route const& x)
			 { return x.ID == route.at(j).RouteID; })).name << endl;
	}
}

void PackageManagement::detail_vessel(Vessel vessel, bool showInd)
{
	auto vesselDeck = from(db->deck) >> where([&](Deck const& x)
	{ return x.VesselID == vessel.ID; })
									 >> orderby_ascending([&](Deck const& x)
									 { return x.Level; });
	cout << ((showInd) ? "5 : " : "") << "Vessel for the Package : " << endl;
	cout << "Vessel Name : \t" + vessel.name + "\n";
	cout << "Vessel Year : \t" + to_string(vessel.year) + " \n";
	cout << "Number of Decks : \t" + to_string(vesselDeck >> count()) + " \n";
	cout << "Maximum Capacity : \t" + to_string(vesselDeck >> sum([&](Deck const& x)
	{ return x.MaxPassenger; })) +
		" Passengers\n";
}
