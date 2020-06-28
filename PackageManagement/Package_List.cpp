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
			TextTable t('-', '|', '+');
			t.add("Num");
			t.add("Package Name");
			t.add("Departure - Return Date");
			t.endOfRow();
			for (int j = 0; j < packages.size(); j++)
			{
				auto pack = packages.at(j);
				t.add(to_string(j+1));
				t.add(pack.Name);
				t.add(pack.start_date + " - " + pack.end_date);
				t.endOfRow();
			}

			cout<<t<<endl;
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
		detail_ticketList(pack);

		Dependency::EndLine();
		Dependency::EndLine();

		InputInt = input::InputInt("1- Edit Package Details, 2- Delete Package, 0- Back");

		if (InputInt == 1)
		{
			AddEdit(pack.ID);
		}
		else if (InputInt == 2)
		{
			SelectedPackageID = pack.ID;
			Delete();
		}
	} while (InputInt != 0);
	InputInt = 1;
}

void PackageManagement::Delete()
{
	auto packages = from(db->ticket) >> where([&](Ticket const& a)
	{ return a.packageID == SelectedPackageID; }) >> count();

	if (packages != 0)
	{
		cout << "This Package cannot be deleted as it has customers associated to the package" << endl;
	}

	else{
		InputInt = input::InputInt("Are you sure you want to Delete this Package?\n1-Yes, 0-No", 0, 1);

		if (InputInt == 1)
		{
			auto pkg = from(db->package) >> first_or_default([&](Package const& v)
			{ return v.ID == SelectedPackageID; });
			if (!db->Delete(pkg))
			{
				cout << "DATABASE ERROR\n";
				InputInt = 1;
			}
			else
			{
				cout << "\nVessel DELETED SUCCESSFULLY\n";
				InputInt = 0;
			}
		}
	}
	Dependency::SleepCommand(1000);
}

void PackageManagement::detail_package(Package pack, bool showInd)
{
	cout << "Package Details : " << endl;
	cout << ((showInd) ? "1 : " : "") << "Package Name : \t" + pack.Name + "\n";
	cout << ((showInd) ? "2 : " : "") << "Departure Date : \t" + pack.start_date + "\n";
	cout << ((showInd) ? "    " : "") << "Return Date:  \t" + pack.end_date + "\n";
	cout << ((showInd) ? "3 : " : "") << "Price :  RM " + input::toString(pack.price,2) + "\n";
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

void PackageManagement::detail_ticketList(Package pack)
{
	auto ticket = from(db->ticket)>>where([&](const Ticket &x){return x.packageID == pack.ID; })>>to_vector();

	if (!ticket.empty()){
		TextTable t('-', '|', '+');
		t.add("Num");
		t.add("Name");
		t.add("Phone");
		t.add("Email");
		t.add("Deck Name");
		t.add("Deck Level");
		t.add("Price & Ticket Type");
		t.endOfRow();

		for (int j = 0; j < ticket.size(); ++j)
		{
			auto tick = ticket.at(j);
			auto ticketType = from(db->ticket_type)
				>> first_or_default([&](Ticket_Type const& x)
				{ return x.ID == tick.ticketTypeID; });
			auto deck = from(db->deck) >> first_or_default([&](Deck const& a)
			{ return a.Id == tick.deckID; });

			t.add(to_string(j + 1));
			t.add(tick.name);
			t.add(tick.phone);
			t.add(tick.email);
			t.add(deck.Name);
			t.add(to_string(deck.Level));
			t.add("RM " + input::toString(tick.price, 2) + "(" + ticketType.name + ")");
			t.endOfRow();
		}
		t.setAlignment(4, TextTable::Alignment::RIGHT);

		cout << "TICKET LIST : " << endl << t << endl;
	}
}