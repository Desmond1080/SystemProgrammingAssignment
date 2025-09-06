#include <iostream>
#include <string>
#include <limits>
#include <sstream>
#include "Event.h"
#include "EventManager.h"
#include "FileManager.h"
#include <iomanip>

#include "function.h"
#include "participantRegHeader.h"

using namespace std;

int main() {
	int choice;
	User currentUser;
	bool login = false;

	do {
		clearScreen();
		cout << "=== Event Management System ===" << endl;
		cout << "1. Login to Existing Account" << endl;
		cout << "2. Register New Account" << endl;
		cout << "3. Continue as Guest" << endl;
		cout << "4. Forgot Password" << endl;
		cout << "5. Exit" << endl;
		cout << "Enter your choice: ";
		cin >> choice;
		
		switch (choice) {
		case 1:
			// after login 
			login = userLogin(currentUser);
			if (login) {
				int userchoice;
				do {
					cout << "=== User Page ===" << endl;
					cout << "1. Browse Events" << endl;
					cout << "2. View Tickets" << endl;
					cout << "3. View Profile" << endl;
					cout << "4. Edit Profile" << endl;
					cout << "5. Delete Account" << endl; // under edit profile?
					cout << "6. Logout" << endl;
					cout << "Enter your choice: ";
					cin >> userchoice;

					switch (userchoice) {
					case 1: // Browse Events
						browseEvents(currentUser);
						break;
					case 2: // View Tickets
						break;
					case 3: // View Profile
						clearScreen();
						displayUserProfile(currentUser);
						break;
					case 4: // Edit Profile
						editUserProfile(currentUser);
						break;
					case 5: // Delete Account
						deleteUserAccount(currentUser);
						login = false;
						break;
					case 6: // Logout
						cout << "Logging out..." << endl;
						login = false;
						break;
					default:
						cout << "Invalid choice. Please try again." << endl;
						break;
					}
				} while (userchoice != 6);
			}
			break;
		case 2:
			userRegister();
			break;
		case 3:
			continueAsGuest();
			break;
		case 4:
			forgotPassword(currentUser.email);
			break;
		case 5:
			cout << "Exiting the program." << endl;
			break;
		}	

	} while (choice != 5);

	return 0;
}
