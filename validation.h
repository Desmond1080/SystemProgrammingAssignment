#ifndef VALIDATION_H
#define VALIDATION_H

#include <string>
#include <ctime>

// Cancellation helper
bool isCancel(const std::string& input);

// Input helpers
int getIntInput(const std::string& prompt, bool allowCancel = false);
double getDoubleInput(const std::string& prompt, bool allowCancel = false);
bool getConfirmation(const std::string& prompt);

// String validation
bool containsNumber(const std::string& str);
bool isDigitsOnly(const std::string& str);
bool isValidNameOrLocation(const std::string& input); // letters, spaces, commas
bool isValidEventName(const std::string& str);        // letters, numbers, spaces

// Date validation
bool isValidDate(const std::string& input); // YYYY-MM-DD

#endif
