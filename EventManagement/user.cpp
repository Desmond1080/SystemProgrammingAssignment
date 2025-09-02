#include <iostream>
#include <string>
#include <algorithm>
#include "function.h"


using namespace std;

//user sign up 
void userRegister() {

	User user1;
	cout << "=== User Registration ===" << endl;

	//clear input buffer before taking input
	cin.ignore(numeric_limits<streamsize>::max(), '\n');

	cout << "Enter First Name: ";
	getline(cin, user1.username.firstname);

	cout << "Enter Last Name: ";
	getline(cin, user1.username.lastname);

	cout << "Enter Nickname: ";
	getline(cin, user1.nickname);

	cout << "Enter Age: ";
	cin >> user1.age;
	cin.ignore(); //clear new line character

	while (true) {
		cout << "Enter gender (Male/Female): ";
		getline(cin, user1.gender);
		if (validateGender(user1.gender)) {
			break;
		}
		else {
			cout << "Invalid gender. Re-enter" << endl;
		}
	}

	while (true) {
		cout << "Enter contact number (10 digits): ";
		getline(cin, user1.contactNumber);
		if (validateContactNumber(user1.contactNumber)) {
			break;
		}
		else {
			cout << "Invalid contact number. Re-enter." << endl;
		}
	}

	while (true) {
		cout << "Enter email: ";
		getline(cin, user1.email);
		if (validateEmail(user1.email)) {
			break;
		}
		else {
			cout << "Invalid email. Re-enter." << endl;
		}
	}

	while (true) {
		cout << "Enter password (At least 8 characters): ";
		getline(cin, user1.password);
		if (validatePassword(user1.password)) {
			break;
		}
		else {
			cout << "Invalid password. Re-enter." << endl;
		}
	}

}