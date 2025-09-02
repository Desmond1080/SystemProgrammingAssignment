#include <iostream>
#include <string>
#include "function.h"

using namespace std;

int main() {
	int choice;
	while (true) {
		cout << "=== User Management System ===" << endl;
		cout << "1. Register" << endl;
		cout << "2. Login" << endl;
		cout << "3. Display User Details" << endl;
		cout << "4. Exit" << endl;
		cout << "Enter your choice: ";
		cin >> choice;

		switch (choice) {
		case 1:
			userRegister();
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			cout << "Exiting the program." << endl;
			return 0;
		default:
			cout << "Invalid choice. Please try again." << endl;
		}
	}

	return 0;
}