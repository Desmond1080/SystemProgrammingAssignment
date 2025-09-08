// Fix for ambiguous operator= error when assigning to Transaction struct members

#include "Transactionh.h"

using namespace std;

// Convert string to lowercase
string toLowerStr(const string& s) {
    string result = s;
    transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

vector<Transaction> parseFile(const string& filename) {
    ifstream file(filename);
    vector<Transaction> transactions;
    if (!file) {
        cerr << "Error: cannot open '" << filename << "' for reading.\n";
        return transactions;
    }

    Transaction current;
    string line;

    auto ltrim = [](string& s) {
        s.erase(s.begin(), find_if(s.begin(), s.end(), [](unsigned char ch) { return !isspace(ch); }));
        };

    while (getline(file, line)) {
        if (line.find("Event Category") != string::npos) {
            string value = line.substr(line.find(":") + 1);
            ltrim(value);
            current.category = value; // Use a temporary string variable
        }
        else if (line.find("Status") != string::npos) {
            string value = line.substr(line.find(":") + 1);
            ltrim(value);
            current.status = toLowerStr(value); // Use a temporary string variable
        }
        else if (line.find("Quantity") != string::npos) {
            string value = line.substr(line.find(":") + 1);
            ltrim(value);
            current.quantity = stoi(value); // Use a temporary string variable
        }
        else if (line.find("Amount") != string::npos) {
            string value = line.substr(line.find(":") + 1);
            ltrim(value);
            current.amount = stod(value); // Use a temporary string variable
            // Completed record
            transactions.push_back(current);
        }
    }
    return transactions;
}
