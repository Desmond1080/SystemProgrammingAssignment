#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>
#include "Event.h"
#include <vector>

class FileManager {
public:
    static void saveToJSON(const std::string& filename, const std::vector<Event>& events);
    static std::vector<Event> loadFromJSON(const std::string& filename);

    static void loadFromJSON(const std::string& filename, Event& manager);
};

#endif
