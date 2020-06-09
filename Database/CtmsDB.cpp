//
// Created by Adi Iman on 25/04/2020.
//

#include "CtmsDB.h"

CtmsDB::CtmsDB(Database db)
{
	this->db = std::move(db);
	initData();
}

void CtmsDB::initData()
{
	deck.clear();
	package.clear();
	package_route.clear();
	receipt.clear();
	role.clear();
	route.clear();
	ticket.clear();
	ticket_type.clear();
	user.clear();
	user_role.clear();
	value_type.clear();
	vessel.clear();

	db.statement = "select ID, Name, Level, MaxPassenger, PremiumID, PremiumValue, VesselID from DECK";
	db.select();
	while ((db.row = db.FetchRow()))
	{
		deck.emplace_back(Deck(
			stol(string(db.row[0])),
			string(db.row[1]),
			stoi(string(db.row[2])),
			stoi(string(db.row[3])),
			(db.row[4] != nullptr) ? stol(string(db.row[4])) : 0,
			(db.row[5] != nullptr) ? stod(string(db.row[5])) : 0,
			stol(string(db.row[6]))
		));
	}

	db.statement =
		"select ID, Name, DATE_FORMAT(START_DATE,'%d/%m/%Y %H%i') ,DATE_FORMAT(END_DATE,'%d/%m/%Y'), Price, VesselID from PACKAGE";
	db.select();
	while ((db.row = db.FetchRow()))
	{
		package.emplace_back(
			stol(string(db.row[0])),
			string(db.row[1]),
			string(db.row[2]),
			string(db.row[3]),
			stod(string(db.row[4])),
			stol(string(db.row[5]))
		);
	}

	db.statement = "select ID, RouteID, PackageID from PACKAGE_ROUTE";
	db.select();
	while ((db.row = db.FetchRow()))
	{
		package_route.emplace_back(Package_Route(
			stol(string(db.row[0])),
			stol(string(db.row[1])),
			stol(string(db.row[2]))
		));
	}

	db.statement = "select ID, DATE_FORMAT(Date,'%d/%m/%Y') , Total, CashIn, CashOut, PurchaseUserID from RECEIPT";
	db.select();
	while ((db.row = db.FetchRow()))
	{
		receipt.emplace_back(Receipt(
			stol(string(db.row[0])),
			string(db.row[1]),
			stod(string(db.row[2])),
			stod(string(db.row[3])),
			stod(string(db.row[4])),
			stol(string(db.row[5]))
		));
	}

	db.statement = "select ID, name from ROLE";
	db.select();
	while ((db.row = db.FetchRow()))
	{
		role.emplace_back(Role(
			stol(string(db.row[0])),
			string(db.row[1])
		));
	}

	db.statement = "select ID, name from ROUTE";
	db.select();
	while ((db.row = db.FetchRow()))
	{
		route.emplace_back(Route(
			stol(string(db.row[0])),
			string(db.row[1])
		));
	}

	db.statement =
		"select ID, ReceiptID,PackageID,DeckID,TicketTypeID,DATE_FORMAT(DateIssued,'%d/%m/%Y %H%i'),Name,Phone,Email,Price,CheckIn,CheckInUserID,DATE_FORMAT(CheckInDate,'%d/%m/%Y %H%i') from TICKET";
	db.select();
	while ((db.row = db.FetchRow()))
	{
		ticket.emplace_back(Ticket(
			stol(string(db.row[0])),
			stol(string(db.row[1])),
			stol(string(db.row[2])),
			stol(string(db.row[3])),
			stol(string(db.row[4])),
			string(db.row[5]),
			string(db.row[6]),
			string(db.row[7]),
			string(db.row[8]),
			stod(string(db.row[9])),
			(db.row[10] != nullptr),
			(db.row[10] == nullptr) ? 0 : stol(string(db.row[11])),
			(db.row[10] == nullptr) ? "" : string(db.row[12])
		));
	}

	db.statement = "select ID, Name, DiscountID, DiscountValue from TICKET_TYPE";
	db.select();
	while ((db.row = db.FetchRow()))
	{
		ticket_type.emplace_back(Ticket_Type(
			stol(string(db.row[0])),
			string(db.row[1]),
			(db.row[2] == nullptr) ? 0 : stol(string(db.row[2])),
			(db.row[3] == nullptr) ? 0 : stoi(string(db.row[3]))
		));
	}

	db.statement = "SELECT ID, USERNAME, PASSWORD, ISACTIVE, ADDEDBY FROM USER";
	db.select();
	while ((db.row = db.FetchRow()))
	{
		user.emplace_back(User(
			stol(string(db.row[0])),
			string(db.row[1]),
			string(db.row[2]),
			!(string(db.row[3]) == "0"),
			(db.row[4] == nullptr) ? 0 : stol(string(db.row[4]))
		));
	}

	db.statement = "select ID, UserID, RoleID from USER_ROLE";
	db.select();
	while ((db.row = db.FetchRow()))
	{
		user_role.emplace_back(User_Role(
			stol(string(db.row[0])),
			stol(string(db.row[1])),
			stol(string(db.row[2]))
		));
	}

	db.statement = "select ID, name from VALUE_TYPE";
	db.select();
	while ((db.row = db.FetchRow()))
	{
		value_type.emplace_back(Value_Type(
			stol(string(db.row[0])),
			string(db.row[1])
		));
	}

	db.statement =
		"select ID,Name,GT,DWT,Length,Breadth,Year,DATE_FORMAT(RegisterDate,'%d/%m/%Y'),isActive from VESSEL";
	db.select();
	while ((db.row = db.FetchRow()))
	{
		vessel.emplace_back(Vessel(
			stol(string(db.row[0])),
			string(db.row[1]),
			stod(string(db.row[2])),
			stod(string(db.row[3])),
			stod(string(db.row[4])),
			stod(string(db.row[5])),
			stoi(string(db.row[6])),
			string(db.row[7]),
			!(string(db.row[8]) == "0")
		));
	}
}

template<>
bool CtmsDB::Insert<User, User_Role>(User item, vector<User_Role> user_roles)
{
	string stat = "USER (USERNAME, PASSWORD, ISACTIVE, ADDEDBY) VALUES ('" + item.Username + "' , '" + item.Password + "',1,'" + to_string(item.AddedBy.value()) + "')";
	if (Operation(stat, 1))
	{
		stat = "USER_ROLE (UserID, RoleID) values ";
		for (int i = 0; i < user_roles.size(); i++)
		{
			if (i != 0) stat += ",";
			stat += "(" + to_string(db.LastInsertedID()) + ", " + to_string(user_roles.at(i).RoleID) + ")";
		}
		return Operation(stat, 1);
	}
	return false;
}

template<>
bool CtmsDB::Insert<Deck>(Deck item)
{
	string stat = "DECK (NAME, LEVEL, MAXPASSENGER, PREMIUMID, PREMIUMVALUE, VESSELID) VALUES ('" + item.Name + "' , " +
		to_string(item.Level) + "," + to_string(item.MaxPassenger) + ","
		+ ((item.PremiumID != 0) ? to_string(item.PremiumID) : "NULL") + "," +
		((item.PremiumID != 0) ? to_string(item.PremiumValue) : "NULL") + "," + to_string(item.VesselID) + ")";
	return Operation(stat, 1);
}

template<>
bool CtmsDB::Insert<Route>(Route item)
{
	string stat = "ROUTE (Name) values ('" + item.name + "')";
	return Operation(stat, 1);
}

template<>
bool CtmsDB::Insert<Package, Package_Route>(Package aPackage, vector<Package_Route> item2)
{
	string stat = "PACKAGE (Name, Start_Date, End_Date, Price, VesselID) values ('" + aPackage.Name + "', '" +
		aPackage.start_date + "', '" + aPackage.end_date + "', " + to_string(aPackage.price) + "," +
		to_string(aPackage.vesselID) + ")";
	if (Operation(stat, 1))
	{
		stat = "PACKAGE_ROUTE (routeid, packageid) values ";
		for (int i = 0; i < item2.size(); i++)
		{
			if (i != 0) stat += ",";
			stat += "(" + to_string(item2.at(i).RouteID) + ", " + to_string(db.LastInsertedID()) + ")";
		}
		return Operation(stat, 1);
	}
	return false;
}

template<>
bool CtmsDB::Insert<Vessel>(Vessel item)
{
	string stat = "VESSEL (NAME, GT, DWT, LENGTH, BREADTH, YEAR, REGISTERDATE, ISACTIVE) VALUES ('" +
		item.name + "' , " + to_string(item.gt) + "," + to_string(item.dwt) + "," + to_string(item.length) +
		"," +
		to_string(item.breadth) + "," + to_string(item.year) + ", NOW(), 1)";
	return Operation(stat, 1);
}

template<>
bool CtmsDB::Insert<Receipt, Ticket>(Receipt item, vector<Ticket> itemm)
{
	string stat = "RECEIPT (Date, Total, CashIn, CashOut, PurchaseUserID) VALUES ('" +
		item.date + "' , " + to_string(item.total) + "," + to_string(item.cashIn) + "," + to_string(item.cashOut) + "," + to_string(item.purchaseUserID) + ")";
	if (Operation(stat, 1))
	{
		string stat = "TICKET (ReceiptID, PackageID, DeckID, TicketTypeID, DateIssued, Name, Phone, Email, Price) VALUES ";

		for (auto item : itemm)
		{
			stat += "(" + to_string(db.LastInsertedID()) + ", " + to_string(item.packageID) + "," + to_string(item.deckID) + "," + to_string(item.ticketTypeID) + ", NOW() ,'"
				+ item.name + "', '" + item.phone + "' , '" + item.email + "' ," + to_string(item.price) + ") , ";
		}
		stat = stat.substr(0, (stat.length() - 3));
		return Operation(stat, 1);
	}
	return false;
}

template<>
bool CtmsDB::Update<Deck>(Deck item)
{
	string stat = "DECK SET ID = " + to_string(item.Id);
	return Operation(stat, 2);
}
/***Check in Ticket***/
template<>
bool CtmsDB::Update<Ticket>(vector<Ticket> item)
{
	string stat = "TICKET SET CheckIn = 1, CheckInUserID = " + to_string(item.at(0).checkInUserID.value()) + " , CheckInDate = NOW() WHERE ID IN (";
	for (int j = 0; j < item.size(); j++)
	{
		if (j != 0) stat += ",";
		stat += to_string(item.at(j).ID);
	}
	stat += ")";
	return Operation(stat, 2);
}

template<>
bool CtmsDB::Update<Vessel>(Vessel item)
{
	string stat = "VESSEL SET NAME='" + item.name +
		"', GT = " + to_string(item.gt) +
		", DWT = " + to_string(item.dwt) +
		", LENGTH = " + to_string(item.length) +
		", BREADTH = " +
		to_string(item.breadth) + ", YEAR = " +
		to_string(item.year) + " , ISACTIVE = ";
	stat += (item.isActive) ? "1" : "0";
	stat += " WHERE ID = " +
		to_string(item.ID);
	return Operation(stat, 2);
}

template<>
bool CtmsDB::Update<Package, Package_Route>(Package aPackage, vector<Package_Route> item2)
{
	string stat = "PACKAGE SET NAME='" + aPackage.Name +
		"', PRICE = " + to_string(aPackage.price) +
		", START_DATE = '" + aPackage.start_date +
		"', END_DATE = '" + aPackage.end_date +
		"', VESSELID = " + to_string(aPackage.vesselID) +
		" WHERE ID = " + to_string(aPackage.ID);
	if (Operation(stat, 2))
	{
		auto noUpdate = from(item2) >> where([&](Package_Route const& b)
		{ return b.ID != 0; }) >> to_vector();
		if (noUpdate.empty()){
			stat = "PACKAGE_ROUTE WHERE PACKAGEID = " + to_string(aPackage.ID);
		}

		else{
			stat = "";
			for (int i = 0; i < noUpdate.size(); i++)
			{
				if (!stat.empty()) stat += ",";
				stat += to_string(noUpdate.at(i).ID);
			}
			stat += ")";
			stat = "PACKAGE_ROUTE WHERE PACKAGEID = " + to_string(aPackage.ID) + " AND ID NOT IN (" + stat;
		}

		if (Operation(stat, 3))
		{
			auto insert = from(item2) >> where([&](Package_Route const& a)
			{ return a.ID == 0; }) >> to_vector();
			if (insert.empty()) return true;
			stat = "PACKAGE_ROUTE (routeid, packageid) values ";
			for (int i = 0; i < insert.size(); i++)
			{
				if (i != 0) stat += ",";
				stat += "(" + to_string(insert.at(i).RouteID) + ", " + to_string(aPackage.ID) + ")";
			}
			return Operation(stat, 1);
		}
	}
	return false;
}

/***ONLY FOR DEACTIVATING & ACTIVATING USER***/
template<>
bool CtmsDB::Update<User>(User item)
{
	string stat = "USER SET ISACTIVE = ";
	stat += (item.IsActive) ? "1" : "0";
	stat += " WHERE ID = " + to_string(item.Id) + "";
	return Operation(stat, 2);
}

template<>
bool CtmsDB::Update<User, User_Role>(User item, vector<User_Role> user_roles)
{
	string stat = "USER SET USERNAME = '" + item.Username + "' , PASSWORD = '" + item.Password + "' WHERE ID = " + to_string(item.Id);
	if (Operation(stat, 2))
	{
		auto noUpdate = from(user_roles) >> where([&](User_Role const& b)
		{ return b.Id != 0; }) >> to_vector();

		if (noUpdate.empty()){
			stat = "USER_ROLE WHERE USERID = " + to_string(item.Id);
		}
		else{
			stat = "";
			for (int i = 0; i < noUpdate.size(); i++)
			{
				if (!stat.empty()) stat += ",";
				stat += to_string(noUpdate.at(i).Id);
			}
			stat += ")";
			stat = "USER_ROLE WHERE USERID = " + to_string(item.Id) + " AND ID NOT IN (" + stat;
		}

		if (Operation(stat, 3))
		{
			auto insert = from(user_roles) >> where([&](User_Role const& a)
			{ return a.Id == 0; }) >> to_vector();
			if (insert.empty()) return true;

			stat = "USER_ROLE (UserID, RoleID) values ";
			for (int i = 0; i < insert.size(); i++)
			{
				if (i != 0) stat += ",";
				stat += "(" + to_string(item.Id) + ", " + to_string(insert.at(i).RoleID) + ")";
			}
			return Operation(stat, 1);
		}
	}
	return false;
}

template<>
bool CtmsDB::Delete<Deck>(Deck item)
{
	string stat = "DECK WHERE ID = " + to_string(item.Id);
	return Operation(stat, 3);
}

template<>
bool CtmsDB::Delete<Route>(Route item)
{
	string stat = "ROUTE WHERE ID = " + to_string(item.ID);
	return Operation(stat, 3);
}

template<>
bool CtmsDB::Delete<Vessel>(Vessel item)
{
	string stat = "DECK WHERE VESSEL_ID = " + to_string(item.ID);
	if (Operation(stat, 3))
	{
		stat = "VESSEL WHERE ID = " + to_string(item.ID);
		return Operation(stat, 3);
	}
	return false;
}

template<>
bool CtmsDB::Delete<User>(User item)
{
	string stat = "USER_ROLE WHERE UserID = " + to_string(item.Id);
	if (Operation(stat, 3))
	{
		string stat = "USER WHERE ID = " + to_string(item.Id);
		return Operation(stat, 3);
	}
	return false;
}

template<>
bool CtmsDB::Delete<Package>(Package item)
{
	string stat = "PACKAGE_ROUTE WHERE PackageID = " + to_string(item.ID);
	if (Operation(stat, 3))
	{
		string stat = "PACKAGE WHERE ID = " + to_string(item.ID);
		return Operation(stat, 3);
	}
	return false;
}

bool CtmsDB::Operation(string Query, int Type)
{
	if (Type == 1) db.statement = "INSERT INTO ";
	else if (Type == 2) db.statement = "UPDATE ";
	else db.statement = "DELETE FROM ";

	db.statement += Query;
	if (db.cud())
	{
		initData();
		return true;
	}
	else return false;
}
