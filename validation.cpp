#include "validation.h"
#include <iostream>
#include <algorithm>
#include <cctype>

using namespace std;

// cancel helper function
bool isCancel(const std::string& input) {
    std::string trimmed = input;
    // remove any empty spaces
    trimmed.erase(trimmed.begin(), std::find_if(trimmed.begin(), trimmed.end(), [](unsigned char ch) {
        return !std::isspace(ch);
        }));
    trimmed.erase(std::find_if(trimmed.rbegin(), trimmed.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
        }).base(), trimmed.end());

    return (trimmed == "q" || trimmed == "Q" || trimmed == "-1");
}

// Helper function for integer input
int getIntInput(const std::string& prompt, bool allowCancel) {
    std::string input;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, input);

        if (allowCancel && isCancel(input)) return -1;

        // Check that every character is a digit
        if (!input.empty() && std::all_of(input.begin(), input.end(), ::isdigit)) {
            return std::stoi(input);
        }

        std::cout << "Invalid input. Please enter a valid number.\n";
    }
}

// Helper function for double input
double getDoubleInput(const string& prompt, bool allowCancel) {
    string input;
    while (true) {
        cout << prompt;
        getline(cin, input);

        if (allowCancel && isCancel(input)) return -1.0;

        try {
            return stod(input);
        }
        catch (...) {
            cout << "Invalid input. Please enter a number.\n";
        }
    }
}

// Helper function for confirmation (y/n)
bool getConfirmation(const std::string& prompt) {
    while (true) {
        std::string input;
        std::cout << prompt;
        std::getline(std::cin, input);

        if (input == "y" || input == "Y") return true;
        if (input == "n" || input == "N") return false;

        std::cout << "Invalid input. Please enter 'y' or 'n'.\n";
    }
}

// Helper: check if string contains any digits
bool containsNumber(const string& str) {
    return any_of(str.begin(), str.end(), ::isdigit);
}

// Validate names and locations (only letters, spaces and commas allowed)
bool isValidNameOrLocation(const std::string& input) {
    if (input.empty()) return false;

    // Check each character
    return std::all_of(input.begin(), input.end(), [](unsigned char ch) {
        return std::isalpha(ch) || std::isspace(ch) || ch == ','; // only letters, spaces and commas allowed
        });
}

// Event name validation: letters, numbers, spaces only
bool isValidEventName(const std::string& str) {
    for (char c : str) {
        if (!isalnum(c) && !isspace(c)) return false;
    }
    return !str.empty();
}

