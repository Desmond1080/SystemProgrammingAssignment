#include "EventManager.h"
#include <iostream>
#include <iomanip>

using namespace std;

void EventManager::addEvent(const Event& event) {
    events.push_back(event);
}

vector<Event> EventManager::getEvents() const {
    return events;
}

void EventManager::listEvents() const {
    for (size_t i = 0; i < events.size(); i++) {
        const auto& event = events[i];
        char dateBuffer[80];
        struct tm readable;
        localtime_s(&readable, &event.date);
        strftime(dateBuffer, sizeof(dateBuffer), "%Y-%m-%d", &readable);

        cout << i << ". Event: " << event.name
            << ", Date: " << dateBuffer
            << ", Location: " << event.location << endl;
    }
}

void EventManager::editEvent(size_t index, const Event& updatedEvent) {
    if (index < events.size()) {
        events[index] = updatedEvent;
    }
    else {
        cout << "Invalid event index." << endl;
    }
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
    for (const auto& event : events) {
        if (event.name == name) {
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
    }
    return results;
}
