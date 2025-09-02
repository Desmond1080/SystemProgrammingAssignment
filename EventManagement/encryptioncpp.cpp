#include "function.h"
#include <iostream>
#include <string>

using namespace std;

string generateSalt() {
	unsigned char salt[SALT_LENGTH];
	if(RAND_bytes(salt, sizeof(salt)) != 1) {
		//fall back to less secure method if RAND_bytes fails
		srand(time(0));
		for(int i = 0; i < SALT_LENGTH; ++i) {
			salt[i] = rand() % 256;
		}
	}

	// convert to hex string
	stringstream ss;
	for(int i = 0; i < SALT_LENGTH; ++i) {
		ss << hex << setw(2) << setfill('0') << (int)salt[i];
	}
	return ss.str();