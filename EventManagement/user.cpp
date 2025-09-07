#include <iostream>
#include <string>
#include <algorithm>
#include "function.h"


using namespace std;

//before user login 
bool userLoginMenu(User &user) {
	int choice;
	User loginUser;

	while (true) {
		cout << "=== User Page ===" << endl;
		cout << "1. Login" << endl;
		cout << "2. Register" << endl;
		cout << "3. Forget Password" << endl;
		cout << "4. Back to Main Menu" << endl;
		cout << "Enter your choice: ";

		// Check if input is valid
		if (!(cin >> choice)) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			clearScreen();
			cout << "Invalid input. Please enter a number." << endl;
			continue;
		}
		
		if (validateChoice(choice,1 ,4)) {
			switch (choice) {
			case 1:
				clearScreen();
				if (userLogin(loginUser)) {
					user = loginUser;
					return true; // login successful
				}
				break;
			case 2:
				clearScreen();
				userRegister();
				break;
			case 3:
				clearScreen();
				{
					string email;
					cout << "Enter your registered email: ";
					getline(cin, email);
					forgotPassword(email);
				}
				break;
			case 4:
				clearScreen();
				return false; // back to main menu
			default:
				cout << "Invalid choice. Please try again." << endl;
			}
		}
		else {
			cout << endl << endl;
			clearScreen();
			cout << "Invalid input. Please enter a number between 1 and 4." << endl;
		}

	}
}

// after user login 
void userMenu(User& user) {
	int choice;
	while (true) {
		cout << "=== User Menu ===" << endl;
		cout << "1. View Profile" << endl;
		cout << "2. Edit Profile" << endl;
		cout << "3. Delete Account" << endl;
		cout << "4. Forget Password" << endl;
		cout << "5. Logout" << endl;
		cout << "Enter your choice: ";
		
		// Check if input is valid
		if (!(cin >> choice)) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Invalid input. Please enter a number." << endl;
			continue;
		}

		if (validateChoice(choice,1,5)) {
			switch (choice) {
			case 1:
				displayUserProfile(user);
				break;
			case 2:
				editUserProfile(user);
				break;
			case 3:
				deleteUserAccount(user);
				return; // exit user menu after account deletion
			case 4:
				forgotPassword(user.email);
				break;
			case 5:
				cout << "Logging out..." << endl;
				clearScreen();
				return; // exit user menu to logout
			default:
				cout << "Invalid choice. Please try again." << endl;
			}
		}
		else {
			cout << "Invalid input. Please enter a number between 1 and 5." << endl;
		}
	}
}

//user sign up 
void userRegister() {

	User user1;
	cout << "=== User Registration ===" << endl;

	//clear input buffer before taking input
	cin.ignore(numeric_limits<streamsize>::max(), '\n');

	while (true) {
		cout << "Enter First Name: ";
		getline(cin, user1.username.firstname);
		if (validateName(user1.username.firstname) && validateLine(user1.username.firstname)) {
			break;
		}
		else {
			cout << "Invalid first name input. Re-enter and first name cannot contain '|'." << endl;
		}
	}

	while (true) {
		cout << "Enter Last Name: ";
		getline(cin, user1.username.lastname);
		if (validateName(user1.username.lastname) && validateLine(user1.username.lastname)) {
			break;
		}
		else {
			cout << "Invalid last name input. Re-enter and last name cannot contain '|'." << endl;
		}
	}

	while (true) {
		cout << "Enter Nickname: ";
		getline(cin, user1.nickname);
		if (!user1.nickname.empty() && validateLine(user1.nickname)) {
			break;
		}
		else {
			cout << "Ninckname cannot be empty or contain '|' . Re-enter." << endl;
		}
	}

	while (true) {
		cout << "Enter Age: ";
		getline(cin, user1.age);
		if (validateAge(user1.age)) {
			break;
		}
		else {
			cout << "Invalid age input. Re-enter" << endl;
		}
	}

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
		cout << "Enter contact number (012-3456789 or 012-34567891): ";
		getline(cin, user1.contactNumber);
		if (validateContactNumber(user1.contactNumber) && validateLine(user1.contactNumber)) {
			break;
		}
		else {
			cout << "Invalid contact number. Re-enter." << endl;
		}
	}

	while (true) {
		cout << "Enter email: ";
		getline(cin, user1.email);
		if (validateEmail(user1.email) && validateLine(user1.email)) {
			// check if the email already exits
			User tempUser;
			if (loadUserProfile(user1.email, tempUser)) {
				cout << "Email already registered. Please use a different email." << endl;
			}
			else {
				break;
			}
			break;
		}
		else {
			cout << "Invalid email. Re-enter and field cannot contain '|'." << endl;
		}
	}

	while (true) {
		cout << "Enter password (At least 8 characters): ";
		getline(cin, user1.password);
		if (validatePassword(user1.password) && validateLine(user1.password)) {
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
	//clear input buffer before taking input
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	string email, password;
	User user1;

	cout << "=== User Login ===" << endl;

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
				bool validChoice = false;
				while (!validChoice) {
					cin >> choice;
					cin.ignore(numeric_limits<streamsize>::max(), '\n');

					if (validatePasswordConfirmation(choice)) {
						validChoice = true;
						if (choice == 'Y' || choice == 'y') {
							forgotPassword(email);
						}
						else {
							cout << "Please try again" << endl;
						}
					}
					else {
						cout << "Invalid input please field with y or n " << endl;
					}
				} 
				updateUserProfile(user1);
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