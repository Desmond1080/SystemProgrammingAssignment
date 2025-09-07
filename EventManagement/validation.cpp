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
	//check the format of contact number is it correct or not (is it 10 digit or 11 digit)
	regex regex_pattern(R"(^\d{3}-\d{7,8}$)");
	return regex_match(contactNumber, regex_pattern);
}

bool validateAge(string& age) {
	// only can accept number for the age 
	try {
		int ageNum = stoi(age);
		return ageNum > 0 && ageNum < 120; // assuming a valid age range
	}
	catch (...) {
		return false;
	}
}

bool validateChoice(int choice, int minChoice, int maxChoice) {
	return choice >= minChoice && choice <= maxChoice;
}

// validate if the input have '|'
bool validateLine(const string& input) {
	return input.find('|') == string::npos;
}

// validate name 
bool validateName(const string& name) {
	// only can accept letter and space for the name
	regex regex_pattern(R"(^[A-Za-z\s]+$)");
	return regex_match(name, regex_pattern);
}

// validate choice for password change confirmation
bool validatePasswordConfirmation(const char& input	) {
	return input == 'Y' || input == 'N' || input == 'y' || input == 'n';
}