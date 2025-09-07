    #ifndef EVENTMANAGER_H
    #define EVENTMANAGER_H

    #include <vector>
    #include "Event.h"

    class EventManager {
    private:
        std::vector<Event> events;

    public:
        void addEvent(const Event& event);
        std::vector<Event> getEvents() const;
        void listEvents() const;
        void editEvent(std::size_t index, const Event& updatedEvent);
        void deleteEvent(std::size_t index);
        std::vector<Event> searchEventsByName(const std::string& name) const;
        std::vector<Event> searchEventsByCategory(EventCategory category) const;
    };

    #endif
