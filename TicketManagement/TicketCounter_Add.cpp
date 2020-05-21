//
// Created by Adi Iman on 10/05/2020.
//

#include "TicketManagement.h"

boost::optional<Package> TicketManagement::Package_SelectPackage()
{
	Dependency::ClearScreen("TICKET COUNTER :: SELECT PACKAGE");
	for (int j = 0; j < packages.size(); j++)
	{
		cout << j + 1 << " : " << packages[j].Name << endl;
		cout << "Date : " << packages[j].start_date << endl;
		cout << endl;
	}

	InputInt = input::InputInt("Enter your input, 0 to cancel (Back to Menu)", 0, packages.size());

	if (InputInt > 0)
	{
		return packages.at(InputInt - 1);
	}

	return boost::none;
}

Deck TicketManagement::Package_SelectDeck(Package selectedPackage)
{
	auto vesselDeck = from(db->deck) >> where([&](Deck const& x)
	{
	  return x.VesselID == selectedPackage.vesselID &&
		  (x.MaxPassenger - (from(db->ticket) >> count([&](Ticket const& a)
		  { return a.deckID == x.Id; }))) > 0;
	}) >> orderby_ascending([&](Deck const& x)
	{ return x.Level; }) >> to_vector();
	Deck selectedDeck;

	Dependency::ClearScreen("TICKET COUNTER :: SELECT PACKAGE");
	Package_PackageDetails(selectedPackage, true, false, 0);

	cout << "Available Decks : " << endl << endl;

	cout << "Deck Level & Name" << setw(30) << "Available Seats" << setw(20) << "Price (Per Ticket)" << endl;

	for (int j = 0; j < vesselDeck.size(); ++j)
	{
		auto price = selectedPackage.price;
		auto seatsLeft = vesselDeck.at(j).MaxPassenger - (from(db->ticket) >> count([&](Ticket const& a)
		{ return a.deckID == vesselDeck.at(j).Id; }));
		if (vesselDeck.at(j).PremiumID != 0)
		{
			if (vesselDeck.at(j).PremiumID == 1)
			{
				price += vesselDeck.at(j).PremiumValue;
			}
			else
			{
				price *= vesselDeck.at(j).PremiumValue;
			}
		}
		cout << to_string(vesselDeck.at(j).Level) << " : " << vesselDeck.at(j).Name << setw(30) << seatsLeft << setw(20) << "RM " << price << endl;
	}
	Dependency::EndLine();
	Dependency::EndLine();

	check = false;
	do
	{
		InputInt = input::InputInt("Select Deck Level", vesselDeck.front().Level, vesselDeck.back().Level);
		selectedDeck = from(vesselDeck) >> first_or_default([&](Deck const& x)
		{ return x.Level == InputInt; });
		if (selectedDeck.Id == 0)
		{
			cout << "level not available! Please enter a valid input" << endl;
		}
		else
		{
			check = true;
		}
	} while (!check);
	return selectedDeck;
}

void TicketManagement::Package_AddTickets(Deck selectedDeck, Package selectedPackage, int ticketIndex)
{
	vector<Ticket> ticketList;
	if (ticketIndex >= 0)
	{
		ticketList = from(tickets.at(ticketIndex)) >> where([&](Ticket const& a)
		{ return a.deckID == selectedDeck.Id && a.packageID == selectedPackage.ID; }) >> to_vector();
	}
	do
	{
		Dependency::ClearScreen("TICKET COUNTER :: TICKET DETAILS");

		Package_PackageDetails(selectedPackage, false, true, selectedDeck.Id);

		cout << "Total Tickets : " << ticketList.size() << endl << "Ticket List : " << endl;
		Output_Ticket(ticketList);
		Dependency::EndLine();

		InputInt = input::InputInt(
			"1 - Add Passenger, 2 - Delete Passenger, 3 - Change Deck, 4 - Change Package, 5 - Confirm Order, 0 - Cancel");
		if (InputInt == 1)
		{
			Ticket newTicket;
			newTicket.deckID = selectedDeck.Id;
			newTicket.packageID = selectedPackage.ID;
			newTicket.name = input::getInput("Passenger Name");
			newTicket.email = input::getInput("Passenger Email");
			newTicket.phone = input::getInput("Passenger Phone Number");
			ticketList.emplace_back(newTicket);
		}
		else if (InputInt == 2)
		{
			int selectedIndex =
				input::InputInt("Enter Passenger Number to delete, 0 to cancel", 0, ticketList.size()) - 1;

			if (selectedIndex != -1)
			{
				cout << "Passenger Name : " << ticketList.at(selectedIndex).name << endl;
				InputInt = input::InputInt("Confirm to delete this passenger? 1 - yes, 0 - no", 0, 1);
				if (InputInt == 1)
				{
					ticketList.erase(ticketList.begin() + (selectedIndex - 1));
				}
				InputInt = 1;
			}
		}
		else if (InputInt == 3)
		{
			selectedDeck = Package_SelectDeck(selectedPackage);
			ticketList = from(ticketList) >> select([&](Ticket const& a)
			{
			  a.deckID << selectedDeck.Id;
			  return a;
			}) >> to_vector();
		}

		else if (InputInt == 4)
		{
			boost::optional<Package> selectedPackage2 = Package_SelectPackage();
			if (selectedPackage2.has_value())
			{
				selectedPackage = selectedPackage2.value();
				selectedDeck = Package_SelectDeck(selectedPackage);
				ticketList = from(ticketList) >> select([&](Ticket const& a)
				{
				  a.deckID << selectedDeck.Id;
				  a.packageID << selectedPackage.ID;
				  return a;
				}) >> to_vector();
			}
		}

		if (InputInt == 5 && ticketList.size() <= 0)
		{
			cout << "Ticket list cannot be empty. Please insert a ticket!" << endl;
			InputInt = 1;
		}
	} while (InputInt != 0 && InputInt != 5);

	if (InputInt == 5)
	{
		if (ticketIndex >= 0)
		{
			tickets.at(ticketIndex) = ticketList;
		}
		else
		{
			tickets.emplace_back(ticketList);
		}
	}
}

void TicketManagement::Package_PackageDetails(Package selectedPackage, bool showRoute, bool showDeck, long deckID)
{
	auto vessel =
		from(db->vessel) >> first_or_default([&](Vessel const& a)
		{ return a.ID == selectedPackage.vesselID; });

	cout << "Package Name : \t" + selectedPackage.Name + "\n";
	cout << "Start Date : \t" + selectedPackage.start_date + "\n";
	cout << "End Date:  \t" + selectedPackage.end_date + "\n\n";
	cout << "Vessel Name : \t" + vessel.name + "\n";
	if (showDeck)
	{
		auto selectedDeck = from(db->deck) >> first_or_default([&](Deck const& x)
		{ return x.Id == deckID; });
		cout << "Selected Deck : \t" + selectedDeck.Name + " (Level " << selectedDeck.Level << ")\n";
	}

	if (showRoute)
	{
		auto route = from(db->package_route)
			>> where([&](Package_Route const& a)
			{ return a.PackageID == selectedPackage.ID; })
			>> orderby_ascending([&](Package_Route const& a)
			{ return a.ID; })
			>> to_vector();
		cout << "Package Routes : " << endl;
		for (int j = 0; j < route.size(); ++j)
		{
			cout << j + 1 << " : "
				 << (from(db->route)
					 >> first_or_default([&](Route const& x)
					 { return x.ID == route.at(j).RouteID; })).name
				 << endl;
		}
	}
}




