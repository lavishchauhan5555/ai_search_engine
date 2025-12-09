#pragma once
#include <string>

class DocManager {
public:
    static void saveDocMapping(int docId, const std::string& url,
                               const std::string& outputPath);
};
