#include "function.h"
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

void saveUserProfile(const User& user) {
	ofstream file("user.txt", ios::app);
	if (file.is_open()) {
		file << user.username.firstname << "|" << user.username.lastname << "|" << user.nickname << "|" << user.age << "|" << user.gender << "|" << user.contactNumber << "|" << user.email << "|" << user.passwordHash << "|" << user.salt << "|" << user.loginAttempts << endl;
		file.close();
	}
	else {
		cerr << "Error opening file for writing." << endl;
	}
}

//compare email to load the user data 
// return true if found, false if not found
bool loadUserProfile(const string& email, User& user) {
	ifstream file("user.txt");
	if (file.is_open()) {
		string line;
		while (getline(file, line)) {
			vector<string> fields;
			stringstream ss(line);
			string field;

			while (getline(ss, field, '|')) {
				fields.push_back(field);
			}

			if (fields.size() >= 10 && fields[6] == email ) {
				user.username.firstname = fields[0];
				user.username.lastname = fields[1];
				user.nickname = fields[2];
				user.age = fields[3];
				user.gender = fields[4];
				user.contactNumber = fields[5];
				user.email = fields[6];
				user.passwordHash = fields[7];
				user.salt = fields[8];
				user.loginAttempts = stoi(fields[9]);
				file.close();
				return true;
			}

		}
		file.close();
	}
	return false;
}

//store all users into a vector 
vector<User> storeAllUsers() {
	vector<User> users;
	ifstream file("user.txt");

	if (file.is_open()) {
		string line;
		while (getline(file, line)) {
			vector<string> fields;
			stringstream ss(line);
			string field;
			while (getline(ss, field, '|')) {
				fields.push_back(field);
			}

			if (fields.size() >= 10) {
				User user;
				user.username.firstname = fields[0];
				user.username.lastname = fields[1];
				user.nickname = fields[2];
				user.age = fields[3];
				user.gender = fields[4];
				user.contactNumber = fields[5];
				user.email = fields[6];
				user.passwordHash = fields[7];
				user.salt = fields[8];
				user.loginAttempts = stoi(fields[9]);

				users.push_back(user);
			}
		}
		file.close();
	}
	return users;
}

//update user profile 
void updateUserProfile(const User& user) {
	vector<User> users = storeAllUsers();
	ofstream file("user.txt");

	if (file.is_open()) {
		for (int i = 0; i < users.size(); i++) {
			User currentUser = users[i];

			if (currentUser.email == user.email) {
				// write new updated user data 
				file << user.username.firstname << "|" << user.username.lastname << "|" << user.nickname << "|" << user.age << "|" << user.gender << "|" << user.contactNumber << "|" << user.email << "|" << user.passwordHash << "|" << user.salt << "|" << user.loginAttempts << endl;
			}
			else {
				file << currentUser.username.firstname << "|" << currentUser.username.lastname << "|" << currentUser.nickname << "|" << currentUser.age << "|" << currentUser.gender << "|" << currentUser.contactNumber << "|" << currentUser.email << "|" << currentUser.passwordHash << "|" << currentUser.salt << "|" << currentUser.loginAttempts << endl;
			}
		}
		file.close();
	}
	else {
		cerr << "Error opening file for writing." << endl;
	}
}

// remove user date from file 
void removeUserProfile(const string& email) {
	vector<User> users = storeAllUsers();
	ofstream file("user.txt");

	if (file.is_open()) {
		for (int i = 0; i < users.size(); i++) {
			User currentUser = users[i];

			//check email is it the one to remove 
			bool toRemove = (currentUser.email == email);

			if (!toRemove) {
				file << currentUser.username.firstname << "|" << currentUser.username.lastname << "|" << currentUser.nickname << "|" << currentUser.age << "|" << currentUser.gender << "|" << currentUser.contactNumber << "|" << currentUser.email << "|" << currentUser.passwordHash << "|" << currentUser.salt << "|" << currentUser.loginAttempts << endl;
			}
		}

		file.close();
	}
	else {
		cerr << "Error opening file for writing." << endl;
	}
}

// organizer file input i/o
void saveOrganizerProfile(const Organizer& organizer) {
	ofstream file("organizer.txt", ios::app);
	if (file.is_open()) {
		file << organizer.name << "|" << organizer.email << "|" << organizer.password << "|" << organizer.description << endl;
		file.close();
	}
	else {
		cerr << "Error opening file for writing." << endl;
	}
}

// compare email to load the organizer data
// return true if found, false if not found
bool loadOrganizerProfile(const string& email, Organizer& organizer) {
	ifstream file("organizer.txt");
	if (file.is_open()) {
		string line;
		while (getline(file, line)) {
			vector<string> fields;
			stringstream ss(line);
			string field;

			while (getline(ss, field, '|')) {
				fields.push_back(field);
			}

			if (fields.size() >= 4 && fields[1] == email) {
				organizer.name = fields[0];
				organizer.email = fields[1];
				organizer.password = fields[2];
				organizer.description = fields[3];
				file.close();
				return true;
			}

		}
		file.close();
	}
	return false;
}

//store all organizers into a vector
vector<Organizer> storeAllOrganizer() {
	vector<Organizer> organizers;
	ifstream file("organizer.txt");

	if (file.is_open()) {
		string line;
		while (getline(file, line)) {
			vector<string> fields;
			stringstream ss(line);
			string field;
			while (getline(ss, field, '|')) {
				fields.push_back(field);
			}

			if (fields.size() >= 4) {
				Organizer organizer;
				organizer.name = fields[0];
				organizer.email = fields[1];
				organizer.password = fields[2];
				organizer.description = fields[3];

				organizers.push_back(organizer);
			}
		}
		file.close();
	}
	return organizers;
}

//update organizer profile 
void updateOrganizerProfile(const Organizer& organizer) {
	vector<Organizer> organizers = storeAllOrganizer();
	ofstream file("organizer.txt");

	if (file.is_open()) {
		for (int i = 0; i < organizers.size(); i++) {
			Organizer currentOrganizer = organizers[i];

			if (currentOrganizer.email == organizer.email) {
				// write new updated organizer data 
				file << organizer.name << "|" << organizer.email << "|" << organizer.password << "|" << organizer.description << endl;
			}
			else {
				file << currentOrganizer.name << "|" << currentOrganizer.email << "|" << currentOrganizer.password << "|" << currentOrganizer.description << endl;
			}
		}
		file.close();
	}
	else {
		cerr << "Error opening file for writing." << endl;
	}
}