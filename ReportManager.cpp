#include "ReportUI.h"
#include "ReportManager.h"
#include "validation.h"
#include "function.h"
#include "Transactionh.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <limits>

using namespace std;

// --------------------- Date Input Helper ---------------------
time_t ReportUI::getDateInput(const string& prompt) {
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
        tmStruct.tm_hour = tmStruct.tm_min = tmStruct.tm_sec = 0;

        time_t t = mktime(&tmStruct);
        if (t == -1) {
            cout << "Error parsing date. Try again.\n";
            continue;
        }
        return t;
    }
}

// --------------------- Refund Report Menu ---------------------
void ReportUI::showRefundReportMenu(const vector<Transaction>& transactions) {
    if (transactions.empty()) {
        cout << "No transaction records found.\n";
        return;
    }

    auto summary = generateSummary(transactions);

    while (true) {
        cout << "\n====== Refund Summary Menu ======\n"
            << "1. Show Full Refund Summary\n"
            << "2. Show Top 3 Events by Sales\n"
            << "3. Exit\n"
            << "Choose an option: ";

        string choiceStr;
        getline(cin, choiceStr);

        if (isCancel(choiceStr)) return;
        if (!isDigitsOnly(choiceStr)) {
            cout << "Invalid input. Enter digits only.\n";
            continue;
        }

        int choice = stoi(choiceStr);
        switch (choice) {
        case 1: showRefundSummary(summary); break;
        case 2: showTop3Sales(summary); break;
        case 3: return;
        default: cout << "Invalid option. Try again.\n"; continue;
        }

        cout << "\nPress Enter to return to Refund Summary Menu...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

map<string, Summary> ReportUI::generateSummary(const vector<Transaction>& transactions) {
    map<string, Summary> summary;
    for (auto& t : transactions) {
        auto& s = summary[t.category];
        s.totalCount++;
        s.totalQuantity += t.quantity;
        s.totalAmount += t.amount;

        if (t.status == "refunded") {
            s.refundCount++;
            s.refundQuantity += t.quantity;
            s.refundAmount += t.amount;
        }
    }
    return summary;
}

// --------------------- Refund Summary Display ---------------------
void ReportUI::showRefundSummary(const map<string, Summary>& summary) {
    const int width = 90;
    string title = "Refund Summary Report";
    int padding = (width - title.size()) / 2;

    cout << "\n" << string(width, '=') << "\n";
    cout << string(padding, ' ') << title << "\n";
    cout << string(width, '=') << "\n\n";

    cout << left << setw(20) << "Event Category"
        << right << setw(12) << "Refund Qty"
        << setw(15) << "Refund Amount"
        << setw(12) << "Sold Qty"
        << setw(15) << "Total Amount"
        << setw(12) << "Refund %" << endl;

    cout << string(width, '-') << endl;

    int totalRefundQty = 0, totalSoldQty = 0;
    double totalRefundAmt = 0.0, totalSalesAmt = 0.0;

    for (auto& entry : summary) {
        const auto& cat = entry.first;
        const auto& s = entry.second;
        double refundPerc = s.totalQuantity > 0 ? (double)s.refundQuantity / s.totalQuantity * 100.0 : 0.0;

        cout << left << setw(20) << cat
            << right << setw(12) << s.refundQuantity
            << setw(15) << fixed << setprecision(2) << s.refundAmount
            << setw(12) << s.totalQuantity
            << setw(15) << s.totalAmount
            << setw(11) << fixed << setprecision(2) << refundPerc << endl;

        totalRefundQty += s.refundQuantity;
        totalSoldQty += s.totalQuantity;
        totalRefundAmt += s.refundAmount;
        totalSalesAmt += s.totalAmount;
    }

    cout << string(width, '-') << endl;
    double overallRefundPerc = totalSoldQty > 0 ? (double)totalRefundQty / totalSoldQty * 100.0 : 0.0;
    cout << left << setw(20) << "TOTAL"
        << right << setw(12) << totalRefundQty
        << setw(15) << fixed << setprecision(2) << totalRefundAmt
        << setw(12) << totalSoldQty
        << setw(15) << totalSalesAmt
        << setw(11) << fixed << setprecision(2) << overallRefundPerc << endl;
}

// --------------------- Top 3 Events ---------------------
void ReportUI::showTop3Sales(const std::map<std::string, Summary>& summary) {
    struct EventSales {
        std::string category;
        int totalQty;
        double sales;
    };

    std::vector<EventSales> events;
    for (const auto& entry : summary) {
        events.push_back({ entry.first,
                           entry.second.totalQuantity - entry.second.refundQuantity,
                           entry.second.totalAmount - entry.second.refundAmount });
    }

    std::sort(events.begin(), events.end(), [](const EventSales& a, const EventSales& b) { return a.totalQty > b.totalQty; });

    const int width = 60;
    std::string title = "Top 3 Events by Sales";
    int padding = (width - title.size()) / 2;

    std::cout << "\n" << std::string(width, '=') << "\n";
    std::cout << std::string(padding, ' ') << title << "\n";
    std::cout << std::string(width, '=') << "\n\n";

    std::cout << std::left << std::setw(20) << "Event Category"
        << std::right << std::setw(12) << "Sold Qty"
        << std::setw(15) << "Total Sales" << std::endl;
    std::cout << std::string(width, '-') << std::endl;

    for (int i = 0; i < std::min(3, (int)events.size()); i++) {
        std::cout << std::left << std::setw(20) << events[i].category
            << std::right << std::setw(12) << events[i].totalQty
            << std::setw(15) << std::fixed << std::setprecision(2) << events[i].sales << std::endl;
    }
}

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
