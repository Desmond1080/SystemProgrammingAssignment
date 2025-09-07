#pragma once

#ifndef PAYMENT_H
#define PAYMENT_H

#include <iostream>
#include <string>
#include <regex>
#include <vector>
using namespace std;

struct Payment {
    string name;
    string phone;
    string email;
    double amount;
    string method;
    string status;
    string date;
    string time;
    string timestamp;
    string eventName;
    vector<pair<int, int>> tickets; // {categoryIndex, quantity}
};

// Function declarations
string getCurrentDate();
string getCurrentTime();
void getGuestDetails(Payment& p);
void showPaymentSummary(Payment& p);
void choosePaymentMethod(Payment& p);
bool processPayment(Payment& p);
void generateReceipt(const Payment& p);
void savePaymentToFile(const Payment& p);
vector<Payment> loadPaymentsFromFile();
void saveAllPayments(const vector<Payment>& payments);
void refundPayment();


// Validation function declarations
bool validateContactNumber(const string& phone);
bool validatePaymentName(const string& name);
bool validateAmount(double amount);

#endif
