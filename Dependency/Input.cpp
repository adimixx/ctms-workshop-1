//
// Created by Adi Iman on 25/04/2020.
//

#include "Input.h"

int input::InputInt(string coutText)
{
	return processInput<int>(coutText, 0, 0, false, false);
}

int input::InputInt(string coutText, int minVal)
{
	return processInput<int>(coutText, minVal, 0, true, false);
}

int input::InputInt(string coutText, int minVal, int maxVal)
{
	return processInput<int>(coutText, minVal, maxVal, true, true);
}

double input::InputDouble(string coutText)
{
	return processInput<double>(coutText, 0, 0, false, false);
}

double input::InputDouble(string coutText, int minVal)
{
	return processInput<double>(coutText, minVal, 0, true, false);
}

double input::InputDouble(string coutText, int minVal, int maxVal)
{
	return processInput<double>(coutText, minVal, maxVal, true, true);
}

string input::getInput(string coutText)
{
	string input;
	do
	{
		cout << coutText + " : ";
		getline(cin, input);
		input = trim(input);
		if (input.empty())
		{
			cout << "input cannot be empty. Please re-enter your input\n";
		}
	} while (trim(input).empty());
	return input;
}

string input::getInput(string coutText, bool nullable)
{
	if (nullable == false)
	{
		return getInput(coutText);
	}
	else
	{
		string input;
		cout << coutText + " : ";
		getline(cin, input);
		return input;
	}
}

string input::InputDate(string coutText, Database db)
{
	bool check = false;
	string conv = "";
	do
	{
		conv = ConvertDate(getInput(coutText), db);
		if (conv != "")
		{
			check = true;
		}
	} while (!check);
	return conv;
}

string input::InputDateWithTime(string coutText, Database db)
{
	bool check = false;
	string conv = "", convTime = "";
	do
	{
		conv = ConvertDate(getInput(coutText), db);
		if (conv != "")
		{
			do
			{
				convTime = ConvertTime(getInput("Enter Time (HHmm)"), db);
				if (convTime != "")
				{
					check = true;
				}
			} while (!check);
		}
	} while (!check);
	return conv + " " + convTime;
}

string input::ConvertDate(string input, Database db)
{
	db.statement = "SELECT STR_TO_DATE('" + input + "','%d/%m/%Y')";
	db.select();
	db.row = db.FetchRow();
	if (db.row[0] == nullptr)
	{
		cout << "Invalid Date format. Please re-enter your date\n";
		return "";
	}
	return string(db.row[0]);
}

string input::ConvertTime(string input, Database db)
{
	db.statement = "SELECT STR_TO_DATE('" + input + "','%H%i')";
	db.select();
	db.row = db.FetchRow();
	if (db.row[0] == nullptr)
	{
		cout << "Invalid Time format. Please re-enter your date\n";
		return "";
	}
	return string(db.row[0]);
}

string input::ConvertDateAndTime(string input, Database db)
{
	db.statement = "SELECT STR_TO_DATE('" + input + "','%d/%m/%Y %H%i')";
	db.select();
	db.row = db.FetchRow();
	if (db.row[0] == nullptr)
	{
		cout << "Invalid Date/Time format. Please re-enter your date\n";
		return "";
	}
	return string(db.row[0]);
}

string input::getDateTimeNow(Database db)
{
	db.statement = "SELECT NOW()";
	db.select();
	db.row = db.FetchRow();
	if (db.row[0] == nullptr)
	{
		return "";
	}
	return string(db.row[0]);
}

string input::AddHour(string date, int hours, Database db)
{
	db.statement = "select date_add('" + date + "',interval " + to_string(hours) + " hour)";
	db.select();
	db.row = db.FetchRow();
	if (db.row[0] == nullptr)
	{
		return "";
	}
	return string(db.row[0]);
}

string input::AddDay(string date, int day, Database db)
{
	if (date != "") db.statement = "select date_add('" + date + "' ";
	else db.statement = "select date_add(NOW() ";
	db.statement += ",interval " + to_string(day) + " day)";
	db.select();
	db.row = db.FetchRow();
	if (db.row[0] == nullptr)
	{
		return "";
	}
	return string(db.row[0]);
}

bool input::compareDate(string from, string to, Database db, bool coutB)
{
	if (to == "") db.statement = "select datediff(NOW() ,'" + from + "');";
	else if ( from == "") db.statement = "select datediff('" + to + "',NOW());";
	else db.statement = "select datediff('" + to + "','" + from + "');";
	db.select();
	db.row = db.FetchRow();
	if (db.row[0] == nullptr || stoi(string(db.row[0])) <= 0)
	{
		if (stoi(string(db.row[0])) <= 0 && coutB) cout << "Start date must be greater than end date\n";
		return false;
	}
	else
	{
		return true;
	}
}

long input::compareMinute(string from, string to, Database db)
{
	db.statement = "select timestampdiff(minute,";
	if (from == "") db.statement += "NOW(), ";
	else db.statement += "'"+from+"', ";
	if (to  == "") db.statement += "NOW())";
	else db.statement += "'"+to+"')";
	db.select();

	db.row = db.FetchRow();
	if (db.row[0] == nullptr || stoi(string(db.row[0])) <= 0)
	{
		return 0;
	}
	else
	{
		return stol(string(db.row[0]));
	}
}

string input::sanitizeString(string input)
{
	return input;
}

template<class T>
T input::processInput(string coutText, T min, T max, bool checkMin, bool checkMax)
{
	T convert = 0;
	bool checked = true;
	do
	{
		try
		{
			checked = true;
			string input = getInput(coutText);

			if (is_same<T, int>::value)
			{
				convert = stoi(input);
			}
			else if (is_same<T, double>::value)
			{
				convert = stod(input);
			}

			if (checkMin && convert < min)
			{
				cout << "input must be greater than " << min << endl;
				checked = false;
			}
			else if (checkMax && convert > max)
			{
				cout << "input must be less than " << max << endl;
				checked = false;
			}
		}
		catch (exception e)
		{
			cout << "Invalid input. Please re-enter your input\n";
			checked = false;
		}
	} while (!checked);
	return convert;
}

string input::trim(const string& s)
{
	return ltrim(rtrim(s));
}

string input::ltrim(const string& s)
{
	return std::regex_replace(s, std::regex("^\\s+"), std::string(""));
}

string input::rtrim(const string& s)
{
	return std::regex_replace(s, std::regex("\\s+$"), std::string(""));
}

string input::toString(double input, int precision)
{
	ostringstream out;
	out.precision(precision);
	out << fixed << input;
	return out.str();
}


