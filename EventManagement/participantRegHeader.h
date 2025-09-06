#pragma once

#include "structs.h"
#include "function.h"
#include "Event.h"

#include <vector>
#include <iostream>

using namespace std;

void browseEvents(User user);
void displayCatMenu();
void listEventsUser(const vector<Event>& events);
void selectEvent(const vector<Event>& categoryEvents);
//vector<Event> displayEventInCategory(
//    const vector<Event>& events,
//    Category category
//);
//void loadEvents(Event events[]);
