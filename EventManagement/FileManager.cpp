#include "FileManager.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;
using json = nlohmann::json;

void FileManager::saveToJSON(const string& filename, const vector<Event>& events) {
    try {
        json j;
        j["events"] = json::array();  // initialize explicitly

        for (const auto& e : events) {
            json ev;
            ev["name"] = e.name;
            ev["description"] = e.description;
            ev["category"] = static_cast<int>(e.category);
            ev["startDate"] = static_cast<int64_t>(e.getStartDate());
            ev["endDate"] = static_cast<int64_t>(e.getEndDate());
            ev["location"] = e.location;
            ev["organizer"] = e.organizer;
            ev["ticketPrice"] = e.ticketPrice;

            // save category options
            json options = json::array();
            for (const auto& opt : e.categoryOptions) {
                if (opt.first.empty()) continue; // skip invalid category
                options.push_back({
                    {"catName", opt.first},
                    {"price", opt.second.first},
                    {"available", opt.second.second}
                    });
            }
            ev["categoryOptions"] = options;

            j["events"].push_back(ev);
        }

        ofstream outFile(filename);
        if (!outFile) {
            cerr << "Error: Could not open JSON file '" << filename << "' for writing.\n";
            return;
        }
        outFile << j.dump(4);
        if (!outFile.good()) {
            cerr << "Error: Failed while writing JSON file.\n";
        }
        else {
            cout << "Events saved to " << filename << endl;
        }
    }
    catch (const exception& ex) {
        cerr << "Exception while saving JSON: " << ex.what() << endl;
    }
}


vector<Event> FileManager::loadFromJSON(const string& filename) {
    vector<Event> events;
    ifstream inFile(filename);
    if (!inFile) {
        cerr << "Warning: Could not open file '" << filename << "' for reading.\n";
        return events;
    }

    try {
        json j;
        inFile >> j;

        if (!j.contains("events") || !j["events"].is_array()) {
            cerr << "Warning: No valid 'events' array found in JSON.\n";
            return events;
        }

        for (const auto& ev : j["events"]) {
            if (!ev.is_object()) continue;

            Event e;
            e.name = ev.value("name", "");
            e.description = ev.value("description", "");
            int catInt = ev.value("category", 0);
            if (catInt < 0 || catInt > static_cast<int>(EventCategory::Competition)) {
                cerr << "Invalid category in file, defaulting to 0.\n";
                catInt = 0;
            }
            e.category = static_cast<EventCategory>(catInt);

            e.setStartDate(ev.value("startDate", 0));
            e.setEndDate(ev.value("endDate", 0));
            e.location = ev.value("location", "");
            e.organizer = ev.value("organizer", "");
            e.ticketPrice = ev.value("ticketPrice", 0.0);

            if (ev.contains("categoryOptions") && ev["categoryOptions"].is_array()) {
                for (const auto& opt : ev["categoryOptions"]) {
                    string catName = opt.value("catName", "");
                    double price = opt.value("price", 0.0);
                    int available = opt.value("available", 0);
                    if (!catName.empty()) {
                        e.addCategory(catName, price, available);
                    }
                }
            }

            events.push_back(e);
        }

        cout << "Loaded " << events.size() << " events from " << filename << endl;
    }
    catch (const json::parse_error& ex) {
        cerr << "JSON parse error in file '" << filename << "': " << ex.what() << endl;
    }
    catch (const exception& ex) {
        cerr << "Exception while reading JSON: " << ex.what() << endl;
    }

    return events;
}

