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
			//generate salt and hash password 
			user1.salt = generateSalt();
			user1.passwordHash = hashPassword(user1.password, user1.salt);
			break;
		}
		else {
			cout << "Invalid password. Re-enter." << endl;
		}
	}

	user1.loginAttempts = 0;

	saveUserProfile(user1);
	cout << "Registration successful!" << endl;


}

// user login
bool userLogin(User &loginUser) {
	string email, password;
	User user1;

	cout << "=== User Login ===" << endl;

	//clear input buffer before taking input
	cin.ignore(numeric_limits<streamsize>::max(), '\n');

	cout << "Enter email: ";
	getline(cin, email);

	// compare email to load user profile check if it is true or false
	if (!loadUserProfile(email, user1)) {
		cout << "Login failed. Invalid email or password." << endl;
		return false;
	}

	int attemps = 0;
	while (attemps < MAX_LOGIN_ATTEMPTS) {
		cout << "Enter password: ";
		getline(cin, password);

		//password hashing and comparison
		string passwordHash = hashPassword(password, user1.salt);

		if (passwordHash == user1.passwordHash) {
			cout << "Login successful! Welcome, " << user1.nickname << endl;
			user1.loginAttempts = 0; //reset login attempts after successful login
			loginUser = user1;
			updateUserProfile(user1);
			clearScreen();// update the login attempt
			return true;
		}
		else {
			attemps++;
			user1.loginAttempts++;
			cout << "Login failed. Invalid email or password." << endl;
			cout << "Login attempts: " << user1.loginAttempts << endl;
			cout << "You have " << (MAX_LOGIN_ATTEMPTS - attemps) << " attempts left." << endl;

			if (attemps >= MAX_LOGIN_ATTEMPTS) {
				cout << "Too many failed login attempts. " << endl;
				cout << "Please try again later or use the 'Forgot Password' option (Y/N)." << endl;
				char choice;
				cin >> choice;
				if(choice == 'Y' || choice == 'y') {
					forgotPassword(email);
				}
				updateUserProfile(user1); // update the login attempt
				return false;

			}
		}
	}
	updateUserProfile(user1); // update the login attempt
	return false;

}

// display user profile 
void displayUserProfile(const User& user) {
	cout << "=== User Profile ===" << endl;
	cout << "Name: " << user.username.firstname << " " << user.username.lastname << endl;
	cout << "Nickname: " << user.nickname << endl;
	cout << "Age: " << user.age << endl;
	cout << "Gender: " << user.gender << endl;
	cout << "Contact Number: " << user.contactNumber << endl;
	cout << "Email: " << user.email << endl;
}

// edit user profile 
void editUserProfile(User& user) {
	int choice;
	string field;
	do {
		cout << "=== Edit Profile ===" << endl;
		cout << "1. Edit First Name" << endl;
		cout << "2. Edit Last Name" << endl;
		cout << "3. Edit Nickname" << endl;
		cout << "4. Edit Age" << endl;
		cout << "5. Edit Gender" << endl;
		cout << "6. Edit Contact Number" << endl;
		cout << "7. Edit Email" << endl;
		cout << "8. Edit Password" << endl;
		cout << "9. Exit Edit Profile" << endl;
		cout << "Enter your choice: ";

		cin >> choice;
		cin.ignore();

		switch (choice) {
		case 1:
			cout << "Enter new First Name: ";
			getline(cin, field);
			if (!field.empty()) {
				user.username.firstname = field;
				cout << "First Name updated successfully!" << endl;
			}
			break;

		case 2:
			cout << "Enter new Last Name: ";
			getline(cin, field);
			if (!field.empty()) {
				user.username.lastname = field;
				cout << "Last Name updated successfully!" << endl;
			}
			break;

		case 3:
			cout << "Enter new Nickname: ";
			getline(cin, field);
			if (!field.empty()) {
				user.nickname = field;
				cout << "Nickname updated successfully!" << endl;
			}
			break;
		case 4:
			cout << "Enter new Age: ";
			getline(cin, field);
			if (!field.empty()) {
				user.age = field;
				cout << "Age updated successfully!" << endl;
			}
			break;
		case 5:
			cout << "Enter new Gender: ";
			getline(cin, field);
			if (!field.empty() && validateGender(field)) {
				user.gender = field;
				cout << "Gender updated successfully!" << endl;
			}
			break;
		case 6:
			cout << "Enter new Contact Number: ";
			getline(cin, field);
			if (!field.empty() && validateContactNumber(field)) {
				user.contactNumber = field;
				cout << "Contact Number updated successfully!" << endl;
			}
			else {
				cout << "Invalid contact number. Update failed." << endl;
			}
			break;
		case 7:
			cout << "Enter new Email: ";
			getline(cin, field);
			if (!field.empty() && validateEmail(field)) {
				user.email = field;
				cout << "Email updated successfully!" << endl;
			}
			else {
				cout << "Invalid email. Update failed." << endl;
			}
			break;
		case 8:
			cout << "Enter new Password: ";
			getline(cin, field);
			if (!field.empty() && validatePassword(field)) {
				user.password = field;
				user.salt = generateSalt();
				user.passwordHash = hashPassword(user.password, user.salt);
				cout << "Password updated successfully!" << endl;
			}
			else {
				cout << "Invalid password. Update failed." << endl;
			}
			break;
		case 9:
			cout << "Exiting Edit Profile." << endl;
			//save updated profile to file
			updateUserProfile(user);
			break;
		default:
			cout << "Invalid choice. Please try again." << endl;
		}

		if (choice >= 1 && choice <= 8) {
			updateUserProfile(user); //save after each successful update
		}

	} while (choice >= 1 && choice <= 7);
}

// delete user account 
void deleteUserAccount(User& user) {
	char confirmation;
	cout << "Are you sure you want to delete your account? (Y/N): ";
	cin >> confirmation;

	if (confirmation == 'Y' || confirmation == 'y') {
		removeUserProfile(user.email);
		cout << "Account deleted successfully." << endl;
	}
	else {
		cout << "Account deletion cancelled." << endl;
	}
}