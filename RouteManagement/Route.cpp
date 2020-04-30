//
// Created by Adi Iman on 18/04/2020.
//

#include "RouteManagement.h"

RouteManagement::RouteManagement(CtmsDB* db) {
    this->db = db;
}

void RouteManagement::Menu()
{
    Dependency::ClearScreen("Route Management");

    cout << "1. Add New Route\n";
    cout << "2. List Route\n\n";

    cout << "0. Main Menu\n\n";

    do {
        InputInt = input::InputInt("Enter your input");
    } while (InputInt != 0 && InputInt != 1 && InputInt != 2);

    if (InputInt == 1) {
        Add();
    }

    else if (InputInt == 2) {
        List();
    }

    else if (InputInt == 0) {
        return;
    }

    Menu();
}

