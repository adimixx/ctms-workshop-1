//
// Created by Adi Iman on 18/04/2020.
//
#include "VesselManagement.h"
void VesselManagement::AddEdit(int VesselID)
{
	Vessel vessel;
	string Header = "Vessel Management : ";
	Header += (VesselID == 0) ? "Add New Vessel" : "Edit Vessel Details";
	Dependency::ClearScreen(Header);

	if (VesselID != 0)
		vessel = from(db->vessel) >> first_or_default([&](Vessel const& a)
		{ return a.ID == VesselID; });
	else
	{
		cout << "Please enter the following details\n\n";
		vessel.name = Vessel_Name(0);
		vessel.gt = Vessel_GT();
		vessel.dwt = Vessel_DWT();
		vessel.length = Vessel_Length();
		vessel.breadth = Vessel_Breadth();
		vessel.year = Vessel_Year();
	}

	do
	{
		Dependency::ClearScreen(Header);
		Vessel_VesselDetails(vessel);
		InputInt = input::InputInt("Pick number to make changes\n\e[32m9 - Save\n\e[31m0 - Cancel\e[39;49m");

		if (InputInt == 1) vessel.name = Vessel_Name(0);
		else if (InputInt == 2) vessel.gt = Vessel_GT();
		else if (InputInt == 3)vessel.dwt = Vessel_DWT();
		else if (InputInt == 4) vessel.length = Vessel_Length();
		else if (InputInt == 5) vessel.breadth = Vessel_Breadth();
		else if (InputInt == 6) vessel.year = Vessel_Year();
	} while (InputInt != 9 && InputInt != 0);

	Dependency::ClearScreen(Header);
	Dependency::EndLine();
	if (InputInt == 9)
	{
		bool inserted = (VesselID == 0) ? db->Insert(vessel) : db->Update(vessel);
		if (!inserted)
		{
			cout << "DATABASE ERROR\n";
			Dependency::SleepCommand(1000);
		}
		else
		{
			if (VesselID != 0)
			{
				cout << "VESSEL UPDATE SUCESSFULLY\n";
				Dependency::SleepCommand(1000);
			}
			else
			{
				cout << "VESSEL ADDED SUCCESSFULLY\n";
				Dependency::SleepCommand(1000);
				SelectedVesselID = db->db.LastInsertedID();
				Detail();
				InputInt = 1;
			}
		}
	}
}

string VesselManagement::Vessel_Name(int VesselID)
{
	bool check;
	string name;
	do
	{
		check = true;
		name = input::getInput("Vessel Name");

		if ((from(db->vessel)
			>> where([&](Vessel const& a)
			{
			  bool b = boost::to_lower_copy(a.name) == boost::to_lower_copy(name);
			  if (VesselID != 0) return (a.ID != VesselID) && b;
			  else return b;
			})
			>> count()) != 0)
		{
			cout << "Vessel name already exists. Please enter different name\n";
			check = false;
		}
	} while (!check);

	return name;
}

double VesselManagement::Vessel_GT()
{
	return input::InputDouble("Gross Tonne (GT)");
}

double VesselManagement::Vessel_DWT()
{
	return input::InputDouble("Deadweight tonnage (DWT)");
}

double VesselManagement::Vessel_Length()
{
	return input::InputDouble("Length");
}

double VesselManagement::Vessel_Breadth()
{
	return input::InputDouble("Breadth");
}

int VesselManagement::Vessel_Year()
{
	return input::InputInt("Vessel Year");
}




