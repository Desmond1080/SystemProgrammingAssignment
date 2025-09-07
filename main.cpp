#include <iostream>
#include <limits>
#include <sstream>
#include <cstdlib>
#include <string>
#include "Event.h"
#include "EventManager.h"
#include "FileManager.h"
#include "validation.h"
#include "function.h"
#include <iomanip>

using namespace std;

void listEvents(const vector<Event>& events) {
	if (events.empty()) {
		cout << "No events available.\n";
		return;
	}

	cout << "\n-------------------- Event List --------------------\n";
	for (size_t i = 0; i < events.size(); i++) {
		events[i].printDetails((int)i);
	}
}


Event createEventFromInput() {
	string name, description, location, organizer;
	double ticketPrice = 0.0;
	time_t startTime = 0, endTime = 0;

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

	cout << "-------------------- Categories --------------------n";
	for (int i = 0; i < (int)EventCategory::Count; i++) {
		cout << "  " << i << ". " << Event::categoryToString((EventCategory)i) << "\n";
	}
	cout << "----------------------------------------------------\n";

	int categoryChoice = -1;
	while (true) {
		categoryChoice = getIntInput("Select Category (q to cancel): ");
		if (categoryChoice == -1) {
			clearScreen();
			return Event();
		}

		// validate bounds
		if (categoryChoice < 0 || categoryChoice >= (int)EventCategory::Count) {
			cout << "Invalid category!Please select a valid number.\n";
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

	while (true) {
		cout << "Enter Organizer (q to cancel): ";
		std::getline(std::cin, organizer);
		if (isCancel(organizer)) { 
			clearScreen();
			return Event();
		}
		if (organizer.empty()) {
			cout << "Organizer cannot be empty.\n";
			continue;
		}
		if (!isValidNameOrLocation(organizer)) {
			cout << "Organizer can only contain letters and spaces. Retry.\n";
			continue;
		}
		break;
	}

	ticketPrice = getDoubleInput("Enter Base Ticket Price (q to cancel): ");
	if(ticketPrice == -1.0) {
		clearScreen();
		return Event();
	}
	if (ticketPrice < 0.0) {
		cout << "Ticket price cannot be negative!\n";
		return Event();
	}

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
		startTime, endTime, location, organizer, ticketPrice, {});

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
	} while (addMore == 'y' || addMore == 'Y');

	return e;
}

int main() {
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
			cout << "\n---------- Event Manager ----------\n"
				<< "1. Add Event\n"
				<< "2. List Events\n"
				<< "3. Search Events\n"
				<< "4. Edit Event\n"
				<< "5. Delete Event\n"
				<< "6. Save & Exit\n"
				<< "Choose an option: ";

			string rawInput;
			getline(cin, rawInput);

			try {
				choice = stoi(rawInput);
				if (choice < 1 || choice > 6) {
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
			Event e = createEventFromInput();
			if (e.name.empty()) {
				cout << "Event creation cancelled.\n";
			}
			else {
				manager.addEvent(e);
				cout << "Event added!\n";
			}

		}
		// 2. List Events
		else if (choice == 2) {
			manager.listEvents();
		}
		// 3. Search Events
		else if (choice == 3) {
			int searchChoice = -1;
			while (true) {
				searchChoice = getIntInput(
					"\n ===== Search Options ===== \n"
					"1. Search by Name\n"
					"2. Search by Category\n"
					"Choose an option (q to cancel): "
				);
				if (searchChoice == -1) {
					clearScreen();
					cout << "Search cancelled.\n";
					break;
				}
				if (searchChoice < 1 || searchChoice > 2) {
					cout << "Invalid search option.\n";
					continue;
				}
				break;
			}
			if (searchChoice == -1) continue;

			vector<Event> results;
			if (searchChoice == 1) {
				string keyword;
				cout << "Enter event name to search (q to cancel): ";
				getline(cin, keyword);
				if (isCancel(keyword)) {
					clearScreen();
					cout << "Search cancelled.\n";
					continue;
				}
				auto results = manager.searchEventsByName(keyword);
				if (results.empty()) {
					cout << "No events found matching '" << keyword << "'.\n";
				}
				else {
					cout << results.size() << " event(s) found:\n";
					listEvents(results);
				}
			}
			else if (searchChoice == 2) {
				cout << "-------------------- Categories --------------------\n";
				for (int i = 0; i < (int)EventCategory::Count; i++) {
					cout << "  " << i << ". " << Event::categoryToString((EventCategory)i) << "\n";
				}
				cout << "-----------------------------------------------------\n";
				int categoryChoice = -1;
				while (true) {
					categoryChoice = getIntInput("Select Category to search (q to cancel): ");
					if (categoryChoice == -1) {
						clearScreen();
						cout << "Search cancelled.\n";
						break;
					}
					// validate bounds
					if (categoryChoice < 0 || categoryChoice >= (int)EventCategory::Count) {
						cout << "Invalid category!Please select a valid number.\n";
						continue;
					}
					break;
				}
				// Display results
				if (results.empty()) {
					cout << "No events found.\n";
				}
				else {
					cout << "\n----- Search Results -----\n";
					for (size_t i = 0; i < results.size(); i++) {
						results[i].printDetails((int)i);
					}
				}
			}
		}
		// 4. Edit Event
		else if (choice == 3) {
			listEvents(manager.getEvents());

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

			// Confirm
			if (!getConfirmation("Are you sure you want to update event '" +
				manager.getEvents()[idx].name + "'? (y/n): ")) {
				cout << "Update cancelled.\n";
				continue;
			}

			int updateChoice;
			while (true) {
				updateChoice = getIntInput(
					"\n ----- Update Options ----- \n"
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
					break;
				}
			}
			else if (updateChoice == 3) {
				Event fresh = createEventFromInput();
				if (fresh.name.empty()) {
					cout << "Event update cancelled.\n";
				}
				else {
					manager.editEvent(idx, fresh);
					cout << "Event fully updated!\n";
				}
			}
			else {
				cout << "Invalid update option.\n";
			}
		}
		// 5. Delete Event
		else if (choice == 5) {
			listEvents(manager.getEvents());
			int idx;
			while (true) {
				idx = getIntInput("Enter index of event to delete (q to cancel): ");
				if (idx == -1) { 
					cout << "Delete cancelled.\n"; 
					break;
				}
				if (idx < 0 || idx >= (int)manager.getEvents().size()) {
					cout << "Invalid index. Try again.\n";
					continue;
				}

				if (getConfirmation("Are you sure you want to delete event '" +
					manager.getEvents()[idx].name + "'? (y/n): ")) {
					manager.deleteEvent(idx);
					cout << "Event deleted!\n";
				}else {
						cout << "Delete cancelled.\n";
					}

				break; // valid
			}
			if (idx == -1) continue;
		}
		// 6. Save & Exit
		else if (choice == 6) {
			FileManager::saveToJSON(filename, manager.getEvents());
			cout << "Events saved. Exiting...\n";
			break;
		}
	}
	return 0;
}
