#include "function.h"
#include "participantRegHeader.h"

#include <iostream>

using namespace std;

//guest menu
void guestMenu() {
	// menu for guest 
	int choice;
	string email;
	while (true) {
		cout << "===== Guest Menu =====\n";
		cout << "1. Browse Events\n";
		cout << "2. View Tickets\n";
		cout << "3. Return to Main Menu\n";
		cout << "Enter your choice: " << endl;

		if (!(cin >> choice)) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Invalid input. Please enter a number.\n";
			system("pause");
			continue;
		}

		if (validateChoice(choice, 1, 3)) {
			switch (choice) {
			case 1:
				clearScreen();
				browseEvents(nullptr);
				break;
			case 2:
				clearScreen();
				cout << "Enter your email to view tickets: ";
				cin >> email;
				viewTickets(email);
				break;
			case 3:
				return; // Return to main menu
			default:
				cout << "Invalid choice. Please try again.\n";
				break;
			}
		}
		else {
			cout << "Invalid choice. Please try again.\n";
			system("pause");
		}
	}
}