#pragma once

#include "structs.h"
#include "function.h"

#include <vector>
#include <iostream>

void browseEvents(User user);
void displayCatMenu();
vector<Event> displayEventInCategory(
    const vector<Event>& events,
    Category category
);
void loadEvents(Event events[]);
