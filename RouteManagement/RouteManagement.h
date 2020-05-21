//
// Created by Adi Iman on 18/04/2020.
//

#ifndef CTMS_NEW_ROUTEMANAGEMENT_H
#define CTMS_NEW_ROUTEMANAGEMENT_H

#include "../Core.h"

class RouteManagement
{
public:
	RouteManagement(CtmsDB*);
	void Menu(), Add(), List(), Delete();
private:
	CtmsDB* db;
	int InputInt, SelectedIndex;
};

#endif //CTMS_NEW_ROUTEMANAGEMENT_H
