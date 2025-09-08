#pragma once

#define FUNCTION_H

#include <iostream>
#include <string>
#include <limits>
#include <regex>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <random>
#include <cstdlib>


using namespace std;

const int MAX_LOGIN_ATTEMPTS = 5;
const int SALT_LENGTH = 16;
const int HASH_LENGTH = 16;
const int LOCKOUT_DURATION = 300; // in seconds

//initialize username into two parts
struct Username {
	string firstname;
	string lastname;
};

//create user records using structure
struct User {
	Username username;
	string nickname;
	string age;
	string gender;
	string contactNumber;
	string email;
	string password;
	string passwordHash;
	string salt;
	int loginAttempts = 0;
};

//create organizer records using structure 
struct Organizer {
	string name;
	string email;
	string password;
	string passwordHash;
	string salt;
	string description;
};


//create ticket details using structure 
struct ticket {
	string ticketID;
};

void clearScreen();

//user functions
bool userLoginMenu(User& user);
void userMenu(User& user);
void userRegister();
bool userLogin(User &loginUser);
void displayUserProfile(const User &user);
void editUserProfile(User &user);
void deleteUserAccount(User& user);
void viewTickets(User& user);

//organizer function
bool organizerLoginMenu(Organizer& loginOrganizer);
void organizerMenu(Organizer& organizer);
void organizerRegister();
bool organizerLogin(Organizer& loginOrganizer);
void displayOrganizerProfile(const Organizer& organizer);
void editOrganizerProfile(Organizer& organizer);
void manageEvents();

//validations
bool validateEmail(string& email);
bool validatePassword(string& password);
bool validateContactNumber(string& contactNumber);
bool validateGender(string& gender);
bool validateAge(string& age);
bool validateChoice(int choice, int minChoice, int maxChoice);
bool validateLine(const string& input);
bool validateName(const string& name);
bool validatePasswordConfirmation(const char& input);

//user file input io
void saveUserProfile(const User& user);
bool loadUserProfile(const string& email, User& user);
vector<User> storeAllUsers();
void updateUserProfile(const User& user);
void removeUserProfile(const string& email);

//organizer file input i/o
void saveOrganizerProfile(const Organizer& organizer);
bool loadOrganizerProfile(const string& email, Organizer& organizer);
vector<Organizer> storeAllOrganizer();
void updateOrganizerProfile(const Organizer& organizer);

//forgot password
void forgotPassword(const string& email);
void updatePassword(const string& email, const string& newPassword);

//user password encryption
string generateSalt();
string hashPassword(const string& password, const string& salt);