#include "function.h"
#include <string>
#include <iostream>

using namespace std;

// for user 
void forgotPassword(const string& email) {
	User user1;
	if (!loadUserProfile(email, user1)) {
		cout << "Email not found." << endl;
		return;
	}

	cout << "=== Forgot Password ===" << endl;
	cout << "Enter nickname to verify your identity: ";
	string nickname;
	getline(cin, nickname);

	if (nickname == user1.nickname) {
		string newPassword;
		while (true) {
			cout << "Enter new password (at least 8 characters, including letters and numbers): ";
			getline(cin, newPassword);
			if (validatePassword(newPassword)) {
				updatePassword(email, newPassword);
				cout << "Password updated successfully." << endl;
				break;
			}
			else {
				cout << "Incorrect password format. Re-enter." << endl;
			}
		}
	}
	else {
		cout << "Nickname does not match. Cannot reset password." << endl;
	}

}

void updatePassword(const string& email, const string& newPassword) {
	User user1;
	if (loadUserProfile(email, user1)) {
		user1.salt = generateSalt();
		user1.passwordHash = hashPassword(newPassword, user1.salt);
		updateUserProfile(user1);
	}
}