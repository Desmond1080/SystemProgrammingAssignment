#ifndef EVENT_H
#define EVENT_H

#include <string>
#include <vector>
#include <utility>
#include <ctime>
#include <iostream>

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
    Festival,
    Count // Helper to get number of categories
};

class Event {
public:
    std::string name;
    std::string description;
    EventCategory category;
    std::time_t startDate;
    std::time_t endDate;
    std::string location;
    std::string organizer;
    double ticketPrice;

    // Ticket categories: {name, {price, available}}
    std::vector<std::pair<std::string, std::pair<double, int>>> categoryOptions;

    static std::string categoryToString(EventCategory category);

    Event(const std::string& name,
        const std::string& description,
        EventCategory category,
        const std::time_t startDate,
        const std::time_t endDate,
        const std::string& location,
        const std::string& organizer,
        double ticketPrice,
        const std::vector<std::pair<std::string, std::pair<double, int>>>& categoryOptions);

    Event() = default;

    void setStartDate(std::time_t newStart);
    void setEndDate(std::time_t newEnd);

    std::time_t getStartDate() const;
    std::time_t getEndDate() const;

    void updateCategoryCapacity(std::size_t index, int capacityChange);
    void listCategories() const;
	void addCategory(const std::string& catName, double price, int available);
	void printDetails(int index = -1) const;
};

#endif
