//
// Created by Adi Iman on 25/04/2020.
//

#include "Input.h"

int input::InputInt(string coutText) {
    return processInput<int>(coutText, 0, 0, false, false);
}

int input::InputInt(string coutText, int minVal) {
    return processInput<int>(coutText, minVal, 0, true, false);
}

int input::InputInt(string coutText, int minVal, int maxVal) {
    return processInput<int>(coutText, minVal, maxVal, true, true);
}

double input::InputDouble(string coutText) {
    return processInput<double>(coutText, 0, 0,false, false);
}

double input::InputDouble(string coutText, int minVal) {
    return processInput<double>(coutText, minVal, 0, true, false);
}

double input::InputDouble(string coutText, int minVal, int maxVal) {
    return processInput<double>(coutText, minVal, maxVal, true, true);
}

string input::getInput(string coutText) {
    string input;
    do {
        cout << coutText + " : ";
        getline(cin, input);
        input = trim(input);
        if (input.empty()) {
            cout << "input cannot be empty. Please re-enter your input\n";
        }
    } while (trim(input).empty());
    return input;
}

string input::getInput(string coutText, bool nullable) {
    if (nullable == false) {
        return getInput(coutText);
    } else {
        string input;
        cout << coutText + " : ";
        getline(cin, input);
        return input;
    }
}

string input::InputDate(string coutText, Database db) {
    do {
        db.statement = "SELECT STR_TO_DATE('" + getInput(coutText) + "','%d/%m/%Y')";
        db.select();
        db.row = db.FetchRow();
        if (db.row == NULL) {
            cout << "Invalid Date format. Please re-enter your date\n";
        }
    } while (db.row == NULL);
    return string(db.row[0]);
}

bool input::compareDate(string from, string to, Database db, bool coutB) {
    db.statement = "select datediff('" + to + "','" + from + "');";
    db.select();
    db.row = db.FetchRow();
    if (db.row == NULL || stoi(string(db.row[0])) <= 0) {
        if (stoi(string(db.row[0])) <= 0 && coutB) cout << "Start date must be greater than end date\n";
        return false;
    } else {
        return true;
    }
}

string input::sanitizeString(string input) {
    return input;
}

template<class T>
T input::processInput(string coutText, T min, T max, bool checkMin, bool checkMax) {
    T convert = 0;
    bool checked = true;
    do {
        try {
            string input = getInput(coutText);

            if (is_same<T, int>::value) {
                convert = stoi(input);
            } else if (is_same<T, double>::value) {
                convert = stod(input);
            }

            if (checkMin && convert < min) {
                cout << "input must be greater than " << min << endl;
                checked = false;
            } else if (checkMax && convert > max) {
                cout << "input must be less than " << max << endl;
                checked = false;
            }
        }
        catch (exception e) {
            cout << "Invalid input. Please re-enter your input\n";
            checked = false;
        }
    } while (!checked);
    return convert;
}

string input::trim(const string &s) {
    return ltrim(rtrim(s));
}

string input::ltrim(const string &s) {
    return std::regex_replace(s, std::regex("^\\s+"), std::string(""));
}

string input::rtrim(const string &s) {
    return std::regex_replace(s, std::regex("\\s+$"), std::string(""));
}

