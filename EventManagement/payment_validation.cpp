#include <regex>
#include <string>
#include <algorithm>
#include <iostream>
#include "payment.h"
#include <iostream>

using namespace std;

bool validateContactNumber(const string& phone) {
	//10 digit number 
	regex regex_pattern(R"(^01\d{8}$)");
	return regex_match(phone, regex_pattern);
}

bool validatePaymentName(const string& name) {
    // Name should be 2-50 characters, only letters and spaces
    if (name.length() < 2 || name.length() > 50) {
        return false;
    }

    // Check if name contains only letters, spaces, and apostrophes
    for (char c : name) {
        if (!isalpha(c) && c != ' ' && c != '\'' && c != '-') {
            return false;
        }
    }

    return true;
}

bool validateAmount(double amount) {
    // Amount should be positive and reasonable
    return (amount > 0.0 && amount <= 1000000.0); // RM 0 to 1,000,000
}

 