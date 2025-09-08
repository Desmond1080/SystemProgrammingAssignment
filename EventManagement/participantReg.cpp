#include "participantRegHeader.h"
#include "function.h"
#include "Event.h"
#include "EventManager.h"
#include "FileManager.h"
#include "payment.h"

using namespace std;

void browseEvents(User* user) {
	vector<Event> events;
	//loadEvents(events);

    // Match selected user count from enum class

    while (true) {
        clearScreen();
        //cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input buffer

        int catChoice = 0;

	    // Menu to select category
	    displayCatMenu();

        // Display exit as last option
        cout << static_cast<int>(EventCategory::Count) + 1 << ". Return" << endl;

        cout << endl << "Select Choice: ";
        //cin >> catChoice;

		// Check if input is valid
		if (!(cin >> catChoice)) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Invalid choice. Please try again." << endl;
            system("pause"); // Press any key to continue
			continue;
		}

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

        EventCategory category = static_cast<EventCategory>(catChoice);

		// Event manager
        EventManager manager;
        string filename = "events.json";

        // Load existing events if file exists
        auto existing = FileManager::loadFromJSON(filename);
        for (auto& e : existing) {
            manager.addEvent(e);
        }

        vector<Event> categoryEvents = manager.searchEventsByCategory(category);

        // Filter out events that are full and past
        time_t now = time(0);
        categoryEvents.erase(remove_if(categoryEvents.begin(), categoryEvents.end(),
			[now](const Event& e) {
				bool allSoldOut = all_of(e.categoryOptions.begin(), e.categoryOptions.end(),
					[](const pair<string, pair<double, int>>& cat) {
						return cat.second.second <= 0; // Check if capacity is 0
					});
				return allSoldOut || e.endDate < now; // Remove if all categories sold out or event has ended
			}), categoryEvents.end());

		if (categoryEvents.empty()) {
			//cout << "No events found in this category." << endl;
            system("pause"); // Press any key to continue
            continue;
		}

        selectEvent(categoryEvents, category, user);
    }
}

void selectEvent(const vector<Event>& categoryEvents, EventCategory category, User* user) {
    while (true) {
        //cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input buffer

        clearScreen();
	    cout << "#" << Event::categoryToString(category) << endl;

        int eventChoice = 0;

        listEventsUser(categoryEvents);

        cout << "Select Event to view details or 0 to return: ";
        //cin >> eventChoice;

		// Check if input is valid
		if (!(cin >> eventChoice)) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Invalid choice. Please try again." << endl;
            system("pause"); // Press any key to continue
			continue;
		}

        // Invalid choice
        if (eventChoice < 0 || eventChoice > static_cast<int>(categoryEvents.size())) {
            cout << "Invalid choice. Please try again." << endl;
			system("pause"); // Press any key to continue
			//clearScreen();
			continue;
		}

        if (eventChoice == 0) {
			//clearScreen();
			return; // Return to category menu
		}

        // Display event details and ticket prices
        Event selectedEvent = categoryEvents[eventChoice - 1];
        selectTicket(selectedEvent, user);
    }
}

void selectTicket(Event event, User* user) {
    // vector of tickets to buy
    // ticketType index, quantity
    vector<pair<int, int>> ticketsToBuy;

    while (true) {
        clearScreen();
        event.printDetailsForUser();

        int ticketChoice = 0;

        // Select ticket type to buy or return
        cout << "Select ticket category to buy or 0 to return: ";
        //cin >> ticketChoice;

        // Check if input is valid
        if (!(cin >> ticketChoice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid choice. Please try again." << endl;
            system("pause"); // Press any key to continue
            continue;
        }

        // Invalid choice
        if (ticketChoice < 0 || ticketChoice > event.categoryOptions.size()) {
            cout << "Invalid choice. Please try again." << endl;
            system("pause"); // Press any key to continue
            continue;
        }

        if (ticketChoice == 0) {
			return; // Return to events in category
		}

        pair<string, pair<double, int>> selectedCategory = event.categoryOptions.at(ticketChoice - 1);
        
        int quantity = selectTicketQuantity(selectedCategory);

        if (quantity == 0) {
            continue; // Return to ticket category selection
		} else {
            // Check if already selected
            bool existing = false;

            for (auto& t : ticketsToBuy) {
				if (t.first == ticketChoice - 1) {
					existing = true;
                    // Add to existing quantity
                    t.second += quantity;
					break;
				}
			}

            // Add new entry if not found
            if (!existing) {
			    ticketsToBuy.push_back({ticketChoice - 1, quantity}); // Store index and quantity
            }
        }

        double totalAmount = 0.0;

        displayCart(ticketsToBuy, event, totalAmount);

        char proceedChoice = ' ';

		//string choiceStr;
		//cout << "Add a ticket category? (y/n): ";
		//getline(cin, choiceStr);
		//if (!choiceStr.empty()) {
		//	addMore = choiceStr[0];
		//}

        while (true) {
            cout << "Enter 'Y' to proceed to checkout or 'N' to buy more tickets: ";
            
            // Check if input is valid
            if (!(cin >> proceedChoice)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid choice. Please try again." << endl;
                system("pause"); // Press any key to continue
                continue;
            }

            if (toupper(proceedChoice) != 'Y' && toupper(proceedChoice) != 'N') {
                cout << "Invalid choice. Please try again" << endl;
                system("pause"); // Press any key to continue
            }
            else {
                break;
            }
        }

        if (toupper(proceedChoice) == 'Y') {
			// Proceed to checkout
            makePayment(ticketsToBuy, user, event, ticketChoice - 1);
			break; // Exit loop after checkout
		} else if (toupper(proceedChoice) == 'N') {
			continue; // Continue loop to select more tickets
        }
    }
}

void makePayment(vector<pair<int, int>> ticketsToBuy, User* user, Event event, int index) {
    Payment payment;

    if (user == nullptr) {
        getGuestDetails(payment);
    }
    else {
        payment.name = user -> username.firstname + user -> username.lastname;
		payment.email = user -> email;
		payment.phone = user -> contactNumber;
    }

    payment.eventName = event.name;
    payment.tickets = ticketsToBuy;

    // Show summary
    clearScreen();

    double totalAmount = 0.0;

    displayCart(ticketsToBuy, event, totalAmount);

    payment.amount = totalAmount;

    if (user == nullptr) {
        showPaymentSummary(payment);
    }
    else {
        cout << "----- Payment Summary -----\n";
        cout << "Name        : " << payment.name << endl;
        cout << "Email       : " << payment.email << endl;
        cout << "Phone       : " << payment.phone << endl;
    }

    // Choose method
    choosePaymentMethod(payment);

    // Process payment
    if (processPayment(payment)) {
        EventManager manager;
        string filename = "events.json";

        // Load existing events if file exists
        auto existing = FileManager::loadFromJSON(filename);
        for (auto& e : existing) {
            manager.addEvent(e);
        }

        // Make a copy so we can modify
        Event updated = manager.getEvents()[index];

        int eventIdx = -1;
        const auto& events = manager.getEvents();

        for (size_t i = 0; i < events.size(); ++i) {
            if (events[i].name == event.name) {
                eventIdx = static_cast<int>(i);
                break;
            }
        }
        if (eventIdx != -1) {
            // Update ticket availability
            for (const auto& t : ticketsToBuy) {
                int catIndex = t.first;
                int qty = t.second;
                if (catIndex >= 0 && catIndex < static_cast<int>(updated.categoryOptions.size())) {
                    int currentCapacity = updated.categoryOptions.at(catIndex).second.second;
                    updated.updateCategoryCapacity(catIndex, currentCapacity - qty);
                }
            }

            manager.editEvent(eventIdx, updated);
            FileManager::saveToJSON("events.json", manager.getEvents());
        }

		//// Update ticket availability
  //      for (const auto& t : ticketsToBuy) {
  //          int catIndex = t.first;
  //          int qty = t.second;

  //          int currentCapacity = updated.categoryOptions.at(catIndex).second.second;
  //          updated.updateCategoryCapacity(catIndex, currentCapacity - qty);
  //      }

		//manager.editEvent(index, updated);
  //      FileManager::saveToJSON("events.json", manager.getEvents());
    }
}

void displayCart(vector<pair<int, int>> ticketsToBuy, Event event, double& totalAmount){
    // e.g. (2x VIP - RM150; Total RM300)
	cout << "Your Cart:" << endl;

	for (const auto& t : ticketsToBuy) {
		int index = t.first;
		int qty = t.second;

		double price = event.categoryOptions.at(index).second.first;
		double subTotal = price * qty;
		totalAmount += subTotal;

		cout << qty << " x " << event.categoryOptions.at(index).first
			 << " - RM" << price << " each"
			 << " | Subtotal: RM" << subTotal << endl;
	}

	cout << endl << "Total Amount: RM" << totalAmount << endl << endl;
}

int selectTicketQuantity(const pair<string, pair<double, int>>& category) {
    int ticketsToBuy = 0;

	while (true) {
		int quantity = 0;

		cout << "Enter quantity of " << category.first << " tickets - RM" << category.second.first << " to buy (or 0 to cancel): ";
		cin >> quantity;

		if (quantity < 0) {
			cout << "Invalid quantity. Please try again." << endl;
			system("pause"); // Press any key to continue
			continue;
		} else if (quantity == 0) {
			cout << "Purchase cancelled." << endl;
			system("pause"); // Press any key to continue
			return ticketsToBuy;
		} else if (quantity > category.second.second) {
			cout << "Only " << category.second.second << " tickets available. Please try again." << endl;
			system("pause"); // Press any key to continue
			continue;
		} else {
			cout << endl << "Added " << quantity << " x " << category.first << " tickets to your cart." << endl << endl;
			ticketsToBuy += quantity;
            return ticketsToBuy;
		}
	}
}

// Event cards
void listEventsUser(const vector<Event>& events) {
    cout << "=====================================" << endl;

    for (size_t i = 0; i < events.size(); i++) {
        const Event& e = events[i];

        //// Convert time_t ? readable string
        //std::tm tmStruct{};
        //localtime_s(&tmStruct, &e.date); // safer version on MSVC
        //std::ostringstream dateStream;
        //dateStream << std::put_time(&tmStruct, "%Y-%m-%d %H:%M");

        // Convert time_t ? readable string
        char startBuffer[80], endBuffer[80];
        std::tm tmStart{}, tmEnd{};
    #ifdef _WIN32
        localtime_s(&tmStart, &e.startDate);
        localtime_s(&tmEnd, &e.endDate);
    #else
        localtime_r(&startDate, &tmStart);
        localtime_r(&endDate, &tmEnd);
    #endif
        strftime(startBuffer, sizeof(startBuffer), "%Y-%m-%d %H:%M", &tmStart);
        strftime(endBuffer, sizeof(endBuffer), "%Y-%m-%d %H:%M", &tmEnd);

        cout << "#" << i + 1 << endl;
        cout << e.name << endl;
        cout << "Start Date : " << startBuffer << "\n";
        cout << "End Date   : " << endBuffer << "\n";
        cout << "Location   : " << e.location << endl;
        //cout << "Date: " << dateStream.str() << endl;

        // From RMxx.xx
        double minPrice = e.categoryOptions.at(0).second.first;

        for (size_t j = 1; j < e.categoryOptions.size(); j++) {
            double catPrice = e.categoryOptions.at(j).second.first;
            if (catPrice < minPrice) {
                minPrice = catPrice;
            }
        }

        cout << fixed << setprecision(2) << "From RM" << minPrice << endl;

        cout << "=====================================" << endl;
    }
}

void displayCatMenu(){
    clearScreen();

    cout << "Select Category:" << endl;

    cout << "=====================================" << endl;

    // for loop to display all categories
    for (int i = 0; i < static_cast<int>(Category::Count); i++) {
        Category cat = static_cast<Category>(i);
        cout << (i + 1) << ". " << catToString(cat) << endl;
    }

    cout << "=====================================" << endl;
}
