//
// Created by Adi Iman on 18/04/2020.
//

#include "VesselManagement.h"

VesselManagement::VesselManagement(CtmsDB* db)
{
	this->db = db;
}

void VesselManagement::Vessel_VesselDetails(Vessel vessel)
{
	Dependency::EndLine();
	cout << "1. Vessel Name : \t" + vessel.name + "\n";
	cout << "2. Gross Tonne (GT) : \t" + to_string(vessel.gt) + " tonne\n";
	cout << "3. Deadweight tonnage (DWT) :  \t" + to_string(vessel.dwt) + " tonne\n";
	cout << "4. Length :  \t" + to_string(vessel.length) + " metre\n";
	cout << "5. Breadth : \t" + to_string(vessel.breadth) + " metre\n";
	cout << "6. Vessel Year : \t" + to_string(vessel.year) + " \n";
	Dependency::EndLine();
}

int VesselManagement::Vessel_VesselDecksDetails(Vessel vessel, bool showDet)
{
	auto decks = from(db->deck) >> where([&](Deck const& x)
	{ return x.VesselID == vessel.ID; }) >> orderby_ascending([&](Deck const& x)
	{ return x.Level; }) >> to_list();

	cout << "Number of Decks : \t" + to_string(decks.size()) + " \n";
	Dependency::EndLine();

	if (showDet)
	{
		for (auto b : decks)
		{
			Dependency::EndLine();
			Deck_Details(b, false);
		}
	}
	return decks.size();
}

void VesselManagement::Deck_Details(Deck b, bool viewIndex)
{
	cout << ((viewIndex) ? "1. " : "") << "Deck Name : " << b.Name << endl;
	cout << ((viewIndex) ? "2. " : "") << "Level " + to_string(b.Level) << endl;
	cout << ((viewIndex) ? "3. " : "") << "Maximum Passenger: " + to_string(b.MaxPassenger) << endl;

	if (b.PremiumID != 0)
	{
		cout << ((viewIndex) ? "4. " : "") << "Premium : " << endl;
		cout << "Premium Type : \t" + (from(db->value_type) >> first_or_default([&](Value_Type const& x)
		{ return x.ID == b.PremiumID; })).name << endl;
		cout << "Premium value : \t" + to_string(b.PremiumValue) << endl;
	}
}

void VesselManagement::Menu()
{
	do
	{
		Dependency::ClearScreen("Vessel Management");

		cout << "1. Add New Vessel\n";
		cout << "2. List Vessel\n\n";

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