//
// Created by Adi Iman on 18/04/2020.
//

#include "VesselManagement.h"

void VesselManagement::List() {
    do {
        Dependency::ClearScreen("Vessel Management : Vessel List");

        for (int j = 0; j < db->vessel.size(); ++j) {
            Dependency::EndLine();
            auto ves = db->vessel.at(j);
            string active = (ves.isActive) ? "Active" : "Not Active";
            cout << j + 1 << " : " + ves.name + "\nRegister Date : " + ves.registerDate + "\nStatus : " + active;
            Dependency::EndLine();
        }

        InputInt = input::InputInt("\n\nNumber to select Vessel, 0 - Back",0, db->vessel.size());

        if (InputInt != 0) {
            SelectedVesselID = db->vessel.at(InputInt-1).ID;
            Detail();
        }
    } while (InputInt != 0);
    InputInt = 1;
}

void VesselManagement::Detail() {
    do {
        int totalDecks;
        auto ves =  from(db->vessel) >> first_or_default([&] (Vessel const &v){return v.ID == SelectedVesselID;});

        string active = (ves.isActive) ? "Active" : "Not Active";
        string active2 = (ves.isActive) ? "Deactivate" : "Activate";

        Dependency::ClearScreen("Vessel Management : Vessel Detail");

        Vessel_VesselDetails(ves);
        cout << "Register Date : \t" + ves.registerDate + " \n";
        cout << "Status : \t" + active + " \n";

        cout << "Deck Details : \n";
        totalDecks = Vessel_VesselDecksDetails(ves, true);

        Dependency::EndLine();
        Dependency::EndLine();
        InputInt = input::InputInt("1- Edit, 2- Delete, 3-" + active2 + " Vessel\n4- Add New Deck, 5- Delete Deck\n0-Back");

        if (InputInt == 1) {
            AddEdit(ves.ID);
        } else if (InputInt == 2) {
            Vessel_Delete();
        } else if (InputInt == 3) {
            Activation(ves);
        } else if (InputInt == 4) {
            Deck_AddEdit(0);
        } else if (totalDecks != 0 && InputInt == 5) {
            Deck_Delete();
        }
    } while (InputInt != 0);
    InputInt = 1;
}

void VesselManagement::Activation(Vessel ves) {
    string active = (!ves.isActive) ? "Activate" : "Deactivate";
    do {
        InputInt = input::InputInt("Are you sure you want to " + active + " this vessel?\n1-Yes, 0-No");
        if (InputInt == 1) {
            ves.isActive = !ves.isActive;

            if (!db->Update(ves)) {
                cout << "DATABASE ERROR\n";
            } else {
                cout << "\nVessel " + active+"d" + " Successfully\n";
            }
            Dependency::SleepCommand(1000);
        }
    } while (InputInt != 1 && InputInt != 0);
    InputInt = 0;
}
