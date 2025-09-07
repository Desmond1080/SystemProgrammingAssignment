#ifndef REFUND_SUMMARY_H
#define REFUND_SUMMARY_H

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <iomanip>
#include <algorithm>
using namespace std;

struct Transaction {
    string category;
    string status;
    int quantity;
    double amount;
};

struct Summary {
    int totalCount = 0;
    int refundCount = 0;
    int totalQuantity = 0;
    int refundQuantity = 0;
    double totalAmount = 0.0;
    double refundAmount = 0.0;
};

// Function 
string toLowerStr(const string& s);
vector<Transaction> parseFile(const string& filename);
map<string, Summary> generateSummary(const vector<Transaction>& transactions);
void showRefundSummary(const map<string, Summary>& summary);
void showTop3Sales(const map<string, Summary>& summary);

#endif
