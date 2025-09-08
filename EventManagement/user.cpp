#include <iostream>
#include <string>
#include <algorithm>
#include "function.h"
#include "participantRegHeader.h"
#include "payment.h"

using namespace std;

//before user login 
bool userLoginMenu(User &user) {
	int choice;
	User loginUser;

	while (true) {
		clearScreen();
		cout << "=== User Page ===" << endl;
		cout << "1. Login" << endl;
		cout << "2. Register" << endl;
		cout << "3. Continue as Guest" << endl;
		cout << "4. Forgot Password" << endl;
		cout << "5. Back to Main Menu" << endl;
		cout << "Enter your choice: ";

		// Check if input is valid
		if (!(cin >> choice)) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			clearScreen();
			cout << "Invalid input. Please enter a number." << endl;
			continue;
		}
		
		if (validateChoice(choice,1 ,5)) {
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
				browseEvents(nullptr);
				break;
			case 4:
				clearScreen();
				{
					string email;
					cout << "Enter your registered email: ";
					getline(cin, email);
					forgotPassword(email);
				}
				break;
			case 5:
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
		clearScreen();
		cout << "=== User Menu ===" << endl;
		cout << "1. Browse Events" << endl;
		cout << "2. View Tickets" << endl;
		cout << "3. View Profile" << endl;
		cout << "4. Edit Profile" << endl;
		cout << "5. Delete Account" << endl;
		cout << "6. Forgot Password" << endl;
		cout << "7. Logout" << endl;
		cout << "Enter your choice: ";
		
		// Check if input is valid
		if (!(cin >> choice)) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Invalid input. Please enter a number." << endl;
			system("pause");
			continue;
		}

		if (validateChoice(choice,1,7)) {
			switch (choice) {
			case 1:
				browseEvents(&user);
				break;
			case 2:
				viewTickets(user);
				break;
			case 3:
				displayUserProfile(user);
				system("pause");
				break;
			case 4:
				editUserProfile(user);
				break;
			case 5:
				deleteUserAccount(user);
				return; // exit user menu after account deletion
			case 6:
				forgotPassword(user.email);
				break;
			case 7:
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
				continue;
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
	cout << endl << "Registration successful!" << endl;
	system("pause");
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
		system("pause");
		return false;
	}

	int attemps = 0;
	while (attemps < MAX_LOGIN_ATTEMPTS) {
		cout << "Enter password: ";
		getline(cin, password);

		//password hashing and comparison
		string passwordHash = hashPassword(password, user1.salt);

		if (passwordHash == user1.passwordHash) {
			user1.loginAttempts = 0; // reset login attempts after successful login
			loginUser = user1;
			updateUserProfile(user1);
			clearScreen();// update the login attempt
			cout << "Login successful! Welcome, " << user1.nickname << endl;
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
		clearScreen();
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
				system("pause");
			}
			break;

		case 2:
			cout << "Enter new Last Name: ";
			getline(cin, field);
			if (!field.empty()) {
				user.username.lastname = field;
				cout << "Last Name updated successfully!" << endl;
				system("pause");
			}
			break;

		case 3:
			cout << "Enter new Nickname: ";
			getline(cin, field);
			if (!field.empty()) {
				user.nickname = field;
				cout << "Nickname updated successfully!" << endl;
				system("pause");
			}
			break;
		case 4:
			cout << "Enter new Age: ";
			getline(cin, field);
			if (!field.empty()) {
				user.age = field;
				cout << "Age updated successfully!" << endl;
				system("pause");
			}
			break;
		case 5:
			cout << "Enter new Gender: ";
			getline(cin, field);
			if (!field.empty() && validateGender(field)) {
				user.gender = field;
				cout << "Gender updated successfully!" << endl;
				system("pause");
			}
			break;
		case 6:
			cout << "Enter new Contact Number: ";
			getline(cin, field);
			if (!field.empty() && validateContactNumber(field)) {
				user.contactNumber = field;
				cout << "Contact Number updated successfully!" << endl;
				system("pause");
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
				system("pause");
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
				system("pause");
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

// view tickets 
void viewTickets(User& user) {
	cout << "=== View Tickets ===" << endl;

	//load all payments from file 
	vector<Payment> payments = loadPaymentsFromFile();
	vector <Payment> userPayments;

	//filter payments for user 
	for (int i = 0; i < payments.size(); i++) {
		if (payments[i].email == user.email && payments[i].status == "Completed") {
			userPayments.push_back(payments[i]);
		}
	}

	if (userPayments.empty()) {
		cout << "You have no tickets purchased." << endl;
		system("pause");
		return;
	}

	//display user tickets 
	for (int i = 0; i < userPayments.size(); i++) {
		Payment p = userPayments[i];
		cout << "=== Ticket " << (i + 1) << " ===" << endl;
		cout << "Event Name: " << p.eventName << endl;
		cout << "Purchase Date: " << p.date << endl;
		cout << "Purchase Time: " << p.time << endl;
		cout << "Amount Paid: RM" << p.amount << endl;
		cout << "Payment Method: " << p.method << endl;

		//display tickets details 
		cout << "Tickets: " << endl;
		for (int j = 0; j < p.tickets.size(); j++) {
			cout << "  Category Index: " << p.tickets[j].first << ", Quantity: " << p.tickets[j].second << endl;
		}
		cout << "======================================" << endl << endl;
	}

	//option to view specific ticket details
	cout << "Enter the ticket number to view details or 0 to return:";
	int ticketChoice;
	//validate ticket choice input
	while (true) {
		if (!(cin >> ticketChoice)) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Invalid input. Please enter a number." << endl;
			continue;
		}
		if (validateChoice(ticketChoice, 0, userPayments.size())) {
			Payment selectedPayment = userPayments[ticketChoice - 1];
			cout << "=== Ticket Details ===" << endl;
			cout << "Event Name: " << selectedPayment.eventName << endl;
			cout << "Purchase Date: " << selectedPayment.date << endl;
			cout << "Purchase Time: " << selectedPayment.time << endl;
			cout << "Name : " << selectedPayment.name << endl;
			cout << "Email : " << selectedPayment.email << endl;
			cout << "Amount Paid: RM" << selectedPayment.amount << endl;
			cout << "Payment Method: " << selectedPayment.method << endl;

			cout << "\n Ticket category: " << endl;
			for (int j = 0; j < selectedPayment.tickets.size(); j++) {
				cout << "  Category Index: " << selectedPayment.tickets[j].first << ", Quantity: " << selectedPayment.tickets[j].second << endl;
			}
			system("pause");
			break;
		}
		else {
			cout << "Invalid choice. Please enter a number between 0 and " << userPayments.size() << "." << endl;
		}
	}
}