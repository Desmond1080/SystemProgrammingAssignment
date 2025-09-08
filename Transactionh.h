#ifndef TRANSACTION_H
#define TRANSACTION_H

#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <iomanip>
#include <algorithm>

using std::string; // Ensure 'string' is recognized

#include "Transactionh.h"
#include <fstream>
#include <sstream>

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

std::vector<Transaction> parseFile(const std::string& filename);

// Remove duplicate declaration of static string toLowerStr(const string& s);
string toLowerStr(const string& s);

#endif
