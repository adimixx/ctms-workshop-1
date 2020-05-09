//
// Created by Adi Iman on 18/04/2020.
//
#include "RouteManagement.h"

void RouteManagement::List()
{
    if (db->route.size() == 0){
        cout<<"No Routes available in database. Please add a new one"<<endl;
        Dependency::SleepCommand(1000);
    }

    else{
        try {
            Dependency::ClearScreen("Route Management : Route List");

            for (int i = 0; i < db->route.size(); ++i) {
                cout << i+1 << " : " + db->route.at(i).name + "\n";
            }

            InputInt = input::InputInt("\n\nNumber to select Route to delete, 0 - Back", 0);

            if (InputInt == 0)
            {
                return;
            }
            else if (InputInt <= db->route.size() && InputInt > 0){
                SelectedIndex = InputInt-1;
                Delete();
            }
            List();
        }
        catch (std::exception e) {
            List();
        }
    }
}


