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

	cout << "Revenue Report" << endl;

	TextTable t('-', '|', '+');
	TextTable u('-', '|', '+');
	/***Select previous 30 day Ticket***/
	auto rep30 = from(db->ticket) >> where([&](Ticket const& a)
	{
	  return input::compareDate(input::AddDay("", -30, db->db), input::ConvertDateAndTime(a.dateIssued, db->db), db->db, false);
	});
	/***Select all Ticket***/
	auto repWhole = from(db->ticket);

	t.add("Total Earned");
	t.add("Ticket Quantities");
	t.add("Highest selling Package");
	t.add("Highest performing staff");
	t.endOfRow();

	u.add("Total Earned");
	u.add("Ticket Quantities");
	u.add("Highest selling Package");
	u.add("Highest performing staff");
	u.endOfRow();

	t.add("RM " + input::toString(rep30 >> sum([&](Ticket const& a)
	{ return a.price; }), 2));
	t.add(to_string(rep30 >> count()));
	t.add(getHighestSelling(-30));
	t.add(getHighestStaff(-30));
	t.endOfRow();

	u.add("RM " + input::toString(repWhole >> sum([&](Ticket const& a)
	{ return a.price; }), 2));
	u.add(to_string(repWhole >> count()));
	u.add(getHighestSelling(0));
	u.add(getHighestStaff(0));
	u.endOfRow();

	cout << "Previous 30 days : " << endl
		 << t << endl << endl
		 << "All Tickets : " << endl
		 << u << endl;

	input::getInput("Press any key to go back");
}

string Reporting::getHighestSelling(int days)
{
	db->db.statement = "SELECT P.Name , count(*) as total from TICKET t join PACKAGE P on t.PackageID = P.ID ";
	if (days != 0) db->db.statement += "where datediff(t.DateIssued,date_add(now(),interval " + to_string(days) + " day)) >= 0 ";
	db->db.statement += "group by PackageID order by total desc limit 1";
	db->db.select();
	if (db->db.RowCount() == 0) return "-";
	else
	{
		db->db.row = db->db.FetchRow();
		return string(db->db.row[0]) + " (" + string(db->db.row[1]) + " tickets)";
	}
}

string Reporting::getHighestStaff(int days)
{
	db->db.statement = "SELECT U.Username , count(*) as total from TICKET t join RECEIPT r on t.ReceiptID = r.ID join USER U on r.PurchaseUserID = U.ID ";
	if (days != 0) db->db.statement += "where datediff(t.DateIssued,date_add(now(),interval " + to_string(days) + " day)) >= 0 ";
	db->db.statement += "group by U.Username order by total desc limit 1";
	db->db.select();
	if (db->db.RowCount() == 0) return "-";
	else
	{
		db->db.row = db->db.FetchRow();
		return string(db->db.row[0]) + " (" + string(db->db.row[1]) + " tickets)";
	}
}

