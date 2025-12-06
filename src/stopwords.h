#pragma once
#include <unordered_set>
#include <string>

class StopwordLoader {
public:
    std::unordered_set<std::string> words;

    void load(const std::string& path);
    bool isStopword(const std::string& w) const;
};
