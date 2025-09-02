#include <iostream>
#include <string>
#include "function.h"

using namespace std;

int main() {
	int choice;
	User currentUser;
	bool login = false;

	do {
		cout << "=== Event Management System ===" << endl;
		cout << "1. Register New Account" << endl;
		cout << "2. Login to Existing Account" << endl;
		cout << "3. Continue as Guest" << endl;
		cout << "4. Forgot Password" << endl;
		cout << "5. Exit" << endl;
		cout << "Enter your choice: ";
		cin >> choice;
		
		switch (choice) {
		case 1:
			userRegister();
			break;
		case 2:
			// after login 
			login = userLogin(currentUser);
			if (login) {
				int userchoice;
				do {
					cout << "=== User Page ===" << endl;
					cout << "1. View Profile" << endl;
					cout << "2. Edit Profile" << endl;
					cout << "3. View Tickets" << endl;
					cout << "4. Delete Account" << endl;
					cout << "5. Logout" << endl;
					cout << "Enter your choice:";
					cin >> userchoice;

					switch (userchoice) {
					case 1:
						clearScreen();
						displayUserProfile(currentUser);
						break;
					case 2:
						editUserProfile(currentUser);
						break;
					case 3:
						break;
					case 4:
						deleteUserAccount(currentUser);
						login = false;
						break;
					case 5:
						cout << "Logging out..." << endl;
						login = false;
						break;
					}
				} while (userchoice != 4);
			}
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