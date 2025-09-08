#include "EventManager.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

using namespace std;

void EventManager::addEvent(const Event& event) {
    if (event.name.empty()) {
        cout << "Error: Event name cannot be empty.\n";
        return;
    }
    events.push_back(event);
}


vector<Event> EventManager::getEvents() const {
    return events;
}

void EventManager::listEvents() const {
    if (events.empty()) {
        cout << "No events available.\n";
        return;
    }

    cout << "\n==================== Event List ====================\n";
    for (size_t i = 0; i < events.size(); i++) {
        events[i].printDetails((int)i);
    }
}


void EventManager::editEvent(size_t index, const Event& updatedEvent) {
    if (index < events.size()) {
        events[index] = updatedEvent;
    }
    else if (index >= events.size()) {
        cout << "Invalid event index.\n";
        return;
    }
    if (updatedEvent.name.empty()) {
        cout << "Error: Cannot update with empty event data.\n";
        return;
    }
    events[index] = updatedEvent;

}

void EventManager::deleteEvent(size_t index) {
    if (index < events.size()) {
        events.erase(events.begin() + index);
    }
    else {
        cout << "Invalid event index." << endl;
    }
}

Event EventManager::searchEventByName(const string& name) const {
    string loweredName = name;
    transform(loweredName.begin(), loweredName.end(), loweredName.begin(), ::tolower);

    for (const auto& event : events) {
        string loweredEventName = event.name;
        transform(loweredEventName.begin(), loweredEventName.end(), loweredEventName.begin(), ::tolower);

        if (loweredEventName.find(loweredName) != string::npos) {
            return event;
        }
    }
    return Event(); // Return default event if not found
}

vector<Event> EventManager::searchEventsByCategory(EventCategory category) const {
    vector<Event> results;
    for (const auto& event : events) {
        if (event.category == category) {
            results.push_back(event);
        }
        else {
            cout << "No events found in this category." << endl;
        }
        return results;
    }
}
