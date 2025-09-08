#ifndef REPORT_UI_H
#define REPORT_UI_H

#include <vector>
#include <string>
#include <map>
#include <ctime>
#include "Event.h"
#include "Transactionh.h"

class ReportUI {
public:
    // Main Menus
    static void showReportMenu(const std::vector<Event>& events);
    static void showRefundReportMenu(const std::vector<Transaction>& transactions);

    // Refund/summary helpers
    static void showRefundSummary(const std::map<std::string, Summary>& summary);
    static void showTop3Sales(const std::map<std::string, Summary>& summary);
    static std::map<std::string, Summary> generateSummary(const std::vector<Transaction>& transactions);

    // Utilities
    static std::time_t getDateInput(const std::string& prompt);
};

#endif
