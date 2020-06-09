//
// Created by Adi Iman on 25/04/2020.
//

#ifndef WORKSHOP1_INPUT_H
#define WORKSHOP1_INPUT_H

#include <string>
#include <ctime>
#include <regex>
#include <sstream>
#include "Database.h"
using namespace std;

class input
{
public:
	static int InputInt(string coutText);
	static int InputInt(string coutText, int minVal);
	static int InputInt(string coutText, int minVal, int maxVal);

	static double InputDouble(string coutText);
	static double InputDouble(string coutText, int minVal);
	static double InputDouble(string coutText, int minVal, int maxVal);

	static string getInput(string coutText);
	static string getInput(string coutText, bool nullable);

	static string InputDate(string coutText, Database db);
	static string InputDateWithTime(string coutText, Database db);

	static string ConvertDate(string input, Database db);
	static string ConvertTime(string input, Database db);
	static string ConvertDateAndTime(string input, Database db);
	static string AddHour(string date,int hours, Database db);
	static string AddDay(string date,int day, Database db);

	static string getDateTimeNow(Database db);
	static bool compareDate(string from, string to, Database db, bool);
	static long compareMinute(string from, string to, Database db);

	static string sanitizeString(string input);
	static string toString(double input, int precision);

	template<class T>
	static T processInput(string coutText, T min, T max, bool, bool);
private:
	static string ltrim(const string& s);
	static string rtrim(const string& s);
	static string trim(const string& s);
};
#endif //WORKSHOP1_INPUT_H
