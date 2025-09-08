#ifndef REPORT_MANAGER_H
#define REPORT_MANAGER_H

#include <vector>
#include <string>
#include <ctime>
#include "Event.h"

class ReportManager {
public:
    // Filter events
    static std::vector<Event> filterByOrganizer(const std::vector<Event>& events, const std::string& organizer);
    static std::vector<Event> filterByCategory(const std::vector<Event>& events, EventCategory category);
    static std::vector<Event> filterByDateRange(const std::vector<Event>& events, std::time_t startDate, std::time_t endDate);
    static std::vector<Event> filterByMinTickets(const std::vector<Event>& events, int minTickets);

    // Sort events
    static void sortByStartDate(std::vector<Event>& events, bool ascending = true);
    static void sortByTotalTickets(std::vector<Event>& events, bool ascending = true);

    // Display events
    static void displaySummaryTable(const std::vector<Event>& events);

};

#endif
