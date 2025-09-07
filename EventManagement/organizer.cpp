#include "function.h"
#include "structs.h"
#include "Event.h"
#include "EventManager.h"
#include "FileManager.h"

#include <iostream>
#include <limits>
#include <sstream>
#include <iomanip>

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
		cout << "3. Manage Events" << endl;
		cout << "4. Add helper to the event" << endl;
		cout << "5. Delete Account" << endl;
		cout << "6. Logout" << endl;
		cout << "Enter your choice: ";
		
		// Check if input is valid
		if (!(cin >> choice)) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clean buffer
			cout << "Invalid input. Please enter a number." << endl;
			continue;
		}

		if (validateChoice(choice,1,6)) {
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
				manageEvents();
				break;
			case 4:
				clearScreen();
				// helper management
				break;
			case 5:
				clearScreen();
				// delete account
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

void listEvents(const vector<Event>& events) {
    if (events.empty()) {
        cout << "No events available.\n";
        return;
    }

    for (size_t i = 0; i < events.size(); i++) {
        const Event& e = events[i];

        // Convert time_t ? readable string
        std::tm tmStruct{};
        localtime_s(&tmStruct, &e.date); // safer version on MSVC
        std::ostringstream dateStream;
        dateStream << std::put_time(&tmStruct, "%Y-%m-%d %H:%M");

        cout << i << ". " << e.name
            << " | " << e.description
            << " | Location: " << e.location
            << " | Organizer: " << e.organizer
            << " | Date: " << dateStream.str()
            << endl;

        // Print ticket categories
        for (const auto& catPair : e.categoryOptions) {
            cout << "   - " << catPair.first //catName
                << " | Price: " << catPair.second.first //catPrice
                << " | Available: " << catPair.second.second << endl; 
        }
    }
}


Event createEventFromInput() {
    string name, description, location, organizer;
    int categoryChoice;
    double ticketPrice;
    time_t date = time(nullptr); // default current time

    cout << "Enter Event Name: ";
    getline(cin, name);

    cout << "Enter Description: ";
    getline(cin, description);

    cout << "Select Category (0=Corporate, 1=TradeShow, 2=Fundraising, "
        "3=Personal, 4=Award, 5=Festival, 6=Wedding, 7=Seminar): ";
    cin >> categoryChoice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter Location: ";
    getline(cin, location);

    cout << "Enter Organizer: ";
    getline(cin, organizer);

    cout << "Enter Base Ticket Price: ";
    cin >> ticketPrice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    Event e(name, description, static_cast<EventCategory>(categoryChoice),
        date, location, organizer, ticketPrice, {});

    // Add ticket categories
    char addMore;
    do {
        string catName;
        double price;
        int available;

        cout << "Enter ticket category name: ";
        getline(cin, catName);

        cout << "Enter price: ";
        cin >> price;
        cout << "Enter available tickets: ";
        cin >> available;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        e.addCategory(catName, price, available);

        cout << "Add another ticket category? (y/n): ";
        cin >> addMore;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    } while (addMore == 'y' || addMore == 'Y');

    return e;
}

void manageEvents() {
    EventManager manager;
    string filename = "events.json";

    // Load existing events if file exists
    auto existing = FileManager::loadFromJSON(filename);
    for (auto& e : existing) {
        manager.addEvent(e);
    }

    int choice;
    do {
        cout << "\n===== Event Management Menu =====\n";
        cout << "1. Create Event\n";
        cout << "2. List Events\n";
        cout << "3. Update Event\n";
        cout << "4. Delete Event\n";
        cout << "5. Save & Exit\n";
        cout << "Choose an option: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
        case 1: {
            Event e = createEventFromInput();
            manager.addEvent(e);
            cout << "Event created!\n";
            break;
        }
        case 2: {
            listEvents(manager.getEvents());
            break;
        }
        case 3: {
            manager.listEvents();
            cout << "Enter index of event to update (0-based): ";
            size_t idx;
            cin >> idx;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (idx < manager.getEvents().size()) {
                int updateChoice;
                cout << "\nUpdate Options:\n";
                cout << "1. Postpone Event (Change Date)\n";
                cout << "2. Update Ticket Capacity\n";
                cout << "3. Update Entire Event\n";
                cout << "Choose an option: ";
                cin >> updateChoice;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                // Make a copy so we can modify
                Event updated = manager.getEvents()[idx];

                if (updateChoice == 1) {
                    int days;
                    cout << "Enter number of days to postpone: ";
                    cin >> days;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    time_t newDate = updated.getDate() + (days * 24 * 60 * 60);
                    updated.setDate(newDate);

                    manager.editEvent(idx, updated);
                    cout << "Event postponed by " << days << " days!\n";
                }
                else if (updateChoice == 2) {
                    updated.listCategories();
                    cout << "Enter ticket category index to update: ";
                    size_t catIdx;
                    cin >> catIdx;
                    cout << "Enter new ticket capacity: ";
                    int newCap;
                    cin >> newCap;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    updated.updateCategoryCapacity(catIdx, newCap);
                    manager.editEvent(idx, updated);
                    cout << "Ticket capacity updated!\n";
                }
                else if (updateChoice == 3) {
                    Event fresh = createEventFromInput();
                    manager.editEvent(idx, fresh);
                    cout << "Event fully updated!\n";
                }
                else {
                    cout << "Invalid update option.\n";
                }
            }
            else {
                cout << "Invalid index.\n";
            }
            break;
        }
        case 4: {
            listEvents(manager.getEvents());
            cout << "Enter index of event to delete (0-based): ";
            size_t idx;
            cin >> idx;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            manager.deleteEvent(idx);
            cout << "Event deleted!\n";
            break;
        }
        case 5: {
            FileManager::saveToJSON(filename, manager.getEvents());
            cout << "Events saved. Exiting...\n";
            break;
        }
        default:
            cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 5);
}