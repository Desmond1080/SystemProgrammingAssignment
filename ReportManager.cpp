#include "ReportManager.h"
#include <algorithm>
#include <numeric>
#include <iostream>
#include <iomanip>

using namespace std;

// Filter by organizer
vector<Event> ReportManager::filterByOrganizer(const vector<Event>& events, const string& organizer) {
    vector<Event> result;
    copy_if(events.begin(), events.end(), back_inserter(result),
        [&](const Event& e) { return e.organizer == organizer; });
    return result;
}

// Filter by category
vector<Event> ReportManager::filterByCategory(const vector<Event>& events, EventCategory category) {
    vector<Event> result;
    copy_if(events.begin(), events.end(), back_inserter(result),
        [&](const Event& e) { return e.category == category; });
    return result;
}

// Filter by date range
vector<Event> ReportManager::filterByDateRange(const vector<Event>& events, time_t startDate, time_t endDate) {
    vector<Event> result;
    copy_if(events.begin(), events.end(), back_inserter(result),
        [&](const Event& e) { return e.startDate >= startDate && e.startDate <= endDate; });
    return result;
}

// Filter by minimum total tickets
vector<Event> ReportManager::filterByMinTickets(const vector<Event>& events, int minTickets) {
    vector<Event> result;
    copy_if(events.begin(), events.end(), back_inserter(result),
        [&](const Event& e) {
            int totalTickets = accumulate(e.categoryOptions.begin(), e.categoryOptions.end(), 0,
                [](int sum, const pair<string, pair<double, int>>& cat) { return sum + cat.second.second; });
            return totalTickets >= minTickets;
        });
    return result;
}

// Sort by start date
void ReportManager::sortByStartDate(vector<Event>& events, bool ascending) {
    sort(events.begin(), events.end(),
        [&](const Event& a, const Event& b) {
            return ascending ? (a.startDate < b.startDate) : (a.startDate > b.startDate);
        });
}

// Sort by total tickets
void ReportManager::sortByTotalTickets(vector<Event>& events, bool ascending) {
    sort(events.begin(), events.end(),
        [&](const Event& a, const Event& b) {
            int totalA = accumulate(a.categoryOptions.begin(), a.categoryOptions.end(), 0,
                [](int sum, const pair<string, pair<double, int>>& cat) { return sum + cat.second.second; });
            int totalB = accumulate(b.categoryOptions.begin(), b.categoryOptions.end(), 0,
                [](int sum, const pair<string, pair<double, int>>& cat) { return sum + cat.second.second; });
            return ascending ? (totalA < totalB) : (totalA > totalB);
        });
}

// Display summary table
void ReportManager::displaySummaryTable(const vector<Event>& events) {
    if (events.empty()) {
        cout << "No events to display.\n";
        return;
    }

    const int width_name = 25;
    const int width_cat = 12;
    const int width_org = 15;
    const int width_date = 16;
    const int width_tickets = 10;

    cout << left
        << setw(width_name) << "Event Name"
        << setw(width_cat) << "Category"
        << setw(width_org) << "Organizer"
        << setw(width_date) << "Start Date"
        << setw(width_date) << "End Date"
        << setw(width_tickets) << "Tickets"
        << "\n";

    cout << string(width_name + width_cat + width_org + width_date * 2 + width_tickets, '-') << "\n";

    for (const auto& e : events) {
        int totalTickets = 0;
        for (auto& opt : e.categoryOptions) totalTickets += opt.second.second;

        char startBuf[20], endBuf[20];
        tm tmStart{}, tmEnd{};
#ifdef _WIN32
        localtime_s(&tmStart, &e.startDate);
        localtime_s(&tmEnd, &e.endDate);
#else
        localtime_r(&e.startDate, &tmStart);
        localtime_r(&e.endDate, &tmEnd);
#endif
        strftime(startBuf, sizeof(startBuf), "%Y-%m-%d", &tmStart);
        strftime(endBuf, sizeof(endBuf), "%Y-%m-%d", &tmEnd);

        cout << left
            << setw(width_name) << e.name.substr(0, width_name - 1)
            << setw(width_cat) << Event::categoryToString(e.category)
            << setw(width_org) << e.organizer.substr(0, width_org - 1)
            << setw(width_date) << startBuf
            << setw(width_date) << endBuf
            << setw(width_tickets) << totalTickets
            << "\n";
    }
    cout << "\n";
}
