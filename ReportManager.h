#ifndef REPORT_MANAGER_H
#define REPORT_MANAGER_H

#include <vector>
#include <string>
#include <ctime>
#include "Event.h"

class ReportManager {
public:
    // Filter by organizer (exact match)
    static std::vector<Event> filterByOrganizer(const std::vector<Event>& events, const std::string& organizer);

    // Filter by category
    static std::vector<Event> filterByCategory(const std::vector<Event>& events, EventCategory category);

    // Filter by date range (events that start between startDate and endDate)
    static std::vector<Event> filterByDateRange(const std::vector<Event>& events, std::time_t startDate, std::time_t endDate);

    // Filter by minimum total tickets available
    static std::vector<Event> filterByMinTickets(const std::vector<Event>& events, int minTickets);

    // Sort events by start date
    static void sortByStartDate(std::vector<Event>& events, bool ascending = true);

    // Sort events by total tickets available
    static void sortByTotalTickets(std::vector<Event>& events, bool ascending = true);

    // Display events in report form
    static void displaySummaryTable(const std::vector<Event>& events);
};

#endif

