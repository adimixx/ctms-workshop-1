//
// Created by Adi Iman on 08/06/2020.
//

#include "TicketManagement.h"

void TicketManagement::TicketCheckIn()
{
	/*** Can check in at least 4 hours before departure and at most 1 hour before departure ***/
	auto canCheckIn = from(db->package) >> where([&](Package const& a)
	{
	  int minutes = input::compareMinute(input::getDateTimeNow(db->db), input::ConvertDateAndTime(a.start_date,db->db), db->db);
	  return (minutes > 60 && minutes < 210);
	}) >> to_vector();

	if (canCheckIn.empty())
	{
		cout << "No packages available to check in" << endl << "Tickets can be checked in at least 4 hours before departure and at most 1 hour before departure" << endl;
		Dependency::SleepCommand(4000);
	}

	else
	{
		TextTable t('-', '|', '+');
		t.add("Num");
		t.add("Package Name");
		t.add("Departure Time");
		t.endOfRow();

		for (int i = 0; i < canCheckIn.size(); i++)
		{
			t.add(to_string(i + 1));
			t.add(canCheckIn.at(i).Name);
			t.add(canCheckIn.at(i).start_date);
			t.endOfRow();
		}

		Dependency::ClearScreen("TICKET COUNTER :: Check In Ticket");
		cout << t << endl;
		input::InputInt("Enter Package Number to check in, 0 to cancel", 0, canCheckIn.size());

		if (InputInt != 0)
		{
			checkIn_Package(canCheckIn.at(InputInt - 1));
		}
	}
}

void TicketManagement::checkIn_Package(Package pkg)
{
	auto currentUser = from(db->user) >> first_or_default([&](User const& a)
	{ return a.Id == db->loggedInUserID; });
	auto vessel = from(db->vessel) >> first_or_default([&](Vessel const& a)
	{ return a.ID == pkg.vesselID; });

	do
	{
		auto checkedIn = from(db->ticket) >> where([&](Ticket const& a)
		{ return a.packageID == pkg.ID && a.checkIn; }) >> to_vector();

		TextTable t('-', '|', '+');
		t.add("Num");
		t.add("Name");
		t.add("Deck Name");
		t.add("Deck Level");
		t.add("Check In Date");
		t.add("Staff in Charge");
		t.endOfRow();

		for (int i = 0; i < checkedIn.size(); i++)
		{
			auto tct = checkedIn.at(i);
			auto deck = from(db->deck) >> first_or_default([&](Deck const& a)
			{ return a.Id == tct.deckID; });
			auto staff = from(db->user) >> first_or_default([&](User const& a)
			{ return a.Id == tct.checkInUserID.value(); });

			t.add(to_string(i + 1));
			t.add(tct.name);
			t.add(deck.Name);
			t.add(to_string(deck.Level));
			t.add(tct.checkInDate.value());
			t.add(staff.Username);
			t.endOfRow();
		}

		Dependency::ClearScreen("TICKET COUNTER :: Check In Ticket");
		cout << "Staff in charge : " << currentUser.Username << endl;
		cout << "Package : " << pkg.Name << endl;
		cout << "Departure Date : " << pkg.start_date << endl;
		cout << "Vessel : " << vessel.name << endl << endl;

		cout << "Checked In Tickets : " << vessel.name << endl;
		cout << t << endl;

		InputInt = input::InputInt("1 - Check in user, 0 - Exit Shift", 0, 1);

		if (InputInt == 1)
		{
			checkIn_Ticket(currentUser, pkg, vessel);
			InputInt = 1;
		}
	} while (InputInt != 0);
}

void TicketManagement::checkIn_Ticket(User currentUser, Package pkg, Vessel vessel)
{
	vector<Ticket> checkInTicket, notCheckInTicket, findTicket, currentPkg;
	currentPkg = from(db->ticket) >> where([&](Ticket const& a)
	{ return a.packageID == pkg.ID; }) >> to_vector();

	Output_CheckInHeader(currentUser, pkg, vessel);
	cout << "Check In ticket" << endl << endl;

	InputString = input::getInput("Please enter ticket email address/phone number");

	findTicket = from(currentPkg) >> where([&](Ticket const& a)
	{ return boost::to_lower_copy(a.email) == boost::to_lower_copy(InputString) || a.phone == InputString; }) >> to_vector();
	notCheckInTicket = findTicket;

	if (findTicket.empty())
	{
		cout << "No ticket matches the credential" << endl;
		Dependency::SleepCommand(1000);
	}
	else
	{
		Output_CheckInHeader(currentUser, pkg, vessel);
		cout << "Ticket list : " << endl << endl;
		Output_Ticket(findTicket);
		InputInt = input::InputInt("1 - Check in all ticket,2- Check in Specific Ticket, 0 - Cancel", 0, 2);

		if (InputInt == 1)
		{
			checkInTicket = findTicket;
		}

		else if (InputInt == 2)
		{
			do
			{
				Output_CheckInHeader(currentUser, pkg, vessel);

				cout << "Ticket list : " << endl;
				Output_Ticket(notCheckInTicket);

				cout << "Tickets to check in : " << endl;
				Output_Ticket(checkInTicket);

				InputInt = input::InputInt("Enter Ticket number to check in,0 - Done make changes", 0, notCheckInTicket.size());
				if (InputInt != 0)
				{
					checkInTicket.emplace_back(notCheckInTicket.at(InputInt - 1));
					notCheckInTicket.erase(notCheckInTicket.begin() + (InputInt - 1));
				}
			} while (InputInt != 0);
			InputInt = 2;
		}

		if (InputInt == 1 || InputInt == 2)
		{
			Output_CheckInHeader(currentUser, pkg, vessel);
			cout << "Tickets to check in : " << endl;
			Output_Ticket(checkInTicket);
			InputInt = input::InputInt("1 - Confirm Ticket check in, 0 - Cancel", 0, 1);

			if (InputInt == 1)
			{
				for (int i = 0; i < checkInTicket.size(); i++)
				{
					checkInTicket.at(i).checkIn = true;
					checkInTicket.at(i).checkInUserID = db->loggedInUserID;
				}

				if (!db->Update(checkInTicket))
				{
					cout << "DATABASE ERROR!" << endl;
				}
				else
				{
					cout << "CHECK IN SUCCESS" << endl;
				}
				Dependency::SleepCommand(1000);
			}
		}
	}
}

void TicketManagement::Output_CheckInHeader(User currentUser, Package pkg, Vessel vessel)
{
	Dependency::ClearScreen("TICKET COUNTER :: Check In Ticket");
	cout << "Staff in charge : " << currentUser.Username << endl;
	cout << "Package : " << pkg.Name << endl;
	cout << "Departure Date : " << pkg.start_date << endl;
	cout << "Vessel : " << vessel.name << endl << endl;
}