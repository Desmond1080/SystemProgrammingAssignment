#include "participantRegHeader.h"

int main1(){
    Event events[5];
    loadEvents(events);

    displayCatMenu();
    
    // Menu to select category
    Category category = Category::Concert; // Example category

    cout << "#" << catToString(category) << endl;

    Event categoryEvents[5];

    // getEventInCategory(categoryEvents, category);

    // displayEvents(category);

    return 0;
}
