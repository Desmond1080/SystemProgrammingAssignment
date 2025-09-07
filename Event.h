#ifndef EVENT_H
#define EVENT_H

#include <string>
#include <vector>
#include <utility>
#include <ctime>
#include <iostream>

using namespace std;

enum class EventCategory {
    Concert,
	Sports,
    Workshop,
    Music,
    Marathon,
    Charity,
    Comedy,
    Theatre,
    Pet,
    Food,
	Technology,
	Competition,
    Festival
};

class Event {
public:
    string name;
    string description;
    EventCategory category;
    time_t date;
    string location;
    string organizer;
    double ticketPrice;

    // Ticket categories: {name, {price, available}}
    vector<pair<string, pair<double, int>>> categoryOptions;

    Event(const string& name,
        const string& description,
        EventCategory category,
        time_t date,
        const string& location,
        const string& organizer,
        double ticketPrice,
        const vector<pair<string, pair<double, int>>>& categoryOptions);

    Event() = default;

    void addCategory(const string& catName, double price, int available);
    void printDetails() const;

    void setDate(time_t newDate);
    time_t getDate() const;

    void updateCategoryCapacity(size_t index, int newCapacity);
    void listCategories() const;

};

#endif
