#include "payment.h"
#include "function.h"

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <ctime>
#include <sstream>

using namespace std;

string getCurrentDate() {
    time_t now = time(0);
    char buffer[80];
    struct tm timeinfo;
    localtime_s(&timeinfo, &now);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", &timeinfo);
    return string(buffer);
}

string getCurrentTime() {
    time_t now = time(0);
    char buffer[80];
    struct tm timeinfo;
    localtime_s(&timeinfo, &now);
    strftime(buffer, sizeof(buffer), "%H:%M:%S", &timeinfo);
    return string(buffer);
}

void getGuestDetails(Payment& p) {

    while (true) {
        cout << "\n===== Customer Details =====\n";
        cout << "Enter your name: ";
        getline(cin, p.name);

        if (validatePaymentName(p.name)) {
            break;
        }
        else {
            cout << "\nInvalid name. Name must be 2-50 characters with only letters and spaces.\n";
        }
    }

    while (true) {
        cout << "Enter phone number (within 10 numbers): ";
        getline(cin, p.phone);

        if (validateContactNumber(p.phone)) {
            break;
        }
        cout << "\nInvalid phone number. Phone number must be 10 digits starting with 01.\n";
        cout << "Example: 0123456789\n" << endl;
    }
}

void showPaymentSummary(Payment& p) {
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input buffer

    string input;
    char choice;
    bool confirmed = false;

    while (!confirmed){
        cout << "\n----- Payment Summary -----\n";
        cout << "Name        : " << p.name << endl;
        cout << "Email       : " << p.email << endl;
        cout << "Phone       : " << p.phone << endl;
        //cout << fixed << setprecision(2);
        //cout << "Amount      : RM " << p.amount << "\n" << endl;
        cout << "1. Comfirm\n";
        cout << "2. Edit details\n";
        cout << "Enter choice: ";
        getline(cin, input);

        if (input.length() == 1){
            choice = input[0];
            if (choice >= '1' && choice <= '2') {

                switch (choice) {
                case '1':
                    confirmed = true;
                    break;
                case '2':
                    getGuestDetails(p);
                    break;
                }
            }
            else {
                cout << "Invalid choice! Please enter 1 or 2.\n";
            } 
        }
        else {
            cout << "Invalid input! Please enter only ONE character only (1-2).\n";
        }  
    }
    
}

void choosePaymentMethod(Payment& p) {
    int choice;

    clearScreen();
    cout << "\nChoose Payment Method:\n";
    cout << "1. Credit/Debit Card\n";
    cout << "2. E-Wallet\n";
    cout << "Enter choice: ";
    cin >> choice;
    cin.ignore();

    switch (choice) {
    case 1: 
        p.method = "Credit/Debit Card"; break;
    case 2: 
        p.method = "E-Wallet"; break;
    default: 
        p.method = "Unknown";
        cout << "Invalid choice \n";
        break;
    }
}

void generateReceipt(const Payment& p) {
    cout << "\n============= RECEIPT =============\n";
    cout << "Customer Name  : " << p.name << endl;
    cout << "Phone Number   : " << p.phone << endl;
    cout << "Payment Method : " << p.method << endl;
    cout << "Payment Status : " << p.status << endl;
    cout << "Date           : " << p.date << endl;
    cout << "Time           : " << p.time << endl;
    cout << fixed << setprecision(2);
    cout << "Amount Paid    : RM " << p.amount << endl;

    if (p.status == "Completed") {
        cout << "Thank you for your payment!\n";
    }
    else if (p.status == "Cancelled") {
        cout << "Payment was cancelled.\n";
    }

    cout << "===================================\n";
}

bool processPayment(Payment& p) {
    string input;
    char choice;
    
    while (true) {
        cout << "\n----- Process Payment -----\n";
        cout << "1. Complete Payment\n";
        cout << "2. Cancel Payment\n";
        cout << "3. Change Payment Method\n";
        cout << "Enter choice: ";
        getline(cin, input);

        if (input.length() == 1) {
            choice = input[0];
            if (choice >= '1' && choice <= '3') {
                switch (choice) {
                case '1':
                    p.status = "Completed";
                    cout << "Payment completed successfully!\n";
                    p.date = getCurrentDate();
                    p.time = getCurrentTime();
                    generateReceipt(p);
                    savePaymentToFile(p);
                    // update payment record file
                    return true;
                case '2':
                    //p.status = "Cancelled";
                    cout << "Payment cancelled.\n";
                    //p.date = getCurrentDate();
                    //p.time = getCurrentTime();
                    return false;
                case '3':
                    cout << "\nChanging payment method...\n";
                    choosePaymentMethod(p);
                    break;
                }
            }
            else {
                cout << "Invalid choice! Please enter 1, 2, or 3.\n";
            }
        }
        else {
            cout << "Invalid input! Please enter only ONE character only (1-3).\n";
        }
            
    }
}

void savePaymentToFile(const Payment& p) {
    ofstream file("payment_records.txt", ios::app); 
    if (file.is_open()) {
        file << "Date  : " << p.date << "\n";
        file << "Time  : " << p.time << "\n";
        file << "Name  : " << p.name << "\n";
        file << "Phone : " << p.phone << "\n";
        file << "Method: " << p.method << "\n";
        file << "Status: " << p.status << "\n";
        file << fixed << setprecision(2);
        file << "Amount: RM " << p.amount << "\n";
        file << "-------------------------\n\n";
        file.close();
        cout << "\nPayment details saved to file.\n";
    }
    else {
        cout << "Error opening file.\n";
    }
}

vector<Payment> loadPaymentsFromFile() {
    string filename = "payment_records.txt";
    vector<Payment> payments;
    Payment p;
    string line;

    ifstream infile(filename);
    if (!infile.is_open()) {
        cout << "Error opening file: " << filename << endl;
        return payments; 
    }

    while (getline(infile, line)) {
        if (line.find("Date  : ") != string::npos) {
            p.date = line.substr(8);
        }
        else if (line.find("Time  : ") != string::npos) {
            p.time = line.substr(8);
        }
        else if (line.find("Name  : ") != string::npos) {
            p.name = line.substr(8);
        }
        else if (line.find("Phone : ") != string::npos) {
            p.phone = line.substr(8);
        }
        else if (line.find("Method: ") != string::npos) {
            p.method = line.substr(8);
        }
        else if (line.find("Status: ") != string::npos) {
            p.status = line.substr(8);
        }
        else if (line.find("Amount: RM ") != string::npos) {
            p.amount = stod(line.substr(10)); // convert string to double
        }
        else if (line.find("-------------------------") != string::npos) {
            payments.push_back(p);
            p = Payment(); // reset
        }
    }
    return payments;
}

void saveAllPayments(const vector<Payment>& payments) {
    string filename = "payment_records.txt";
    ofstream outfile(filename);
    for (const auto& p : payments) {
        outfile << "Date  : " << p.date << "\n";
        outfile << "Time  : " << p.time << "\n";
        outfile << "Name  : " << p.name << "\n";
        outfile << "Phone : " << p.phone << "\n";
        outfile << "Method: " << p.method << "\n";
        outfile << "Status: " << p.status << "\n";
        outfile << fixed << setprecision(2);
        outfile << "Amount: RM " << p.amount << "\n";
        outfile << "-------------------------\n\n";
    }
}

void refundPayment() {
    vector<Payment> payments = loadPaymentsFromFile();
    bool found = false;

    string searchName, searchPhone;

    while (true) {
        cout << "\n===== Refund Menu =====\n";
        cout << "Enter your name: ";
        getline(cin, searchName);

        if (validatePaymentName(searchName)) {
            break;
        }
        else {
            cout << "\nInvalid name? Name must be 2-50 characters with only letters and spaces.\n";
        }
    }

    while (true) {
        cout << "Enter phone number (within 10 numbers): ";
        getline(cin, searchPhone);

        if (validateContactNumber(searchPhone)) {
            break;
        }
        cout << "\nInvalid phone number! Must be 10 digits starting with 01.\n";
        cout << "Example: 0123456789\n" << endl;
    }

    for (auto& p : payments) {
        
        if (p.name == searchName && p.phone == searchPhone) {
            cout << "\n===== Payment Found =====\n";
            cout << "Date   : " << p.date << "\n";
            cout << "Time   : " << p.time << "\n";
            cout << "Name   : " << p.name << "\n";
            cout << "Phone  : " << p.phone << "\n";
            cout << "Amount : RM " << fixed << setprecision(2) << p.amount << "\n";
            cout << "Method : " << p.method << "\n";
            cout << "Status : " << p.status << "\n\n";

            if (p.status == "Cancelled") {
                cout << "This payment has already been cancelled.\n";
                return;
            }
            else if (p.status == "Refunded") {
                cout << "Payment has already been refunded.\n";
                return;
            }

            char confirm;
            while (true) {
                cout << "Do you want to refund this payment? (y/n): ";
                string input;
                getline(cin, input); 

                if (input.length() == 1) {          
                    confirm = tolower(input[0]);    
                    if (confirm == 'y' || confirm == 'n') {
                        break; 
                    }
                }
                cout << "Invalid input! Please enter 'y' or 'n'.\n";
            }


            if (confirm == 'y' || confirm == 'Y') {
                p.status = "Refunded";
                cout << "Refund processed successfully.\n";
            }
            else {
                cout << "Refund aborted by user.\n";
            }

            found = true;
            break; // stop after first exact match
        }
    }

    if (!found) {
        cout << "No payment found for the given name and phone number.\n";
    }

    saveAllPayments(payments);
}
