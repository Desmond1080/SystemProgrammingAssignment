#include "function.h"
#include "structs.h"
#include "Event.h"
#include "EventManager.h"
#include "FileManager.h"
#include "validation.h"
#include "payment.h"

#include <iostream>
#include <limits>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <string>

using namespace std;

//organizer login menu
bool organizerLoginMenu(Organizer& loginOrganizer) {
	int choice;
	Organizer loginOrgan;

	while (true) {
		cout << "=== Organizer Page ===" << endl;
		cout << "1. Login" << endl;
		cout << "2. Register" << endl;
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
				if (organizerLogin(loginOrgan)) {
					loginOrganizer = loginOrgan; //copy logged-in organizer data
					return true; //login successful
				}
				break;
			case 2:
				clearScreen();
				organizerRegister();
				break;
			case 3:
				clearScreen();
				return false; //back to main menu
			default:
				cout << "Invalid choice. Please try again." << endl;
			}
		}
		else {
			cout << endl << endl;
			cout << "Invalid input. Please enter a number between 1 and 3." << endl;
			clearScreen();
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
		cout << "3. Manage Events" << endl;
		cout << "4. Logout" << endl;
		cout << "Enter your choice: ";
		
		// Check if input is valid
		if (!(cin >> choice)) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clean buffer
			cout << "Invalid input. Please enter a number." << endl;
			continue;
		}

		if (validateChoice(choice,1,4)) {
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
				manageEvents(organizer);
				break;
			case 4:
				clearScreen();
				cout << "Logging out..." << endl;
				return;
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
			do {
				organizer1.salt = generateSalt();
			} while (!validateLine(organizer1.salt));
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
	clearScreen();
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
		clearScreen();
		cout << "Email not found. Please register first." << endl;
		//system("pause");
		return false;
	}

	cout << "Enter password: ";
	getline(cin, password);

	//hash the entered password with the stored salt
	string hashedPassword = hashPassword(password, organizer.salt);

	if (hashedPassword == organizer.passwordHash) {
		clearScreen();
		cout << "Login successful! Welcome, " << organizer.name << "!" << endl;
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

		//cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clear buffer

		if (validateChoice(choice,1,4)) {
			cin.ignore(numeric_limits<streamsize>::max(), '\n'); //clear input buffer
			switch (choice) {
			case 1:
				cout << "Enter new name: ";
				getline(cin, organizer.name);
				if (validateLine(organizer.name) && !organizer.name.empty()) {
					updateOrganizerProfile(organizer);
					clearScreen();
					cout << "Name updated successfully!" << endl;
				}
				else {
					cout << "Invalid input. Please try again." << endl;
				}
				break;
			case 2:
				cout << "Enter new description: ";
				getline(cin, organizer.description);
				if (validateLine(organizer.description) && !organizer.name.empty()) {
					updateOrganizerProfile(organizer);
					clearScreen();
					cout << "Description updated successfully!" << endl;
				}
				else {
					cout << "Invalid input. Please try again." << endl;
				}
				break;
			case 3:
			{
				string newPassword;
				while (true) {
					cout << "Enter new password (at least 8 characters, including letters and numbers): ";
					getline(cin, newPassword);
					if (validatePassword(newPassword)) {
						do {
							organizer.salt = generateSalt();
						} while (!validateLine(organizer.salt));
						organizer.passwordHash = hashPassword(newPassword, organizer.salt);
						updateOrganizerProfile(organizer);
						clearScreen();
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

void listEvents(const vector<Event>& events, const string& organizerName = "") {
	if (events.empty()) {
		cout << "No events available.\n";
		return;
	}

	cout << "\n==================== Event List ====================\n";
	for (size_t i = 0; i < events.size(); i++) {
		if (organizerName.empty() || events[i].organizer == organizerName) {
			events[i].printDetails((int)i);
		}
	}
}

Event createEventFromInput(Organizer& loginOrganizer) {
	string name, description, location, organizer = loginOrganizer.name;
	double ticketPrice = 0.0;
	time_t startTime = 0, endTime = 0;

	clearScreen();

	while (true) {
		cout << "Enter Event Name (q to cancel): ";
		getline(cin, name);
		if (isCancel(name)) {
			name = "";
			break;
		}
		if (name.empty()) {
			cout << "Event name cannot be empty!\n";
			continue;
		}
		if (!isValidEventName(name)) {  // New function for event name
			cout << "Event name can only contain letters, numbers, and spaces. Retry.\n";
			continue;
		}
		break;
	}
	cout << "Enter Description (q to cancel): ";
	std::getline(std::cin, description);
	if (isCancel(description)) {
		clearScreen();
		return Event();
	}

	cout << "==================== Categories ====================\n";
	for (int i = 0; i < (int)EventCategory::Count; i++) {
		cout << "  " << i << ". " << Event::categoryToString((EventCategory)i) << "\n";
	}
	cout << "====================================================\n";

	int categoryChoice = -1;
	while (true) {
		categoryChoice = getIntInput("Select Category (q to cancel): ");
		if (categoryChoice == -1) {
			clearScreen();
			return Event();
		}

		// validate bounds
		if (categoryChoice < 0 || categoryChoice >= (int)EventCategory::Count) {
			cout << "Invalid category! Please select a valid number.\n";
			continue;
		}
		break;
	}

	while (true) {
		cout << "Enter Location (q to cancel): ";
		std::getline(std::cin, location);
		if (isCancel(location)) {
			clearScreen();
			return Event();
		}
		if (location.empty()) {
			cout << "Location cannot be empty!\n";
			return Event();
		}
		if (!isValidNameOrLocation(location)) {
			cout << "Location can only contain letters, spaces and commas. Retry.\n";
			continue;
		}
		break;
	}

	//while (true) {
	//	cout << "Enter Organizer (q to cancel): ";
	//	std::getline(std::cin, organizer);
	//	if (isCancel(organizer)) { 
	//		clearScreen();
	//		return Event();
	//	}
	//	if (organizer.empty()) {
	//		cout << "Organizer cannot be empty.\n";
	//		continue;
	//	}
	//	if (!isValidNameOrLocation(organizer)) {
	//		cout << "Organizer can only contain letters and spaces. Retry.\n";
	//		continue;
	//	}
	//	break;
	//}

	//ticketPrice = getDoubleInput("Enter Base Ticket Price (q to cancel): ");
	//if(ticketPrice == -1.0) {
	//	clearScreen();
	//	return Event();
	//}
	//if (ticketPrice < 0.0) {
	//	cout << "Ticket price cannot be negative!\n";
	//	return Event();
	//}

	while (true) {
		string startStr, endStr;
		cout << "Enter event start datetime (YYYY-MM-DD HH:MM) (q to cancel): ";
		getline(cin, startStr);
		if (isCancel(startStr)) return Event();

		tm startTm{};
		istringstream ssStart(startStr);
		ssStart >> get_time(&startTm, "%Y-%m-%d %H:%M");
		if (ssStart.fail()) {
			cout << "Invalid format.\n";
			continue;
		}

		startTime = mktime(&startTm);
		// new date must be after today
		time_t now = time(0);
		if (difftime(startTime, now) <= 0) {
			cout << "Start time must be in the future.\n";
			continue;
		}

		cout << "Enter event end datetime (YYYY-MM-DD HH:MM) (q to cancel): ";
		getline(cin, endStr);
		if (isCancel(endStr)) return Event();

		tm endTm{};
		istringstream ssEnd(endStr);
		ssEnd >> get_time(&endTm, "%Y-%m-%d %H:%M");
		if (ssEnd.fail()) { 
			cout << "Invalid format.\n"; 
			continue;
		}
		endTime = mktime(&endTm);

		if (difftime(endTime, startTime) <= 0) {
			cout << "End time must be after start time.\n";
			continue;
		}
		break;
	}

	Event e(name, description, static_cast<EventCategory>(categoryChoice),
		startTime, endTime, location, organizer, "scheduled", {});

	// Add ticket categories
	char addMore = 'n';
	do {
		string choiceStr;
		cout << "Add a ticket category? (y/n): ";
		getline(cin, choiceStr);
		if (!choiceStr.empty()) {
			addMore = choiceStr[0];
		}

		if (addMore == 'y' || addMore == 'Y') {
			string catName;
			double price;
			int available;

			while (true) {
				cout << "Enter ticket category name (q to cancel): ";
				getline(cin, catName);
				if (isCancel(catName)) {
					clearScreen();
					return Event();
				}
				if (!isValidNameOrLocation(catName)) {
					cout << "Category name cannot contain special characters. Retry.\n";
					continue;
				}
				break; // valid
			}

			price = getDoubleInput("Enter price (q to cancel): ");
			if (price == -1.0) {
				clearScreen();
				return Event();
			}
			while (true) {
				available = getIntInput("Enter available tickets (q to cancel): ");
				if (available == -1) {
					clearScreen();
					return Event();
				}
				if (available < 0) {
					cout << "Ticket availability cannot be negative. Retry.\n";
					continue;
				}
				break; // valid
			}

			e.addCategory(catName, price, available);
		}
		else if (addMore == 'n' || addMore == 'N') {
			if (e.categoryOptions.empty()) {
				cout << "At least one ticket category is required.\n";
			}
			else {
				break; // done adding
			}
		}
		else {
			cout << "Invalid choice. Please enter 'y' or 'n'.\n";
		}
	} while (addMore == 'y' || addMore == 'Y' || e.categoryOptions.empty());

	return e;
}

void manageEvents(Organizer& organizer) {
	cin.ignore(numeric_limits<streamsize>::max(), '\n');

	EventManager manager;
	string filename = "events.json";

	// Load saved events
	auto existing = FileManager::loadFromJSON(filename);
	if (existing.empty()) {
		cout << "No events loaded (file may be missing or empty).\n";
	}
	else {
		for (auto& e : existing) {
			manager.addEvent(e);
		}
	}

	while (true) {
		int choice = -1;
		while (true) {
			cout << "========= Event Manager =========\n"
				<< "1. Add Event\n"
				<< "2. List Events\n"
				<< "3. Edit Event\n"
				<< "4. Cancel Event\n"
				<< "5. Save & Exit\n"
				<< "Choose an option: ";

			string rawInput;
			getline(cin, rawInput);

			try {
				choice = stoi(rawInput);
				if (choice < 1 || choice > 5) {
					cout << "Invalid option. Try again.\n";
					continue;
				}
				break;
			}
			catch (...) {
				cout << "Invalid input. Please enter a number.\n";
			}
		}

		// 1. Add Event
		if (choice == 1) {
			Event e = createEventFromInput(organizer);
			if (e.name.empty()) {
				cout << "Event creation cancelled.\n";
			}
			else {
				manager.addEvent(e);
				clearScreen();
				cout << "Event added!\n";
			}
		}
		// 2. List Events
		else if (choice == 2) {
			manager.listEventsByOrganizer(organizer.name);
			system("pause");
			clearScreen();
		}
		// 3. Edit Event
		else if (choice == 3) {
			vector<Event> orgEvents = manager.getEventsByOrganizer(organizer.name);
			if(orgEvents.empty()) {
				cout << "No events to edit.\n";
				system("pause");
				clearScreen();
				continue;
			}

			cout << "========== Event List ==========\n";
			for (size_t i = 0; i < orgEvents.size(); i++) {
				orgEvents[i].printDetails((int)i);
			}

			int idx;
			while (true) {
				idx = getIntInput("Enter index of event to update (q to cancel): ");

				if (idx == -1) {
					clearScreen();
					cout << "Update cancelled.\n";
					continue;
				}

				if (idx < 0 || idx >= (int)manager.getEvents().size()) {
					cout << "Invalid index.\n";
					continue;
				}

				break;
			}
			if (idx == -1) continue;

			// find actual index in the full events	list 
			Event selectedEvent = orgEvents[idx];
			size_t actualIdx = 0;
			bool found = false;
			vector<Event> allEvents = manager.getEvents();
			for (size_t i = 0; i < allEvents.size(); i++) {
				if (allEvents[i].name == selectedEvent.name &&
					allEvents[i].organizer == selectedEvent.organizer) {
					actualIdx = i;
					found = true;
					break;
				}
			}

			// event not found 
			if (!found) {
				cout << "Error: Event not found.\n";
				continue;
			}

			// Confirm
			if (!getConfirmation("Are you sure you want to update event '" +
				manager.getEvents()[idx].name + "'? (y/n): ")) {
				cout << "Update cancelled.\n";
				continue;
			}

			int updateChoice;
			while (true) {
				updateChoice = getIntInput(
					"\n ===== Update Options ===== \n"
					"1. Postpone Event (Change Date)\n"
					"2. Update Ticket Capacity\n"
					"3. Update Entire Event\n"
					"Choose an option (q to cancel): "
				);

				if (updateChoice == -1) {
					clearScreen();
					cout << "Update cancelled.\n";
					continue;
				}

				if (updateChoice < 1 || updateChoice > 3) {
					cout << "Invalid update option.\n";
					continue;
				}
				break;
			}
			if (updateChoice == -1) continue;

			Event updated = manager.getEvents()[idx];

			if (updateChoice == 1) {
				time_t newStart, newEnd;
				while (true)
				{
					string startStr, endStr;

					cout << "Enter new event start datetime (YYYY-MM-DD HH:MM) (q to cancel): ";
					getline(cin, startStr);
					if (isCancel(startStr)) {
						clearScreen();
						cout << "Update cancelled.\n";
						break;
					}

					tm startTm{};
					istringstream ssStart(startStr);
					ssStart >> get_time(&startTm, "%Y-%m-%d %H:%M");
					if (ssStart.fail()) {
						cout << "Invalid date format! Use YYYY-MM-DD HH:MM.\n";
						continue;
					}

					newStart = mktime(&startTm);

					// new date must be after today
					time_t now = time(0);
					if (difftime(newStart, now) <= 0) {
						cout << "Start time must be in the future.\n";
						continue;
					}
					
					cout << "Enter new event start datetime (YYYY-MM-DD HH:MM) (q to cancel): ";
					getline(cin, endStr);
					if (isCancel(endStr)) {
						clearScreen();
						cout << "Update cancelled.\n";
						break;
					}

					tm endTm{};
					istringstream ssEnd(endStr);
					ssEnd >> get_time(&endTm, "%Y-%m-%d %H:%M");
					if (ssEnd.fail()) { cout << "Invalid format.\n"; continue; }

					newEnd = mktime(&endTm);

					if (difftime(newEnd, newStart) <= 0) {
						cout << "End time must be after start time.\n";
						continue;
					}

					updated.setStartDate(newStart);
					updated.setEndDate(newEnd);
					manager.editEvent(idx, updated);
					clearScreen();
					cout << "Event start and end dates updated successfully!\n";
					break;
				}
			}
			else if (updateChoice == 2) {
				updated.listCategories();

				int catIdx;
				while (true)
				{
					catIdx = getIntInput("Enter ticket category index to update (q to cancel): ");
					if (catIdx == -1) {
						clearScreen();
						cout << "Update cancelled.\n";
						break;
					}

					if (catIdx < 0 || catIdx >= (int)updated.categoryOptions.size()) {
						cout << "Invalid ticket category index.\n";
						continue;
					}
					break;
				}
				if (catIdx == -1) continue;

				while (true) {
					int change = getIntInput("Enter number of tickets to add/deduct (use negative to deduct, q to cancel): ");
					if (change == -1) {
						clearScreen();
						cout << "Update cancelled.\n";
						break;
					}

					int currentCap = updated.categoryOptions[catIdx].second.second;
					int updatedCap = currentCap + change;

					if (updatedCap < 0) {
						cout << "Error: resulting capacity cannot be negative. Retry.\n";
						continue;
					}

					updated.updateCategoryCapacity(catIdx, change);
					manager.editEvent(idx, updated);
					cout << "Ticket capacity updated successfully! New capacity: " << updatedCap << "\n";
					clearScreen();
					break;
				}
			}
			else if (updateChoice == 3) {
				Event fresh = createEventFromInput(organizer);
				if (fresh.name.empty()) {
					cout << "Event update cancelled.\n";
				}
				else {
					manager.editEvent(idx, fresh);
					cout << "Event fully updated!\n";
					// compare to previous name 
					string prevName = manager.getEvents()[idx].name;
					if (prevName != updated.name) {
						// update payment records if name changes
						updateEventNameInPayments(prevName, updated.name);
					}
				}
			}
			else {
				cout << "Invalid update option.\n";
			}
		}
		// 4. Delete Event
		else if (choice == 4) {
			vector<Event> orgEvents = manager.getEventsByOrganizer(organizer.name);

			if (orgEvents.empty()) {
				cout << "No events to delete.\n";
				system("pause");
				clearScreen();
				continue;
			}
			cout << "========== Event List ==========\n";
			for (size_t i = 0; i < orgEvents.size(); i++) {
				orgEvents[i].printDetails((int)i);
			}

			int idx;
			while (true) {
				idx = getIntInput("Enter index of event to cancel (q to cancel): ");
				if (idx == -1) { 
					cout << "Delete cancelled.\n"; 
					break;
				}
				if (idx < 0 || idx >= (int)manager.getEvents().size()) {
					cout << "Invalid index. Try again.\n";
					continue;
				}

				// find actual index in the full events	list
				Event selectedEvent = orgEvents[idx];
				size_t actualIdx = 0;
				bool found = false;
				vector<Event> allEvents = manager.getEvents();
				for (size_t i = 0; i < allEvents.size(); i++) {
					if (allEvents[i].name == selectedEvent.name &&
						allEvents[i].organizer == selectedEvent.organizer) {
						actualIdx = i;
						found = true;
						break;
					}
				}

				// event not found
				if (!found) {
					cout << "Error: Event not found.\n";
					break;
				}

				if (getConfirmation("Are you sure you want to cancel event '" +
					manager.getEvents()[idx].name + "'? (y/n): ")) {
					// change status to cancelled

					Event updated = manager.getEvents()[idx];
					updated.cancelEvent();

					// change payment record status to refund if event is cancelled
					markEventAsRefundedInPayments(manager.getEvents()[idx].name);

					manager.editEvent(idx, updated);
					clearScreen();
					cout << "Event cancelled!\n";
				}else {
						cout << "Cancellation aborted.\n";
					}

				break; // valid
			}
			if (idx == -1) continue;
		}
		// 5. Save & Exit
		else if (choice == 5) {
			clearScreen();
			FileManager::saveToJSON(filename, manager.getEvents());
			cout << "Events saved. Exiting...\n";
			break;
		}
	}
}
