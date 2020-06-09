//
// Created by Adi Iman on 18/04/2020.
//
#include "VesselManagement.h"

void VesselManagement::Vessel_Delete()
{
	auto packages = from(db->package) >> where([&](Package const& a)
	{ return a.vesselID == SelectedVesselID; }) >> count();

	if (packages != 0)
	{
		cout << "This Vessel cannot be deleted as it has been used on other packages. You can opt to disable this vessel" << endl;
	}

	else{
		InputInt = input::InputInt("Are you sure you want to Delete this vessel?\n1-Yes, 0-No", 0, 1);

		if (InputInt == 1)
		{
			auto vessel = from(db->vessel) >> first_or_default([&](Vessel const& v)
			{ return v.ID == SelectedVesselID; });
			if (!db->Delete(vessel))
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
