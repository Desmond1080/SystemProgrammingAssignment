#pragma once

#define FUNCTION_H

#include <iostream>
#include <string>
#include <limits>
#include <regex>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <openssl/evp.h>
#include <openssl/rand.h>

using namespace std;

const int MAX_LOGIN_ATTEMPTS = 5;
const int SALT_LENGTH = 16;
const int HASH_LENGTH = 64;
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

void userRegister();
void userLogin();
void displayUserDetails();

bool validateEmail(string& email);
bool validatePassword(string& password);
bool validateContactNumber(string& contactNumber);
bool validateGender(string& gender);

void saveUserProfile(const User& user);

string generateSalt();