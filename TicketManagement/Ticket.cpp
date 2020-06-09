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
	/***Packages can only be on sale until 1 day before ship sails***/
	packages = from(db->package) >> where([&](Package const& a)
	{
	  return input::compareDate("", input::ConvertDateAndTime(a.start_date, db->db), db->db, false);
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

			int totalTicket = 0;
			for (int i = 0; i < tickets.size(); ++i)
			{
				cout << "Ticket Set " << i + 1 << endl;
				Output_Ticket(tickets.at(i));
				totalTicket += tickets.at(i).size();
			}
			cout << "Tickets in cart : " << totalTicket << " tickets\n";

			cout << "1. Add Ticket Set\n";
			cout << "2. Remove Ticket Set\n";
			cout << "3. Edit Ticket Set\n";
			cout << "4. Checkout\n";
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
				if (Ticket_CheckOut())
				{
					tickets.clear();
				}
			}
			else if (InputInt == 5)
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

bool TicketManagement::Ticket_CheckOut()
{
	Dependency::ClearScreen("TICKET COUNTER :: Checkout");

	cout << "Tickets in cart : " << tickets.size() << " tickets\n";

	double totalPrice = 0;
	for (int i = 0; i < tickets.size(); ++i)
	{
		cout << "Ticket Set " << i + 1 << endl;
		Output_Ticket(tickets.at(i));
		totalPrice += from(tickets.at(i)) >> sum([&](Ticket const& a)
		{ return a.price; });
		cout << endl;
	}

	cout << "Total Price : \tRM " << input::toString(totalPrice, 2) << endl;

	InputInt = input::InputInt("1- Confirm Checkout, 0- Cancel Checkout", 0, 1);

	if (InputInt == 1)
	{
		cout << "Total Price : \tRM " << input::toString(totalPrice, 2) << endl;
		double payValue = input::InputDouble("Please enter cash paid", totalPrice);

		Receipt receipt;
		receipt.date = input::getDateTimeNow(db->db);
		receipt.total = totalPrice;
		receipt.cashIn = payValue;
		receipt.cashOut = payValue - totalPrice;
		receipt.purchaseUserID = db->loggedInUserID;
		vector<Ticket> ticketsInsert;

		for (int i = 0; i < tickets.size(); i++)
		{
			from(tickets.at(i)) >> for_each([&](Ticket const& a)
			{ ticketsInsert.emplace_back(a); });
		}

		if (db->Insert(receipt, ticketsInsert))
		{
			Dependency::EndLine();
			cout << "Total Paid : \tRM " << input::toString(receipt.cashIn, 2) << endl;
			cout << "Balance : \tRM " << input::toString(receipt.cashOut, 2) << endl;
			Dependency::SleepCommand(2000);
			return true;
		}

		else
		{
			cout << "DATABASE ERROR" << endl;
		}
	}
	return false;
}

void TicketManagement::Output_Ticket(vector<Ticket> ticket)
{
	TextTable t('-', '|', '+');
	t.add("Num");
	t.add("Name");
	t.add("Phone");
	t.add("Email");
	t.add("Deck Name");
	t.add("Deck Level");
	t.add("Price & Ticket Type");
	t.endOfRow();

	for (int j = 0; j < ticket.size(); ++j)
	{
		auto tick = ticket.at(j);
		auto ticketType = from(db->ticket_type)
			>> first_or_default([&](Ticket_Type const& x)
			{ return x.ID == tick.ticketTypeID; });
		auto deck = from(db->deck) >> first_or_default([&](Deck const& a)
		{ return a.Id == tick.deckID; });

		t.add(to_string(j + 1));
		t.add(tick.name);
		t.add(tick.phone);
		t.add(tick.email);
		t.add(deck.Name);
		t.add(to_string(deck.Level));
		t.add("RM " + input::toString(tick.price, 2) + "(" + ticketType.name + ")");
		t.endOfRow();
	}
	t.setAlignment(4, TextTable::Alignment::RIGHT);
	cout << t << endl;
}
