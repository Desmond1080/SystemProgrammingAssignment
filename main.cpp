#include <iostream>
#include <limits>
#include <sstream>
#include "Event.h"
#include "EventManager.h"
#include "FileManager.h"
#include <iomanip>

using namespace std;

void listEvents(const vector<Event>& events) {
    if (events.empty()) {
        cout << "No events available.\n";
        return;
    }

    for (size_t i = 0; i < events.size(); i++) {
        const Event& e = events[i];

        // Convert time_t → readable string
        std::tm tmStruct{};
        localtime_s(&tmStruct, &e.timestamp); // safer version on MSVC
        std::ostringstream dateStream;
        dateStream << std::put_time(&tmStruct, "%Y-%m-%d %H:%M");

        cout << i << ". " << e.name
            << " | " << e.description
            << " | Location: " << e.location
            << " | Organizer: " << e.organizer
            << " | Date: " << dateStream.str()
            << endl;

        // Print ticket categories
        for (const auto& [catName, catInfo] : e.categoryOptions) {
            cout << "   - " << catName
                << " | Price: " << catInfo.first
                << " | Available: " << catInfo.second << endl;
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

int main() {
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

    return 0;
}
