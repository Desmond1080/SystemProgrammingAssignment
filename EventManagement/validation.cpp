#include <regex>
#include <string>
#include <algorithm>
#include <iostream>
#include "function.h"
#include <iostream>

using namespace std;

bool validateGender(string& gender) {
	string temp = gender;
	transform(temp.begin(), temp.end(), temp.begin(), ::toupper);
	if (temp == "MALE" || temp == "FEMALE") {
		gender = temp;
		return true;
	}
	else {
		return false;
	}
}

bool validateEmail(string& email) {
	regex regex_pattern(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
	return std::regex_match(email, regex_pattern);
}

bool validatePassword(string& password) {
	//at least 8 characters, at least one letter and one number
	regex regex_pattern(R"(^(?=.*[A-Za-z])(?=.*\d)[A-Za-z\d]{8,}$)");
	return regex_match(password, regex_pattern);
}

bool validateContactNumber(string& contactNumber) {
	//10 digit number 
	regex regex_pattern(R"(^\d{10}$)");
	return regex_match(contactNumber, regex_pattern);
}