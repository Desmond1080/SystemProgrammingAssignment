#include "function.h"
#include "participantRegHeader.h"

#include <iostream>

using namespace std;

// guest details 
void continueAsGuest() {
	//Menu to browse event, or return
	User user;
	browseEvents(user);
}