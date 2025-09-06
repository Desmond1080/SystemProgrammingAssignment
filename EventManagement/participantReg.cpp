#include "participantRegHeader.h"
#include "function.h"
#include "Event.h"
#include "EventManager.h"
#include "FileManager.h"

using namespace std;

void browseEvents(User user) {
	vector<Event> events;
	//loadEvents(events);

    // Match selected user count from enum class

    while (true) {
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input buffer

        int catChoice = 0;

	    // Menu to select category
	    displayCatMenu();

        // Display exit as last option
        cout << static_cast<int>(EventCategory::Count) + 1 << ". Return" << endl;

        cout << "Select Choice: ";
        cin >> catChoice;

        // Invalid choice
        if (catChoice < 1 || catChoice > static_cast<int>(Category::Count) + 1) {
			cout << "Invalid choice. Please try again." << endl;
			system("pause"); // Press any key to continue
			continue;
		} 
        
        // Valid choice
		catChoice -= 1; // Adjust for 0-based index

		// Exit to main menu
		if (catChoice == static_cast<int>(EventCategory::Count)) {
            clearScreen();
			break;
        }

        clearScreen();

	    // Category category = Category::Concert; // Example category
        EventCategory category = static_cast<EventCategory>(catChoice);

	    cout << "#" << categoryToString(category) << endl;

		// Display events in the selected category
        EventManager manager;
        string filename = "events.json";

        // Load existing events if file exists
        auto existing = FileManager::loadFromJSON(filename);
        for (auto& e : existing) {
            manager.addEvent(e);
        }
        vector<Event> categoryEvents = manager.searchEventsByCategory(category);

		if (categoryEvents.empty()) {
			cout << "No events found in this category." << endl;
            system("pause"); // Press any key to continue
            continue;
		}
        
        listEventsUser(categoryEvents);
        system("pause"); // Press any key to continue

        // Select to view event details and ticket prices
        // Return or book ticket
        // 
    }
}

void listEventsUser(const vector<Event>& events) {
    for (size_t i = 0; i < events.size(); i++) {
        const Event& e = events[i];

        // Convert time_t ? readable string
        std::tm tmStruct{};
        localtime_s(&tmStruct, &e.date); // safer version on MSVC
        std::ostringstream dateStream;
        dateStream << std::put_time(&tmStruct, "%Y-%m-%d %H:%M");

        cout << i + 1 << ". " << e.name << endl;
        cout << "Location: " << e.location << endl;
        cout << "Date: " << dateStream.str() << endl;

        // From RMxx.xx
        // Print ticket categories
        for (const auto& catPair : e.categoryOptions) {
            cout << "   - " << catPair.first //catName
                << " | Price: " << catPair.second.first //catPrice
                << " | Available: " << catPair.second.second << endl; 
        }
    }
}

//void loadEvents(vector<Event>& events){
//    // Mock event data
//    // To be read from file
//    events.push_back({ Category::Concert, "g-dragon day 1", "details....", {"axiata arena", "11700", "bukit jalil", State::WP_Kuala_Lumpur}, "2023-10-10", "10:00", {"galaxy", "only the best events"} });
//    events.push_back({ Category::Concert, "g-dragon day 2", "details....", {"axiata arena", "11700", "bukit jalil", State::WP_Kuala_Lumpur}, "2023-10-11", "10:00", {"galaxy", "only the best events"} });
//}

void displayCatMenu(){
    system("cls");

    cout << "Select Category:" << endl;

    // for loop to display all categories
    for (int i = 0; i < static_cast<int>(Category::Count); i++) {
        Category cat = static_cast<Category>(i);
        cout << (i + 1) << ". " << catToString(cat) << endl;
    }
}

//vector<Event> getEventInCategory(
//    const vector<Event>& events,
//    Category category
//){
//    vector<Event> result;
//    for (const auto& event : events) {
//        if (event.category == category) {
//            result.push_back(event);
//        }
//    }
//    return result;
//}

void displayEvents(
    Category category
){
}
