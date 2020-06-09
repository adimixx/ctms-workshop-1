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

	TextTable t('-', '|', '+');

	t.add("1. Vessel Name");
	t.add(vessel.name);
	t.endOfRow();

	t.add("2. Gross Tonne (GT)");
	t.add(input::toString(vessel.gt,4)+ " tonne");
	t.endOfRow();

	t.add("3. Deadweight tonnage (DWT)");
	t.add(input::toString(vessel.dwt,4) + " tonne");
	t.endOfRow();

	t.add("4. Length");
	t.add(input::toString(vessel.length,3)  + " meter");
	t.endOfRow();

	t.add("5. Breadth");
	t.add(input::toString(vessel.breadth,3) + " meter");
	t.endOfRow();

	t.add("6. Vessel Year");
	t.add(to_string(vessel.year));
	t.endOfRow();

	cout << t;
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
		TextTable t('-', '|', '+');
		t.add("Deck Name");
		t.add("Level");
		t.add("Maximum Passenger");
		t.add("Premium Type");
		t.add("Premium value ");
		t.endOfRow();

		for (auto b : decks)
		{
			t.add(b.Name);
			t.add(to_string(b.Level));
			t.add(to_string(b.MaxPassenger));
			if (b.PremiumID == 0)
			{
				t.add("-");
				t.add("-");
			}
			else
			{
				t.add((from(db->value_type) >> first_or_default([&](Value_Type const& x)
				{ return x.ID == b.PremiumID; })).name);
				t.add(input::toString(b.PremiumValue,2));
			}
			t.endOfRow();

		}

		cout<<t;
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
		cout << "Premium value : \t" + input::toString(b.PremiumValue,2) << endl;
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