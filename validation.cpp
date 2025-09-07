#include "validation.h"
#include <iostream>
#include <algorithm>
#include <cctype>
#include <sstream>
#include <iomanip>

using namespace std;

// Cancel Helper
bool isCancel(const string& input) {
    string trimmed = input;
    // Trim leading spaces
    trimmed.erase(trimmed.begin(), find_if(trimmed.begin(), trimmed.end(), [](unsigned char ch) { return !isspace(ch); }));
    // Trim trailing spaces
    trimmed.erase(find_if(trimmed.rbegin(), trimmed.rend(), [](unsigned char ch) { return !isspace(ch); }).base(), trimmed.end());

    return (trimmed == "q" || trimmed == "Q" || trimmed == "-1");
}

//  Integer Input
int getIntInput(const string& prompt, bool allowCancel) {
    string input;
    while (true) {
        cout << prompt;
        getline(cin, input);

        if (allowCancel && isCancel(input)) return -1;

        try {
            size_t idx;
            int value = stoi(input, &idx);
            if (idx != input.size()) throw invalid_argument("Invalid characters");
            return value;
        }
        catch (...) {
            cout << "Invalid input. Please enter a valid integer.\n";
        }
    }
}

//  Double Input 
double getDoubleInput(const string& prompt, bool allowCancel) {
    string input;
    while (true) {
        cout << prompt;
        getline(cin, input);

        if (allowCancel && isCancel(input)) return -1.0;

        try {
            size_t idx;
            double value = stod(input, &idx);
            if (idx != input.size()) throw invalid_argument("Invalid characters");
            if (value < 0) {
                cout << "Number must be positive.\n";
                continue;
            }
            return value;
        }
        catch (...) {
            cout << "Invalid input. Please enter a number.\n";
        }
    }
}

// Confirmation y/n
bool getConfirmation(const string& prompt) {
    while (true) {
        string input;
        cout << prompt;
        getline(cin, input);

        if (input == "y" || input == "Y") return true;
        if (input == "n" || input == "N") return false;

        cout << "Invalid input. Please enter 'y' or 'n'.\n";
    }
}

// String Helper 
bool containsNumber(const string& str) {
    return any_of(str.begin(), str.end(), ::isdigit);
}

bool isDigitsOnly(const string& str) {
    return !str.empty() && all_of(str.begin(), str.end(), ::isdigit);
}

bool isValidNameOrLocation(const string& input) {
    if (input.empty()) return false;
    return all_of(input.begin(), input.end(), [](unsigned char ch) {
        return isalpha(ch) || isspace(ch) || ch == ',';
        });
}

bool isValidEventName(const string& str) {
    if (str.empty()) return false;
    return all_of(str.begin(), str.end(), [](unsigned char ch) {
        return isalnum(ch) || isspace(ch);
        });
}

// Date Validation
bool isValidDate(const string& input) {
    if (input.size() != 10) return false; // strict format

    tm tmStruct{};
    istringstream ss(input);
    ss >> get_time(&tmStruct, "%Y-%m-%d");
    if (ss.fail()) return false;

    if (tmStruct.tm_mon < 0 || tmStruct.tm_mon > 11) return false;
    if (tmStruct.tm_mday < 1 || tmStruct.tm_mday > 31) return false;

    // check if date is reasonable (not before 1970)
    time_t t = mktime(&tmStruct);
    return t != -1;
}
