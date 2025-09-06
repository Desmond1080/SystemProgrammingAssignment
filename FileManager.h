#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>
#include "Event.h"
#include "EventManager.h"
#include <vector>

using namespace std;

class FileManager {
public:
    static void saveToJSON(const string& filename, const vector<Event>& events);
    static vector<Event> loadFromJSON(const string& filename);

    static void loadFromJSON(const string& filename, EventManager& manager);
};

#endif

