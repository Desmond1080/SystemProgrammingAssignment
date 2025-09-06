#include "participantRegHeader.h"
#include "function.h"

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

        cout << "Select Choice: ";
        cin >> catChoice;

        // Invalid choice
        if (catChoice < 1 || catChoice > static_cast<int>(Category::Count)) {
			cout << "Invalid choice. Please try again." << endl;
			system("pause"); // Press any key to continue
			continue;
		} 
        
        // Valid choice
		catChoice -= 1; // Adjust for 0-based index

		// Exit to main menu
		if (catChoice == static_cast<int>(Category::Count)) {
			break;
        }

	    // Category category = Category::Concert; // Example category
        Category category = static_cast<Category>(catChoice);

	    cout << "#" << catToString(category) << endl;

	    Event categoryEvents[5];



	    // getEventInCategory(categoryEvents, category);

	    // displayEvents(category);
    }
}

void loadEvents(vector<Event>& events){
    // Mock event data
    // To be read from file
    events.push_back({ Category::Concert, "g-dragon day 1", "details....", {"axiata arena", "11700", "bukit jalil", State::WP_Kuala_Lumpur}, "2023-10-10", "10:00", {"galaxy", "only the best events"} });
    events.push_back({ Category::Concert, "g-dragon day 2", "details....", {"axiata arena", "11700", "bukit jalil", State::WP_Kuala_Lumpur}, "2023-10-11", "10:00", {"galaxy", "only the best events"} });
}

void displayCatMenu(){
    system("cls");

    cout << "Select Category:" << endl;

    // for loop to display all categories
    for (int i = 0; i < static_cast<int>(Category::Count); i++) {
        Category cat = static_cast<Category>(i);
        cout << (i + 1) << ". " << catToString(cat) << endl;
    }
}

vector<Event> getEventInCategory(
    const vector<Event>& events,
    Category category
){
    vector<Event> result;
    for (const auto& event : events) {
        if (event.category == category) {
            result.push_back(event);
        }
    }
    return result;
}

void displayEvents(
    Category category
){
}
