#include "Event.h"
#include <iomanip>
#include <iostream>
#include <ctime>

using namespace std;

// Convert enum to string
string Event::categoryToString(EventCategory category) {
    switch (category) {
    case EventCategory::Concert: return "Concert";
    case EventCategory::Sports: return "Sports";
    case EventCategory::Workshop: return "Workshop";
    case EventCategory::Music: return "Music";
    case EventCategory::Marathon: return "Marathon";
    case EventCategory::Festival: return "Festival";
    case EventCategory::Charity: return "Charity";
    case EventCategory::Comedy: return "Comedy";
	case EventCategory::Theatre: return "Theatre";
	case EventCategory::Pet: return "Pet";
	case EventCategory::Food: return "Food";
	case EventCategory::Technology: return "Technology";
	case EventCategory::Competition: return "Competition";
    default: return "Unknown";
    }
}

void Event::addCategory(const string& catName, double price, int available) {
    if (price < 0) {
        cout << "Error: Ticket price cannot be negative.\n";
        return;
    }
    if (available < 0) {
        cout << "Error: Ticket availability cannot be negative.\n";
        return;
    }
    categoryOptions.push_back({ catName, { price, available } });
}

// Event constructor
Event::Event(const string& name,
    const string& description,
    EventCategory category,
    time_t startDate,
    time_t endDate,
    const string& location,
    const string& organizer,
    double ticketPrice,
    const vector<pair<string, pair<double, int>>>& categoryOptions)
    : name(name), description(description), category(category),
    startDate(startDate), endDate(endDate), location(location), 
    organizer(organizer), ticketPrice(ticketPrice),
    categoryOptions(categoryOptions) {}

// Getters
time_t Event::getStartDate() const { return startDate; }
time_t Event::getEndDate() const { return endDate; }

// Setters
void Event::setStartDate(time_t newStart) { startDate = newStart; }
void Event::setEndDate(time_t newEnd) { endDate = newEnd; }

void Event::updateCategoryCapacity(size_t index, int capacityChange) {
    if (index >= categoryOptions.size()) {
        cout << "Invalid category index.\n";
        return;
    }

    int& currentCapacity = categoryOptions[index].second.second; // reference to capacity
    int updatedCapacity = currentCapacity + capacityChange;

    if (updatedCapacity < 0) {
        cout << "Error: Capacity cannot be negative.\n";
        return;
    }

    currentCapacity = updatedCapacity;
}

void Event::listCategories() const {
    if (categoryOptions.empty()) {
        cout << "No ticket categories available.\n";
        return;
    }
    for (size_t i = 0; i < categoryOptions.size(); i++) {
        cout << i << ". " << categoryOptions[i].first
            << " | Price: RM" << categoryOptions[i].second.first
            << " | Available: " << categoryOptions[i].second.second
            << "\n";
    }
}


void Event::printDetails(int index) const {
    // Convert time_t → readable string
    char startBuffer[80], endBuffer[80];
    std::tm tmStart{}, tmEnd{};
#ifdef _WIN32
    localtime_s(&tmStart, &startDate);
    localtime_s(&tmEnd, &endDate);
#else
    localtime_r(&startDate, &tmStart);
    localtime_r(&endDate, &tmEnd);
#endif
    strftime(startBuffer, sizeof(startBuffer), "%Y-%m-%d %H:%M", &tmStart);
    strftime(endBuffer, sizeof(endBuffer), "%Y-%m-%d %H:%M", &tmEnd);

    cout << "\n==================== Event ====================\n";
    if (index >= 0) {
        cout << "[" << index << "] " << name << "\n";
        cout << "----------------------------------------------------\n";
    }
    else {
        cout << name << "\n";
        cout << "----------------------------------------------------\n";
    }

    cout << " Description : " << description << "\n";
    cout << " Location    : " << location << "\n";
    cout << " Organizer   : " << organizer << "\n";
    cout << " Start Date  : " << startBuffer << "\n";
    cout << " End Date    : " << endBuffer << "\n";
    cout << " Category    : " << Event::categoryToString(category) << "\n";
    cout << " Base Price  : RM" << std::fixed << std::setprecision(2) << ticketPrice << "\n";

    if (!categoryOptions.empty()) {
        cout << " Ticket Categories:\n";
        int i = 0;
        for (const auto& opt : categoryOptions) { // renamed from 'category' to avoid shadowing
            cout << "   [" << i++ << "] "
                << opt.first
                << " | Price: RM" << opt.second.first
                << " | Availability: " << opt.second.second << "\n";
        }
    }
    cout << "====================================================\n";
}

