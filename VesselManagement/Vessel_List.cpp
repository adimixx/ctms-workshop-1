//
// Created by Adi Iman on 18/04/2020.
//

#include "VesselManagement.h"

void VesselManagement::List()
{
	do
	{
		Dependency::ClearScreen("Vessel Management : Vessel List");

		for (int j = 0; j < db->vessel.size(); ++j)
		{
			Dependency::EndLine();
			auto ves = db->vessel.at(j);
			string active = (ves.isActive) ? "Active" : "Not Active";
			cout << j + 1 << " : " + ves.name + "\nRegister Date : " + ves.registerDate + "\nStatus : " + active;
			Dependency::EndLine();
		}

		InputInt = input::InputInt("\n\nNumber to select Vessel, 0 - Back", 0, db->vessel.size());

		if (InputInt != 0)
		{
			SelectedVesselID = db->vessel.at(InputInt - 1).ID;
			Detail();
		}
	} while (InputInt != 0);
	InputInt = 1;
}

void VesselManagement::Detail()
{
	do
	{
		int totalDecks;
		auto ves = from(db->vessel) >> first_or_default([&](Vessel const& v)
		{ return v.ID == SelectedVesselID; });

		string active = (ves.isActive) ? "Active" : "Not Active";
		string active2 = (ves.isActive) ? "Deactivate" : "Activate";

		Dependency::ClearScreen("Vessel Management : Vessel Detail");

		TextTable t('-', '|', '+');
		t.add("7. Register Date");
		t.add(ves.registerDate);
		t.endOfRow();
		t.add("8. Status");
		t.add(active);
		t.endOfRow();

		Vessel_VesselDetails(ves);
		cout << t;

		cout << "Deck Details : \n";
		totalDecks = Vessel_VesselDecksDetails(ves, true);

		Dependency::EndLine();
		Dependency::EndLine();
		InputInt =
			input::InputInt("1- Edit, 2- Delete, 3-" + active2 + " Vessel\n4- Add New Deck, 5- Delete Deck\n0-Back");

		if (InputInt == 1)
		{
			AddEdit(ves.ID);
		}
		else if (InputInt == 2)
		{
			Vessel_Delete();
		}
		else if (InputInt == 3)
		{
			Activation(ves);
		}
		else if (InputInt == 4)
		{
			Deck_AddEdit(0);
		}
		else if (totalDecks != 0 && InputInt == 5)
		{
			Deck_Delete();
		}
	} while (InputInt != 0);
	InputInt = 1;
}

void VesselManagement::Activation(Vessel ves)
{
	bool okay = true;

	if (ves.isActive){
		auto occupiedVessels = from(db->package) >> where([&](Package const& a)
		{
		  return input::compareDate("",input::ConvertDate(a.start_date, db->db) , db->db, false) && a.vesselID == ves.ID;
		}) >> to_vector();

		if (occupiedVessels.size() > 0)
		{
			TextTable t('-', '|', '+');
			t.add("Package Name");
			t.add("Package Date");
			t.endOfRow();

			for (int i = 0; i < occupiedVessels.size(); i++)
			{
				t.add(occupiedVessels.at(i).Name);
				t.add(occupiedVessels.at(i).start_date);
				t.endOfRow();
			}

			cout << "Cannot deactivate this vessel. This vessel is being used on the following packages : " << endl << t << endl;
			okay = false;
		}
	}

	if (okay){
		string active = (!ves.isActive) ? "Activate" : "Deactivate";
		InputInt = input::InputInt("Are you sure you want to " + active + " this vessel?\n1-Yes, 0-No", 0, 1);
		if (InputInt == 1)
		{
			ves.isActive = !ves.isActive;

			if (!db->Update(ves))
			{
				cout << "DATABASE ERROR\n";
			}
			else
			{
				cout << "\nVessel " + active + "d" + " Successfully\n";
			}
		}
	}

	Dependency::SleepCommand(3000);
	InputInt = 1;
}
