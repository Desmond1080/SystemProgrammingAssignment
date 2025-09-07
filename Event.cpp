#include "Event.h"
#include <iomanip>
#include <iostream>
#include <ctime>
#include <sstream>
#include <algorithm>

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

static vector<string> wrapText(const string& text, size_t width) {
    vector<string> lines;
    stringstream ss(text);
    string word, line;

    while (ss >> word) {
        if (line.size() + word.size() + 1 > width) {
            lines.push_back(line);
            line = word;
        }
        else {
            if (!line.empty()) line += " ";
            line += word;
        }
    }
    if (!line.empty()) lines.push_back(line);

    if (lines.empty()) lines.push_back("");
    return lines;
}

void Event::printDetails(int index) const {
    // Convert time_t → readable string
    char startBuffer[80], endBuffer[80];
    tm tmStart{}, tmEnd{};
#ifdef _WIN32
    localtime_s(&tmStart, &startDate);
    localtime_s(&tmEnd, &endDate);
#else
    localtime_r(&startDate, &tmStart);
    localtime_r(&endDate, &tmEnd);
#endif
    strftime(startBuffer, sizeof(startBuffer), "%Y-%m-%d %H:%M", &tmStart);
    strftime(endBuffer, sizeof(endBuffer), "%Y-%m-%d %H:%M", &tmEnd);

    // ===== Details section =====
    vector<pair<string, string>> details = {
        {"Description", description},
        {"Location", location},
        {"Organizer", organizer},
        {"Start Date", startBuffer},
        {"End Date", endBuffer},
        {"Category", Event::categoryToString(category)}
    };

    ostringstream basePrice;
    basePrice << "RM" << fixed << setprecision(2) << ticketPrice;
    details.push_back({ "Base Price", basePrice.str() });

    // compute column widths (field only, value will be capped for wrapping)
    size_t field_w = 0;
    for (auto& p : details) {
        field_w = max(field_w, p.first.size());
    }

    // wrapping max width for values
    const size_t value_w = 40;

    auto printBorder = [&](size_t f, size_t v) {
        cout << "+" << string(f + 2, '-')
            << "+" << string(v + 2, '-') << "+\n";
        };

    auto printRow = [&](const string& field, const string& value) {
        auto wrapped = wrapText(value, value_w);
        for (size_t i = 0; i < wrapped.size(); i++) {
            if (i == 0) {
                cout << "| " << setw((int)field_w) << left << field
                    << " | " << setw((int)value_w) << left << wrapped[i]
                    << " |\n";
            }
            else {
                cout << "| " << setw((int)field_w) << left << ""
                    << " | " << setw((int)value_w) << left << wrapped[i]
                    << " |\n";
            }
        }
        };

    // ===== Print header =====
    string title = (index >= 0)
        ? ("Event [" + to_string(index) + "] " + name)
        : ("Event: " + name);
    size_t title_w = title.size();
    size_t total_inner = max(field_w + value_w + 5, title_w);

    auto printFullBorder = [&]() {
        cout << "+" << string(total_inner, '-') << "+\n";
        };
    auto printCentered = [&](const string& s) {
        int padLeft = (int)((total_inner - s.size()) / 2);
        if (padLeft < 0) padLeft = 0;
        cout << "|" << string(padLeft, ' ')
            << s
            << string(total_inner - s.size() - padLeft, ' ')
            << "|\n";
        };

    printFullBorder();
    printCentered("Event List");
    printFullBorder();

    cout << "|" << setw((int)total_inner) << left << title << "|\n";
    printBorder(field_w, value_w);

    for (auto& p : details) printRow(p.first, p.second);
    printBorder(field_w, value_w);

    // ===== Ticket Categories =====
    if (!categoryOptions.empty()) {
        vector<string> ids, names, prices, avails;
        int i = 0;
        for (auto& opt : categoryOptions) {
            ids.push_back(to_string(i++));
            names.push_back(opt.first);

            ostringstream priceSS;
            priceSS << fixed << setprecision(2) << opt.second.first;
            prices.push_back(priceSS.str());

            avails.push_back(to_string(opt.second.second));
        }

        size_t id_w = 2, name_w = 4, price_w = 9, avail_w = 11;
        for (auto& s : ids) id_w = max(id_w, s.size());
        for (auto& s : names) name_w = max(name_w, s.size());
        for (auto& s : prices) price_w = max(price_w, s.size());
        for (auto& s : avails) avail_w = max(avail_w, s.size());

        auto printTicketBorder = [&]() {
            cout << "+" << string(id_w + 2, '-')
                << "+" << string(name_w + 2, '-')
                << "+" << string(price_w + 2, '-')
                << "+" << string(avail_w + 2, '-') << "+\n";
            };

        cout << "|" << setw((int)total_inner) << left << " Ticket Categories" << "|\n";
        printTicketBorder();

        cout << "| " << setw((int)id_w) << left << "ID"
            << " | " << setw((int)name_w) << left << "Name"
            << " | " << setw((int)price_w) << right << "Price (RM)"
            << " | " << setw((int)avail_w) << right << "Availability"
            << " |\n";
        printTicketBorder();

        for (size_t j = 0; j < ids.size(); j++) {
            cout << "| " << setw((int)id_w) << left << ids[j]
                << " | " << setw((int)name_w) << left << names[j]
                << " | " << setw((int)price_w) << right << prices[j]
                << " | " << setw((int)avail_w) << right << avails[j]
                << " |\n";
        }

        printTicketBorder();
    }

    printFullBorder();
}
