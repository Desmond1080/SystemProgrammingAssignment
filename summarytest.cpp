#include "refund_summary.h"

// Convert string to lowercase
string toLowerStr(const string& s) {
    string result = s;
    transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

vector<Transaction> parseFile(const string& filename) {
    ifstream file(filename);
    vector<Transaction> transactions;
    Transaction current;

    string line;
    while (getline(file, line)) {
        if (line.find("Event Category") != string::npos) {
            current.category = line.substr(line.find(":") + 1);
            current.category.erase(0, current.category.find_first_not_of(" \t"));
        }
        else if (line.find("Status") != string::npos) {
            current.status = toLowerStr(line.substr(line.find(":") + 1));
            current.status.erase(0, current.status.find_first_not_of(" \t"));
        }
        else if (line.find("Quantity") != string::npos) {
            current.quantity = stoi(line.substr(line.find(":") + 1));
        }
        else if (line.find("Amount") != string::npos) {
            current.amount = stod(line.substr(line.find(":") + 1));
            // Completed record
            transactions.push_back(current);
        }
    }
    return transactions;
}

// Generate summary per event category
map<string, Summary> generateSummary(const vector<Transaction>& transactions) {
    map<string, Summary> summary;
    for (auto& t : transactions) {
        auto& s = summary[t.category]; // When is new category will auto set to 0
        s.totalCount++;
        s.totalQuantity += t.quantity;
        s.totalAmount += t.amount;

        if (t.status == "refunded") {
            s.refundCount++;
            s.refundQuantity += t.quantity;
            s.refundAmount += t.amount;
        }
    }
    return summary;
}

// Print Refund Summary Report
void showRefundSummary(const map<string, Summary>& summary) {
    const int width = 90;
    string title = "Refund Summary Report";
    int padding = (width - title.size()) / 2;

    cout << "\n" << string(width, '=') << "\n";
    cout << string(padding, ' ') << title << "\n";
    cout << string(width, '=') << "\n\n";

    cout << left << setw(20) << "Event Category"
        << right << setw(12) << "Refund Qty"
        << setw(15) << "Refund Amount"
        << setw(12) << "Sold Qty"
        << setw(15) << "Total Amount"
        << setw(12) << "Refund %" << endl;

    cout << string(width, '-') << endl;

    int totalRefundQty = 0, totalSoldQty = 0;
    double totalRefundAmt = 0.0, totalSalesAmt = 0.0;

    for (auto& entry : summary) {
        const auto& cat = entry.first; // get category name
        const auto& s = entry.second; // get summary data
        double refundPerc = (s.totalQuantity > 0) 
            ? (double)s.refundQuantity / s.totalQuantity * 100.0 // if true
            : 0.0; // if false

        cout << left << setw(20) << cat
            << right << setw(12) << s.refundQuantity
            << setw(15) << fixed << setprecision(2) << s.refundAmount
            << setw(12) << s.totalQuantity
            << setw(15) << s.totalAmount
            << setw(11) << fixed << setprecision(2) << refundPerc << endl;

        totalRefundQty += s.refundQuantity;
        totalSoldQty += s.totalQuantity;
        totalRefundAmt += s.refundAmount;
        totalSalesAmt += s.totalAmount;
    }

    cout << string(width, '-') << endl;
    double overallRefundPerc = (totalSoldQty > 0)
        ? (double)totalRefundQty / totalSoldQty * 100.0 // if true
        : 0.0; // if false
    cout << left << setw(20) << "TOTAL"
        << right << setw(12) << totalRefundQty
        << setw(15) << fixed << setprecision(2) << totalRefundAmt
        << setw(12) << totalSoldQty
        << setw(15) << totalSalesAmt
        << setw(11) << fixed << setprecision(2) << overallRefundPerc << endl;
}

// Show Top 3 events by sales (with quantity)
void showTop3Sales(const map<string, Summary>& summary) {
    struct EventSales {
        string category;
        int totalQty;
        double sales;
    };

    vector<EventSales> events;
    for (auto& entry : summary) {
        events.push_back({ entry.first,
                   entry.second.totalQuantity - entry.second.refundQuantity, // NET QTY
                   entry.second.totalAmount - entry.second.refundAmount }); // NET SALES 
    }

    sort(events.begin(), events.end(),
        [](const EventSales& a, const EventSales& b) // compare a and b
        { return a.totalQty > b.totalQty; }); // show result in descending order

    const int width = 60;
    string title = "Top 3 Events by Sales";
    int padding = (width - title.size()) / 2;

    cout << "\n" << string(width, '=') << "\n";
    cout << string(padding, ' ') << title << "\n";
    cout << string(width, '=') << "\n\n";

    cout << left << setw(20) << "Event Category"
        << right << setw(12) << "Sold Qty"
        << setw(15) << "Total Sales" << endl;
    cout << string(width, '-') << endl;

    
    for (int i = 0; i < 3; i++) {
        cout << left << setw(20) << events[i].category
            << right << setw(12) << events[i].totalQty
            << setw(15) << fixed << setprecision(2) << events[i].sales << endl;
    }
}
