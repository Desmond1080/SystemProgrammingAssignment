#include "Event.h"

using namespace std;

// Convert enum to string
string categoryToString(EventCategory category) {
    switch (category) {
        case EventCategory::Corporate: return "Corporate";
        case EventCategory::TradeShow: return "Trade Show";
        case EventCategory::Fundraising: return "Fundraising";
        case EventCategory::Personal: return "Personal";
        case EventCategory::Award: return "Award";
        case EventCategory::Festival: return "Festival";
        case EventCategory::Wedding: return "Wedding";
        case EventCategory::Seminar: return "Seminar";
        default: return "Unknown";
    }
}

// ====== Event Implementation ======
Event::Event(const string& name,
             const string& description,
             EventCategory category,
             time_t date,
             const string& location,
             const string& organizer,
             double ticketPrice,
             const vector<pair<string, pair<double, int>>>& categoryOptions)
    : name(name), description(description), category(category), timestamp(date),
      location(location), organizer(organizer), ticketPrice(ticketPrice),
      categoryOptions(categoryOptions) {}

void Event::addCategory(const string& catName, double price, int available) {
    categoryOptions.push_back({catName, {price, available}});
}

void Event::printDetails() const {
    cout << "Event: " << name << endl;
    cout << "Description: " << description << endl;
    cout << "Category: " << categoryToString(category) << endl;

    // convert time_t into readable date
    time_t eventDate = time(nullptr);  // store as time_t
    struct tm readable;
    localtime_s(&readable, &eventDate);  // convert to broken-down time
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", &readable); 
    cout << "Date: " << buffer << endl;
    
    cout << "Location: " << location << endl;
    cout << "Organizer: " << organizer << endl;
    cout << "Base Ticket Price: $" << ticketPrice << endl;

    cout << "\nTicket Categories:\n";
    for (const auto& option : categoryOptions) {
        cout << " - " << option.first
             << " | Price: $" << option.second.first
             << " | Available: " << option.second.second << endl;
    }
    cout << "------------------------------------\n";
}

// ====== EventManager Implementation ======
void EventManager::addEvent(const Event& event) {
    events.push_back(event);
}

vector<Event> EventManager::getEvents() const {
    return events;
}

void EventManager::listEvents() const {
    for (const auto& event : events) {
        // Format the event timestamp as a readable date string
        char dateBuffer[80];
        struct tm readable;
        localtime_s(&readable, &event.timestamp);
        strftime(dateBuffer, sizeof(dateBuffer), "%Y-%m-%d", &readable);
        cout << "Event: " << event.name
             << ", Date: " << dateBuffer
             << ", Location: " << event.location << endl;
    }
}

void EventManager::editEvent(size_t index, const Event& updatedEvent) {
    if (index < events.size()) {
        events[index] = updatedEvent;
    } else {
        cout << "Invalid event index." << endl;
    }
}

void EventManager::deleteEvent(size_t index) {
    if (index < events.size()) {
        events.erase(events.begin() + index);
    } else {
        cout << "Invalid event index." << endl;
    }
}

vector<Event> EventManager::searchEventsByName(const string& name) const {
    vector<Event> results;
    for (const auto& event : events) {
        if (event.name == name) {
            results.push_back(event);
        }
    }
    return results;
}

vector<Event> EventManager::searchEventsByCategory(EventCategory category) const {
    vector<Event> results;
    for (const auto& event : events) {
        if (event.category == category) {
            results.push_back(event);
        }
    }
    return results;
}
