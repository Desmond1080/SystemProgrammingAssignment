#include "function.h"
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

void saveUserProfile(const User& user) {
	ofstream file("user_profile.txt");
	if (file.is_open()) {
		file << user.username.firstname << "|" << user.username.lastname << "|" << user.nickname << "|" << user.age << "|" << user.gender << "|" << user.contactNumber << "|" << user.email << "|" << user.passwordHash << "|" << user.salt << "|" << user.loginAttempts << endl;
		file.close();
	}
	else {
		cerr << "Error opening file for writing." << endl;
	}
}