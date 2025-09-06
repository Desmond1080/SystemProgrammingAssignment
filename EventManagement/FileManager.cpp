#include "FileManager.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;
using json = nlohmann::json;

void FileManager::saveToJSON(const string& filename, const vector<Event>& events) {
    json j;

    for (const auto& e : events) {
        json ev;
        ev["name"] = e.name;
        ev["description"] = e.description;
        ev["category"] = static_cast<int>(e.category); // store enum as int
        ev["timestamp"] = e.date;
        ev["location"] = e.location;
        ev["organizer"] = e.organizer;
        ev["ticketPrice"] = e.ticketPrice;

        // save category options
        json options = json::array();
        for (const auto& opt : e.categoryOptions) {
            json optJson;
            optJson["catName"] = opt.first;
            optJson["price"] = opt.second.first;
            optJson["available"] = opt.second.second;
            options.push_back(optJson);
        }
        ev["categoryOptions"] = options;

        j["events"].push_back(ev);
    }

    ofstream outFile(filename);
    if (!outFile) {
        cerr << "Error opening JSON file for writing!" << endl;
        return;
    }
    outFile << j.dump(4); // pretty print with 4-space indent
    cout << "Events saved to " << filename << endl;
}

vector<Event> FileManager::loadFromJSON(const string& filename) {
    ifstream inFile(filename);
    vector<Event> events;

    if (!inFile) {
        cerr << "Error opening JSON file for reading!" << endl;
        return events;
    }

    json j;
    inFile >> j;

    if (!j.contains("events")) {
        cerr << "No events found in JSON file!" << endl;
        return events;
    }

    for (const auto& ev : j["events"]) {
        Event e;
        e.name = ev.value("name", "");
        e.description = ev.value("description", "");
        e.category = static_cast<EventCategory>(ev.value("category", 0));
        e.date = static_cast<time_t>(ev.value("timestamp", 0));
        e.location = ev.value("location", "");
        e.organizer = ev.value("organizer", "");
        e.ticketPrice = ev.value("ticketPrice", 0.0);

        if (ev.contains("categoryOptions")) {
            for (const auto& opt : ev["categoryOptions"]) {
                string catName = opt.value("catName", "");
                double price = opt.value("price", 0.0);
                int available = opt.value("available", 0);
                e.addCategory(catName, price, available);
            }
        }

        events.push_back(e);
    }

    cout << "Events loaded from " << filename << endl;
    return events;
}