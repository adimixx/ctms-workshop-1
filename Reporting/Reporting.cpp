//
// Created by Adi Iman on 05/06/2020.
//

#include "Reporting.h"
Reporting::Reporting(CtmsDB* db)
{
	this->db = db;
	List();
}
void Reporting::List()
{
	Dependency::ClearScreen("System Report");

	cout<<"Revenue Report"<<endl;

	TextTable t('-', '|', '+');

	/***Select previous 30 day Ticket***/
	auto rep30 = from(db->ticket) >> where([&](Ticket const& a)
	{
		return input::compareDate(input::AddDay("",-30,db->db), a.dateIssued, db->db, false);
	});

	cout<<"Previous 30 days : "<<endl;
	t.add("Total Earned");
	t.add("Ticket Quantities");
	t.add("Highest selling Package");
	t.add("Highest performing staff");
	t.endOfRow();

	t.add("RM " + input::toString(rep30 >> sum([&](Ticket const& a){return a.price;}),2) );
	t.add(to_string(rep30>>count()));
//	t.add(rep30 >> )



	cout << t <<endl;

}
