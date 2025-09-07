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
	Organizer currentOrganizer;
	bool userLogin = false;
	bool organizerLogin = false;

	while (true) {
		cout << "=== Event Management System ===" << endl;
		cout << "1. User" << endl;
		cout << "2. Organizer" << endl;
		cout << "3. Continue as Guest" << endl;
		cout << "4. Exit" << endl;
		cout << "Enter your choice: ";

		// Check if input is valid
		if (!(cin >> choice)) {
			cin.clear(); // Clear error flags
			cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear buffer
			clearScreen();
			cout << "Invalid input. Please enter a number." << endl;
			continue;
		}

		if (validateChoice(choice, 1, 4)) {
			switch (choice) {
			case 1:
				clearScreen();
				userLogin = userLoginMenu(currentUser);
				if (userLogin) {
					userMenu(currentUser);
				}
				break;
			case 2:
				clearScreen();
				organizerLogin = organizerLoginMenu(currentOrganizer);
				if (organizerLogin) {
					organizerMenu(currentOrganizer);
				}
				break;
			case 3:
				clearScreen();
				break;
			case 4:
				cout << "Exiting the program. Goodbye!" << endl;
				return 0;
			default:
				clearScreen();
				cout << "Invalid choice. Please try again." << endl;
			}
		}
		else {
			clearScreen();
			cout << "Invalid input. Please enter a number between 1 and 4." << endl;
		}
	}
}
