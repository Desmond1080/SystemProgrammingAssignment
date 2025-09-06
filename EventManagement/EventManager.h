#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include <vector>
#include "Event.h"

using namespace std;

class EventManager {
private:
    vector<Event> events;

public:
    void addEvent(const Event& event);
    vector<Event> getEvents() const;
    void listEvents() const;
    void editEvent(size_t index, const Event& updatedEvent);
    void deleteEvent(size_t index);
    vector<Event> searchEventsByName(const string& name) const;
    vector<Event> searchEventsByCategory(EventCategory category) const;
};

#endif
