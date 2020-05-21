//
// Created by Adi Iman on 18/04/2020.
//
#include "VesselManagement.h"

void VesselManagement::Vessel_Delete()
{
	do
	{
		InputInt = input::InputInt("Are you sure you want to Delete this vessel?\n1-Yes, 0-No");
	} while (InputInt != 1 && InputInt != 0);

	if (InputInt == 1)
	{
		auto vessel = from(db->vessel) >> first_or_default([&](Vessel const& v)
		{ return v.ID == SelectedVesselID; });
		if (!db->Delete(vessel))
		{
			cout << "DATABASE ERROR\n";
		}
		else
		{
			cout << "\nVessel DELETED SUCCESSFULLY\n";
		}
		Dependency::SleepCommand(1000);
	}
	InputInt = 0;
}
