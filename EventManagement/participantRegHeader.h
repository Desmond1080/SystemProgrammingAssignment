#pragma once

#include "structs.h"
<<<<<<< HEAD
#include "function.h"
#include "Event.h"

#include <vector>
#include <iostream>
#include <cctype>

using namespace std;

void browseEvents(User user);
void displayCatMenu();
void listEventsUser(const vector<Event>& events);
void selectEvent(const vector<Event>& categoryEvents, EventCategory category, User user);
void selectTicket(Event event, User user);
int selectTicketQuantity(const pair<string, pair<double, int>>& category);
//vector<Event> displayEventInCategory(
//    const vector<Event>& events,
//    Category category
//);
//void loadEvents(Event events[]);
=======

#include <vector>
#include <iostream>

void displayCatMenu();
vector<Event> displayEventInCategory(
    const vector<Event>& events,
    Category category
);
void loadEvents(Event events[]);
>>>>>>> master
