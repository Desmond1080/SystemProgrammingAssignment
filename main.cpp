#include "Event.h"
#include "FileManager.h"
#include <vector>
#include <iostream>

using namespace std;

int main() {
    EventManager manager;
    
    // Create sample event
    Event e1("Music Festival", "Outdoor concert", EventCategory::Festival, time(nullptr),
             "Central Park", "ABC Corp", 50.0, {});
    e1.addCategory("VIP", 120.0, 100);
    e1.addCategory("Standard", 50.0, 500);

    manager.addEvent(e1);

    // Save events
    FileManager::saveToJSON("events.json", manager.getEvents());

    // Load events
    vector<Event> loaded = FileManager::loadFromJSON("events.json");

    cout << "Loaded " << loaded.size() << " events from file." << endl;

    return 0;
}

