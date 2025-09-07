#include "function.h"
#include "structs.h"

using namespace std;

//organizer login menu
bool organizerLoginMenu(Organizer& loginOrganizer) {
	int choice;
	Organizer loginOrgan;

	while (true) {
		cout << "=== Organizer Page ===" << endl;
		cout << "1. Register" << endl;
		cout << "2. Login" << endl;
		cout << "3. Back to Main Menu" << endl;
		cout << "Enter your choice: ";
		
		// Check if input is valid
		if (!(cin >> choice)) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Invalid input. Please enter a number." << endl;
			continue;
		}

		if (validateChoice(choice,1,3)) {
			switch (choice) {
			case 1:
				clearScreen();
				organizerRegister();
				break;
			case 2:
				clearScreen();
				if (organizerLogin(loginOrgan)) {
					loginOrganizer = loginOrgan; //copy logged-in organizer data
					return true; //login successful
				}
				break;
			case 3:
				clearScreen();
				return false; //back to main menu
			default:
				cout << "Invalid choice. Please try again." << endl;
			}
		}
		else {
			cout << "Invalid input. Please enter a number between 1 and 3." << endl;
		}
	}
}

// organizer menu
void organizerMenu(Organizer& organizer) {
	int choice;
	while (true) {
		cout << "=== Organizer Page ===" << endl;
		cout << "1. View Profile" << endl;
		cout << "2. Edit Profile" << endl;
		cout << "3. Create Event" << endl;
		cout << "4. View Events" << endl;
		cout << "5. Add helper to the event" << endl;
		cout << "6. Delete Account" << endl;
		cout << "7. Logout" << endl;
		cout << "Enter your choice: ";
		
		// Check if input is valid
		if (!(cin >> choice)) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clean buffer
			cout << "Invalid input. Please enter a number." << endl;
			continue;
		}

		if (validateChoice(choice,1,7)) {
			switch (choice) {
			case 1:
				clearScreen();
				displayOrganizerProfile(organizer);
				break;
			case 2:
				clearScreen();
				editOrganizerProfile(organizer);
				break;
			case 3:
				clearScreen();
				break;
			case 4:
				clearScreen();
				break;
			case 5:
				clearScreen();
				choice = 6; // to exit after deletion
				break;
			case 6:
				clearScreen();
				cout << "Logging out..." << endl;
				break;
			default:
				cout << "Invalid choice. Please try again." << endl;
			}
		}
		else {
			cout << "Invalid input. Please enter a number between 1 and 6." << endl;
		}
	}
}

void organizerRegister() {
	Organizer organizer1;

	//clear input buffer before taking input
	cin.ignore(numeric_limits<streamsize>::max(), '\n');

	// organiser register 
	cout << "=== Organizer Registration ===" << endl;

	while (true) {
		cout << "Enter Name: ";
		getline(cin, organizer1.name);
		if (validateName(organizer1.name) && validateLine(organizer1.name) && !organizer1.name.empty()) {
			break;
		}
		else {
			cout << "Invalid name format and cannot contain '|'. Please Re-enter:" << endl;
		}
	}

	while (true) {
		cout << "Enter Email: ";
		getline(cin, organizer1.email);
		if (validateEmail(organizer1.email) && validateLine(organizer1.email)) {
			Organizer tempOrgan;
			// check if email exists 
			if (loadOrganizerProfile(organizer1.email, tempOrgan)) {
				cout << "Email already exists." << endl;
			}
			else {
				break;
			}
			break;
		}
		else {
			cout << "Invalid email format. Please Re-enter: " << endl;
		}
	}

	while (true) {
		cout << "Enter Password (at least 8 character, include 1 character and 1 number): ";
		getline(cin, organizer1.password);
		if (validatePassword(organizer1.password)) {
			//generate salt and hash the password
			organizer1.salt = generateSalt();
			organizer1.passwordHash = hashPassword(organizer1.password, organizer1.salt);
			break;
		}
		else {
			cout << "Invalid password format. Please Re-enter: " << endl;
		}
	}

	while (true) {
		cout << "Enter Description: ";
		getline(cin, organizer1.description);
		if (validateLine(organizer1.description)) {
			break;
		}
		else {
			cout << "Description cannot contain '|' . please re-enter." << endl;
		}
	}

	saveOrganizerProfile(organizer1);
	cout << "Organizer registered successfully!" << endl;
}

bool organizerLogin(Organizer& loginOrganizer) {
	string email, password;
	Organizer organizer;
	cout << "=== Organizer Login ===" << endl;
	//clear input buffer before taking input
	cin.ignore(numeric_limits<streamsize>::max(), '\n');

	cout << "Enter email: ";
	getline(cin, email);

	if (!loadOrganizerProfile(email, organizer)) {
		cout << "Email not found. Please register first.";
		return false;
	}

	cout << "Enter password: ";
	getline(cin, password);

	//hash the entered password with the stored salt
	string hashedPassword = hashPassword(password, organizer.salt);

	if (hashedPassword == organizer.passwordHash) {
		cout << "Login successful! Welcome, " << organizer.name << endl;
		loginOrganizer = organizer; //copy organizer data to loginOrganizer
		organizerMenu(loginOrganizer); //show organizer menu
		clearScreen();
		return true;
	}
	else {
		cout << "Invalid password. Please try again: " << endl;
		return false;
	}
	return false;
}

// display organizer profile 
void displayOrganizerProfile(const Organizer& organizer) {
	cout << "=== Organizer Profile ===" << endl;
	cout << "Name: " << organizer.name << endl;
	cout << "Email: " << organizer.email << endl;
	cout << "Description: " << organizer.description << endl;
}

// edit organizer profile
void editOrganizerProfile(Organizer& organizer) {
	int choice;
	while (true) {
		cout << "=== Edit Organizer Profile ===" << endl;
		cout << "1. Edit Name" << endl;
		cout << "2. Edit Description" << endl;
		cout << "3. Change Password" << endl;
		cout << "4. Back to Organizer Menu" << endl;
		cout << "Enter your choice: ";

		// Check if input is valid
		if (!(cin >> choice)) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Invalid input. Please enter a number." << endl;
			continue;
		}
		cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clear buffer

		if (validateChoice(choice,1,4)) {
			cin.ignore(numeric_limits<streamsize>::max(), '\n'); //clear input buffer
			switch (choice) {
			case 1:
				cout << "Enter new name: ";
				getline(cin, organizer.name);
				updateOrganizerProfile(organizer);
				cout << "Name updated successfully!" << endl;
				break;
			case 2:
				cout << "Enter new description: ";
				getline(cin, organizer.description);
				updateOrganizerProfile(organizer);
				cout << "Description updated successfully!" << endl;
				break;
			case 3:
			{
				string newPassword;
				while (true) {
					cout << "Enter new password (at least 8 characters, including letters and numbers): ";
					getline(cin, newPassword);
					if (validatePassword(newPassword)) {
						organizer.salt = generateSalt();
						organizer.passwordHash = hashPassword(newPassword, organizer.salt);
						updateOrganizerProfile(organizer);
						cout << "Password updated successfully!" << endl;
						break;
					}
					else {
						cout << "Incorrect password format. Re-enter." << endl;
					}
				}
				break;
			}
			case 4:
				clearScreen();
				return; //back to organizer menu
			default:
				cout << "Invalid choice. Please try again." << endl;
			}
		}
		else {
			cout << "Invalid input. Please enter a number between 1 and 4." << endl;
		}
	}
}

// create event
void createEvent(const Organizer& organizer) {
	//clear input buffer before taking input
	cin.ignore(numeric_limits<streamsize>::max(), '\n');

	cout << "=== Create Event ===" << endl;


}