//
// Created by Adi Iman on 18/04/2020.
//

#include "VesselManagement.h"

string VesselManagement::Deck_Name(int DeckID)
{
	bool check;
	string name;
	auto deckList = from(db->deck) >> where([&](Deck const& a)
	{
	  if (DeckID == 0)
		  return a.VesselID == SelectedVesselID;
	  else
		  return a.VesselID == SelectedVesselID && a.Id != DeckID;
	});

	do
	{
		check = true;
		name = input::getInput("Deck Name");

		if ((deckList
			>> where([&](Deck const& a)
			{ return boost::to_lower_copy(a.Name) == boost::to_lower_copy(name); })
			>> count()) > 0)
		{
			cout << "Deck name already exists. Please enter different name\n";
			check = false;
		}
	} while (!check);

	return name;
}

int VesselManagement::Deck_level(int DeckID)
{
	bool check;
	int level;
	auto deckList = from(db->deck) >> where([&](Deck const& a)
	{
	  if (DeckID == 0)
		  return a.VesselID == SelectedVesselID;
	  else
		  return a.VesselID == SelectedVesselID && a.Id != DeckID;
	});

	do
	{
		check = true;
		level = input::InputInt("Deck Level");

		if ((deckList >> where([&](Deck const& a)
		{ return a.Level == level; }) >> count()) != 0)
		{
			cout << "Deck level already exists. Please enter different name\n";
			check = false;
		}
	} while (!check);

	return level;
}

void VesselManagement::Deck_Premium(long& PremiumID, double& PremiumValue)
{
	auto premium = db->value_type;
	for (int j = 0; j < premium.size(); ++j)
	{
		cout << j + 1 << " : " << premium.at(j).name << endl;
	}
	do
	{
		PremiumID = input::InputInt("Select Premium Type");
	} while (PremiumID > premium.size() && PremiumID < 0);

	PremiumValue = input::InputDouble("Enter Premium Value");
}

void VesselManagement::Deck_AddEdit(int DeckID)
{
	string Header = "Vessel Management : Add New Deck";
	Dependency::ClearScreen(Header);

	Deck deck;
	if (DeckID != 0)
		deck = from(db->deck) >> first_or_default([&](Deck const& x)
		{ return x.Id == DeckID; });
	auto vessel = from(db->vessel) >> first_or_default([&](Vessel const& x)
	{ return x.ID == SelectedVesselID; });

	if (DeckID == 0)
	{
		cout << "Vessel Name : \t" + vessel.name + "\n";
		cout << "Vessel Year : \t" + to_string(vessel.year) + " \n";
		Vessel_VesselDecksDetails(vessel, false);

		cout << "Please enter the following details\n\n";

		deck.Name = Deck_Name(DeckID);
		deck.Level = Deck_level(DeckID);
		deck.MaxPassenger = input::InputInt("Maximum Passenger");
		deck.VesselID = vessel.ID;
	}

	do
	{
		Dependency::ClearScreen(Header);

		cout << "Vessel Name : \t" + vessel.name + "\n";
		cout << "Vessel Year : \t" + to_string(vessel.year) + " \n";
		Vessel_VesselDecksDetails(vessel, false);

		Deck_Details(deck, true);
		string out = "Pick number to make changes,";
		out += ((deck.PremiumID == 0) ? "4 - Add Premium" : "");
		out += "\n\e[32m9 - Save\n\e[31m0 - Cancel\e[39;49m";
		InputInt = input::InputInt(out);

		if (InputInt == 1) deck.Name = Deck_Name(DeckID);
		else if (InputInt == 2) deck.Level = Deck_level(DeckID);
		else if (InputInt == 3) deck.MaxPassenger = input::InputInt("Maximum Passenger");
		else if (InputInt == 4) Deck_Premium(deck.PremiumID, deck.PremiumValue);
	} while (InputInt != 9 && InputInt != 0);

	Dependency::ClearScreen(Header);
	Dependency::EndLine();
	if (InputInt == 9)
	{
		bool inserted = (DeckID == 0) ? db->Insert(deck) : db->Update(deck);
		if (!inserted)
		{
			cout << "DATABASE ERROR\n";
		}
		else
		{
			cout << ((DeckID != 0) ? "DECK UPDATED SUCESSFULLY\n" : "DECK ADDED SUCCESSFULLY\n");
		}
		Dependency::SleepCommand(1000);
	}
}

void VesselManagement::Deck_Delete()
{
	bool chk;
	Deck deck;
	do
	{
		chk = true;
		InputInt = input::InputInt("Select Desired Deck Level to delete");
		auto check = from(db->deck)
			>> where([&](Deck const& a)
			{ return a.VesselID == SelectedVesselID && a.Level == InputInt; });
		if ((check >> count()) == 0)
		{
			cout << "Invalid deck level entered. Please try again\n";
			chk = false;
		}
		else
		{
			deck = check >> first_or_default();
		}
	} while (!chk);

	Dependency::ClearScreen("Vessel Management : Delete Deck");

	cout << "Deck name : \t" + deck.Name + "\n";
	cout << "Deck level : \t" + to_string(deck.Level) + "\n\n";

	do
	{
		InputInt = input::InputInt("\nConfirm to Delete deck?\n 1- Yes 0- Cancel");

		if (InputInt == 1)
		{
			if (!db->Delete(deck))
			{
				cout << "DATABASE ERROR\n";
			}
			else
			{
				cout << "DECK DELETED SUCCESSFULLY\n";
			}
			Dependency::SleepCommand(1000);
		}
	} while (InputInt != 1 && InputInt != 0);
	InputInt = 1;
}