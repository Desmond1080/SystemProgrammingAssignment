#pragma once

#include "structs.h"

#include <vector>
#include <iostream>

void displayCatMenu();
vector<Event> displayEventInCategory(
    const vector<Event>& events,
    Category category
);
void loadEvents(Event events[]);
