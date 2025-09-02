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
	int loginAttempts;
};

//create event details using structure 
struct event {
	
};

//category 
struct category {

};

//create ticket details using structure 
struct ticket {
	string ticketID;
};

void clearScreen();

// guest functions 
void continueAsGuest();

//user functions
void userRegister();
bool userLogin(User &loginUser);
void displayUserProfile(const User &user);
void editUserProfile(User &user);
void deleteUserAccount(User& user);

//validations
bool validateEmail(string& email);
bool validatePassword(string& password);
bool validateContactNumber(string& contactNumber);
bool validateGender(string& gender);

//file input io
void saveUserProfile(const User& user);
bool loadUserProfile(const string& email, User& user);
vector<User> storeAllUsers();
void updateUserProfile(const User& user);
void removeUserProfile(const string& email);

//forgot password
void forgotPassword(const string& email);
void updatePassword(const string& email, const string& newPassword);

//user password encryption
string generateSalt();
string hashPassword(const string& password, const string& salt);