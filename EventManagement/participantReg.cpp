#include "participantRegHeader.h"

using namespace std;

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

void loadEvents(Event events[]){
    // Mock event data
    // To be read from file
    events[0] = {Category::Concert, "g-dragon day 1", "details....", {"axiata arena", "11700", "bukit jalil", State::WP_Kuala_Lumpur}, "2023-10-10", "10:00", {"galaxy", "only the best events"}};
    events[1] = {Category::Concert, "g-dragon day 2", "details....", {"axiata arena", "11700", "bukit jalil", State::WP_Kuala_Lumpur}, "2023-10-11", "10:00", {"galaxy", "only the best events"}};
}
