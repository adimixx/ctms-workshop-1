//
// Created by Adi Iman on 10/05/2020.
//

#ifndef WORKSHOP1_TICKETMANAGEMENT_H
#define WORKSHOP1_TICKETMANAGEMENT_H

#include "../Core.h"

class TicketManagement
{
public:
	TicketManagement(CtmsDB* db);
	void TicketCounter();

	void TicketCheckIn();
private:
	CtmsDB* db;
	vector<vector<Ticket>> tickets;
	vector<Package> packages;
	bool check;

	void Output_Ticket(vector<Ticket> ticket);

	boost::optional<Package> Package_SelectPackage();
	Deck Package_SelectDeck(Package selectedPackage);
	void Package_AddTickets(Deck, Package, int),
		Package_PackageDetails(Package selectedPackage, bool showRoute, bool showDeck, long deckID);


	int InputInt;
	string InputString;
};

#endif //WORKSHOP1_TICKETMANAGEMENT_H
