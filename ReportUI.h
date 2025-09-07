#ifndef REPORT_UI_H
#define REPORT_UI_H

#include <vector>
#include "Event.h"
#include "ReportManager.h"

class ReportUI {
public:
    static void showReportMenu(const std::vector<Event>& events);
};

#endif

