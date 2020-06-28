//
// Created by Adi Iman on 18/04/2020.
//

#ifndef CTMS_NEW_PACKAGEMANAGEMENT_H
#define CTMS_NEW_PACKAGEMANAGEMENT_H

#include "../Core.h"

class PackageManagement
{
public:
	PackageManagement(CtmsDB* db);
	void Menu(), AddEdit(int PackageID), List(), Detail(), Delete();
private:
	CtmsDB* db;

	int SelectedPackageID;
	int InputInt;
	string InputString;

	void detail_package(Package pack, bool showInd);
	void detail_route(vector<Package_Route> route, bool showInd);
	void detail_vessel(Vessel vessel, bool showInd);
	void detail_ticketList(Package pack);
	string Package_Name(int packID);
	void Package_Date(string& StartDate, string& EndDate, int packageID, long& VesselID);
	long Package_Vessel(string StartDate, string EndDate, int packageID);
	vector<Package_Route> Package_Routes(int packageID);
};

#endif //CTMS_NEW_PACKAGEMANAGEMENT_H
