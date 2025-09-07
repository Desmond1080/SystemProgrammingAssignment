#include "payment.h"
#include <iostream>
using namespace std;

int main3() {
    Payment payment;
    
    // Take user input
    getGuestDetails(payment);
    
    while(true){
        cout << "Enter payment amount (RM): ";
        cin >> payment.amount;
        cin.ignore();

        if (validateAmount(payment.amount)) {
            break;
        }
        cout << "Invalid amount. Please enter a positive amount up to RM 1, 000, 000\n";
    }

    // Show summary
    showPaymentSummary(payment);

    //Choose method
    choosePaymentMethod(payment);

    //Process payment and set status
    processPayment(payment);

    //Generate receipt
    generateReceipt(payment);

    //Save to file
    savePaymentToFile(payment);
   
    refundPayment();

    return 0;
}