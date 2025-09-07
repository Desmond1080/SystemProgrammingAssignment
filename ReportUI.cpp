#include "ReportUI.h"
#include "ReportManager.h"
#include "validation.h"
#include "function.h"

#include <iostream>
#include <string>
#include <ctime>
#include <sstream>
#include <limits>
#include <iomanip>

using namespace std;

// Helper: Get time_t from user input (YYYY-MM-DD)
time_t getDateInput(const string& prompt) {
    while (true) {
        string input;
        cout << prompt;
        getline(cin, input);

        if (isCancel(input)) return -1;

        if (!isValidDate(input)) {
            cout << "Invalid date format. Use YYYY-MM-DD.\n";
            continue;
        }

        tm tmStruct{};
        istringstream ss(input);
        ss >> get_time(&tmStruct, "%Y-%m-%d");
        tmStruct.tm_hour = 0;
        tmStruct.tm_min = 0;
        tmStruct.tm_sec = 0;

        time_t t = mktime(&tmStruct);
        if (t == -1) {
            cout << "Error parsing date. Try again.\n";
            continue;
        }

        return t;
    }
}

// Show Custom Report Menu
void ReportUI::showReportMenu(const vector<Event>& events) {
    if (events.empty()) {
        cout << "No events available.\n";
        return;
    }

    vector<Event> filteredEvents = events;

    while (true) {
        cout << "\n====== Custom Report Menu ======\n"
            << "1. Filter by Organizer\n"
            << "2. Filter by Category\n"
            << "3. Filter by Date Range\n"
            << "4. Filter by Minimum Tickets Available\n"
            << "5. Sort by Start Date\n"
            << "6. Sort by Total Tickets\n"
            << "7. Display Report\n"
            << "8. Reset Filters\n"
            << "9. Exit Report Menu\n"
            << "Choose an option: ";

        string choiceStr;
        getline(cin, choiceStr);

        if (!isDigitsOnly(choiceStr)) {
            cout << "Invalid input. Enter digits only.\n";
            continue;
        }

        int choice = stoi(choiceStr);

        switch (choice) {
        case 1: { // Filter by Organizer
            clearScreen();
            string organizer;
            cout << "Enter organizer name (letters, spaces, commas only, q to cancel): ";
            getline(cin, organizer);

            if (!isCancel(organizer)) {
                if (!isValidNameOrLocation(organizer)) {
                    cout << "Invalid organizer name.\n";
                }
                else {
                    filteredEvents = ReportManager::filterByOrganizer(filteredEvents, organizer);
                    cout << "Filtered " << filteredEvents.size() << " events.\n";
                }
            }
            break;
        }

        case 2: { // Filter by Category
            clearScreen();
            cout << "-------------------- Categories --------------------\n";
            for (int i = 0; i < (int)EventCategory::Count; i++) {
                cout << "  " << i << ". " << Event::categoryToString((EventCategory)i) << "\n";
            }
            cout << "----------------------------------------------------\n";

            int catChoice = getIntInput("Enter category index (0-" + to_string((int)EventCategory::Count - 1) + ", q to cancel): ", true);
            if (catChoice != -1) {
                if (catChoice >= 0 && catChoice < (int)EventCategory::Count) {
                    filteredEvents = ReportManager::filterByCategory(filteredEvents, static_cast<EventCategory>(catChoice));
                    cout << "Filtered " << filteredEvents.size() << " events.\n";
                }
                else {
                    cout << "Invalid category index.\n";
                }
            }
            break;
        }

        case 3: { // Filter by Date Range
            clearScreen();
            time_t start = getDateInput("Enter start date (YYYY-MM-DD, q to cancel): ");
            if (start == -1) break;

            time_t end = getDateInput("Enter end date (YYYY-MM-DD, q to cancel): ");
            if (end == -1) break;

            if (difftime(end, start) < 0) {
                cout << "End date cannot be earlier than start date.\n";
                break;
            }

            filteredEvents = ReportManager::filterByDateRange(filteredEvents, start, end);
            cout << "Filtered " << filteredEvents.size() << " events.\n";
            break;
        }

        case 4: { // Filter by Minimum Tickets
            clearScreen();
            int minTickets = getIntInput("Enter minimum total tickets (q to cancel): ", true);
            if (minTickets != -1) {
                filteredEvents = ReportManager::filterByMinTickets(filteredEvents, minTickets);
                cout << "Filtered " << filteredEvents.size() << " events.\n";
            }
            break;
        }

        case 5: { // Sort by Start Date
            clearScreen();
            int order = getIntInput("Sort by Start Date: 1 = Ascending, 2 = Descending (q to cancel): ", true);
            if (order == 1 || order == 2) {
                ReportManager::sortByStartDate(filteredEvents, order == 1);
                cout << "Sorted by start date.\n";
            }
            else if (order != -1) {
                cout << "Invalid choice.\n";
            }
            break;
        }

        case 6: { // Sort by Total Tickets
            clearScreen();
            int order = getIntInput("Sort by Total Tickets: 1 = Ascending, 2 = Descending (q to cancel): ", true);
            if (order == 1 || order == 2) {
                ReportManager::sortByTotalTickets(filteredEvents, order == 1);
                cout << "Sorted by total tickets.\n";
            }
            else if (order != -1) {
                cout << "Invalid choice.\n";
            }
            break;
        }

        case 7: { // Display Report
            clearScreen();
            if (filteredEvents.empty()) {
                cout << "No events to display.\n";
            }
            else {
                ReportManager::displaySummaryTable(filteredEvents); // concise table
            }
            break;
        }

        case 8: { // Reset filters
            filteredEvents = events;
            cout << "Filters reset. Showing all events.\n";
            break;
        }

        case 9: // Exit
            cout << "Exiting report menu...\n";
            return;

        default:
            cout << "Invalid option. Try again.\n";
        }
    }
}
