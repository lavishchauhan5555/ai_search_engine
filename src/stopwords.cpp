#include "stopwords.h"
#include <fstream>
#include <algorithm>

void StopwordLoader::load(const std::string& path) {
    std::ifstream fin(path);
    if (!fin.is_open()) return;

    std::string w;
    while (fin >> w) {
        std::transform(w.begin(), w.end(), w.begin(), ::tolower);
        words.insert(w);
    }
}

bool StopwordLoader::isStopword(const std::string& w) const {
    return words.find(w) != words.end();
}
