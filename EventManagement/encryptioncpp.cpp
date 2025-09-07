#include "function.h"
#include <iostream>
#include <string>

using namespace std;

//generate random salt 
string generateSalt() {
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<unsigned int> dis(0, 255);
	
	string salt;
	for(int i = 0; i < SALT_LENGTH; ++i) {
		salt += static_cast<char>(dis(gen));
	}
	return salt;
}

//hash function 
string hashPassword(const string& password, const string& salt) {
	string combined = salt + password;
	hash<string> hasher;
	size_t hashValue = hasher(combined);

	// convert hash value to hex string
	stringstream ss;
	ss << hex << setw(16) << setfill('0') << hashValue;
	return ss.str();
}

