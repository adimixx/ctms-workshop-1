//
// Created by Adi Iman on 25/04/2020.
//

#ifndef WORKSHOP1_INPUT_H
#define WORKSHOP1_INPUT_H

#include <string>
#include <ctime>
#include <regex>
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
    static bool compareDate(string from, string to, Database db, bool);

    static string sanitizeString(string input);

    template <class T>
    static T processInput(string coutText, T min, T max, bool, bool);
private:
    static string ltrim(const string& s);
    static string rtrim(const string& s);
    static string trim(const string& s);
};
#endif //WORKSHOP1_INPUT_H
