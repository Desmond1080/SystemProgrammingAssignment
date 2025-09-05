#ifndef EVENT_H
#define EVENT_H

#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <ctime>

using namespace std;

enum class EventCategory {
    Corporate,
    TradeShow,
    Fundraising,
    Personal,
    Award,
    Festival,
    Wedding,
    Seminar
};

// Convert enum to string
string categoryToString(EventCategory category);

struct Event {
    string name;
    string description;
    EventCategory category;
    time_t timestamp{};    // convert to string later
    string location;
    string organizer;
    double ticketPrice{};

    // Ticket categories: { "VIP", {price, available} }
    vector<pair<string,pair<double, int>>> categoryOptions;

    Event(const string& name = "",
          const string& description = "",
          EventCategory category = EventCategory::Corporate,
          time_t timestamp = 0,
          const string& location = "",
          const string& organizer = "",
          double ticketPrice = 0.0,
          const vector<pair<string, pair<double, int>>>& categoryOptions = {});

    void addCategory(const string& catName, double price, int available);
    void printDetails() const;
};

class EventManager {
private:
    vector<Event> events;

public:
    void addEvent(const Event& event);
    void listEvents() const;
    vector<Event> getEvents() const;
    void editEvent(size_t index, const Event& updatedEvent);
    void deleteEvent(size_t index);
    vector<Event> searchEventsByName(const std::string& name) const;
    vector<Event> searchEventsByCategory(EventCategory category) const;
};

#endif
