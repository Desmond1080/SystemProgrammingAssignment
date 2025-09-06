#include "Event.h"

using namespace std;

// Convert enum to string
string categoryToString(EventCategory category) {
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
    categoryOptions.push_back({ catName,{ price, available} });
}

Event::Event(const string& name,
    const string& description,
    EventCategory category,
    time_t date,
    const string& location,
    const string& organizer,
    double ticketPrice,
    const vector<pair<string, pair<double, int>>>& categoryOptions)
    : name(name), description(description), category(category), date(date),
    location(location), organizer(organizer), ticketPrice(ticketPrice),
    categoryOptions(categoryOptions) {}

void Event::setDate(time_t newDate) {
    date = newDate;
}

time_t Event::getDate() const {
    return date;
}

void Event::updateCategoryCapacity(size_t index, int newCapacity) {
    if (index < categoryOptions.size()) {
        categoryOptions[index].second.second = newCapacity;
    }
    else {
        std::cout << "Invalid category index.\n";
    }
}

void Event::listCategories() const {
    for (size_t i = 0; i < categoryOptions.size(); i++) {
        std::cout << i << ". " << categoryOptions[i].first
            << " | Price: $" << categoryOptions[i].second.first
            << " | Available: " << categoryOptions[i].second.second
            << "\n";
    }
}

void Event::printDetails() const {
    cout << "Event: " << name << endl;
    cout << "Description: " << description << endl;
    cout << "Category: " << categoryToString(category) << endl;

    // Convert time_t into readable date
    struct tm readable;
    localtime_s(&readable, &date);
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
