//
// Created by Adi Iman on 18/04/2020.
//

#include "Database.h"

MYSQL_ROW Database::FetchRow()
{
	return mysql_fetch_row(res);
}

bool Database::ConnectionFunction()
{
	cout << "Initiating Database Connection" << endl;

	conn = mysql_init(0);
	if (conn)
	{
		cout << "Connection successfully initiated" << endl;
	}
	else
	{
		cout << "Database initiation failed\n" << mysql_errno(conn) << endl;
		return false;
	}

	cout << "Connecting to Database : " << conn << endl;

	conn = mysql_real_connect(conn, "192.168.99.102", "mysql8-ADM", "x2655DeF@", "CTMS_WORKSHOP1", 3306, NULL, 0);
	if (conn)
	{
		cout << "Database Connected To MySql : " << conn << endl;
	}
	else
	{
		cout << "Failed To Connect!\n" << mysql_errno(conn) << endl;
		return false;
	}
	return true;
}

void Database::select()
{
	data = mysql_query(conn, statement.c_str());

	if (!data)
	{
		res = mysql_store_result(conn);
	}
	else
	{
		cout << "Please check your query : \n" << statement << endl;
	}
}

bool Database::cud()
{
	data = mysql_query(conn, statement.c_str());

	if (!data) return true;
	return false;
}

int Database::RowCount()
{
	return static_cast<int>(res->row_count);
}

int Database::LastInsertedID()
{
	return static_cast<int>(conn->insert_id);
}