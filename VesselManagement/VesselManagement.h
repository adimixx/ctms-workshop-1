//
// Created by Adi Iman on 18/04/2020.
//

#ifndef CTMS_NEW_VESSELMANAGEMENT_H
#define CTMS_NEW_VESSELMANAGEMENT_H

#include "../Core.h"

class VesselManagement
{
public:
	VesselManagement(CtmsDB* db);
	void Menu(),
		AddEdit(int VesselID), List(), Detail(), Activation(Vessel), Vessel_Delete(),
		Deck_AddEdit(int DeckID), Deck_Delete();
private:
	string Vessel_Name(int VesselID);
	double Vessel_GT(), Vessel_DWT(), Vessel_Length(), Vessel_Breadth();
	int Vessel_Year();
	string Deck_Name(int DeckID);
	int Deck_level(int DeckID);
	void Deck_Premium(long& PremiumID, double& PremiumValue);
	void Deck_Details(Deck b, bool);
	void Vessel_VesselDetails(Vessel vessel);
	int Vessel_VesselDecksDetails(Vessel vessel, bool showDet);
	int SelectedVesselID;
	int InputInt;
	CtmsDB* db;
};

#endif //CTMS_NEW_VESSELMANAGEMENT_H
