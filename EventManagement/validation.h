#pragma once
#include <iostream>
#include <string>

// cancel helper
bool isCancel(const std::string& input);

// integer input helper with cancel option
int getIntInput(const std::string& prompt, bool allowCancel = true);

// double input helper with cancel option
double getDoubleInput(const std::string& prompt, bool allowCancel = true);

// confirmation helper (y/n)
bool getConfirmation(const std::string& prompt);

// check if string contains any numeric characters
bool containsNumber(const std::string& str);

// Validation for names and locations
bool isValidNameOrLocation(const std::string& str);

// Validation helper for event names (alphanumeric and spaces)
bool isValidEventName(const std::string& str);
// end of FUNCTION_H
