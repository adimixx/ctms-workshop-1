//
// Created by Adi Iman on 10/05/2020.
//

#include "TicketManagement.h"

TicketManagement::TicketManagement(CtmsDB* db)
{
	this->db = db;
}

void TicketManagement::TicketCounter()
{
	packages = from(db->package) >> where([&](Package const& a)
	{
	  return input::compareDate("", input::ConvertDate(a.start_date, db->db), db->db, false);
	}) >> to_vector();

	if (packages.empty())
	{
		cout << "No packages available on sale\n";
		Dependency::SleepCommand(1000);
	}
	else
	{
		do
		{
			Dependency::ClearScreen("TICKET COUNTER");

			cout << "Tickets in cart : " << tickets.size() << " tickets\n";

			for (int i = 0; i < tickets.size(); ++i)
			{
				cout << "Ticket Set " << i + 1 << endl;
				Output_Ticket(tickets.at(i));
				cout<<endl;
			}

			cout << "1. Add Ticket Set\n";
			cout << "2. Remove Ticket Set\n";
			cout << "3. Edit Ticket Set\n";
			cout << "4. Confirm Ticket Payment\n";
			cout << "5. Reset cart\n\n";

			cout << "0. Main Menu\n\n";

			InputInt = input::InputInt("Enter your input");

			if (InputInt == 1)
			{
				boost::optional<Package> selectedPackage = Package_SelectPackage();
				if (selectedPackage.has_value())
				{
					Deck selectedDeck = Package_SelectDeck(selectedPackage.value());
					Package_AddTickets(selectedDeck, selectedPackage.value(), -1);
				}
			}
			else if (InputInt == 2)
			{
				InputInt = input::InputInt("Select Ticket Set Number, 0 to cancel", 0, tickets.size());
				if (InputInt != 0)
				{
					cout << "Ticket Set " << InputInt << endl;
					Output_Ticket(tickets.at(InputInt - 1));

					InputInt = input::InputInt("Are you sure you want to Delete this ticket set? 1- Yes, 0- No", 0, 1);

					if (InputInt == 1)
					{
						tickets.erase(tickets.begin() + (InputInt - 1));
					}
					InputInt = 1;
				}
			}
			else if (InputInt == 3)
			{
				InputInt = input::InputInt("Select Ticket Set Number, 0 to cancel", 0, tickets.size());
				if (InputInt != 0)
				{
					Deck selectedDeck = from(db->deck) >> first_or_default([&](Deck const& a)
					{ return a.Id == tickets.at(InputInt - 1).at(0).deckID; });
					Package selectedPackage = from(db->package) >> first_or_default([&](Package const& a)
					{ return a.ID == tickets.at(InputInt - 1).at(0).packageID; });
					Package_AddTickets(selectedDeck, selectedPackage, InputInt - 1);
				}
			}
			else if (InputInt == 4)
			{
				TicketCheckIn();
			}
			else if (InputInt == 4)
			{
				if (tickets.size() != 0)
				{
					InputInt = input::InputInt("Are you sure you want to Reset cart? 1- Yes, 0- No", 0, 1);

					if (InputInt == 1)
					{
						tickets.clear();
					}
					InputInt = 1;
				}
			}
		} while (InputInt != 0);
	}
}

void TicketManagement::TicketCheckIn()
{
}

void TicketManagement::Output_Ticket(vector<Ticket> ticket){
	TextTable t('-', '|', '+');
	t.add("Num");
	t.add("Name");
	t.add("Phone");
	t.add("Email");
	t.add("Price & Ticket Type");
	t.endOfRow();

	for (int j = 0; j < ticket.size(); ++j)
	{
		auto tick = ticket.at(j);
		auto ticketType = from(db->ticket_type)
			>> first_or_default([&](Ticket_Type const& x)
			{ return x.ID == tick.ticketTypeID; });

		t.add(to_string(j + 1));
		t.add(tick.name);
		t.add(tick.phone);
		t.add(tick.email);
		t.add("RM " + to_string(tick.price) + "(" + ticketType.name + ")");
		t.endOfRow();
	}
	t.setAlignment(4, TextTable::Alignment::RIGHT);
	cout<<t;
}
