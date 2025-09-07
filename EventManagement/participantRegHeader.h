#pragma once

#include "structs.h"
#include "function.h"
#include "Event.h"

#include <vector>
#include <iostream>
#include <cctype>

using namespace std;

void browseEvents(User* user);
void displayCatMenu();
void listEventsUser(const vector<Event>& events);
void selectEvent(const vector<Event>& categoryEvents, EventCategory category, User* user);
void selectTicket(Event event, User* user);
int selectTicketQuantity(const pair<string, pair<double, int>>& category);
void makePayment(vector<pair<int, int>> ticketsToBuy, User* user, Event event, int index);
void displayCart(vector<pair<int, int>> ticketsToBuy, Event event, double& totalAmount);
//vector<Event> displayEventInCategory(
//    const vector<Event>& events,
//    Category category
//);
//void loadEvents(Event events[]);
