#pragma once
#include <string>
#include <unordered_map>
#include <vector>

#include "posting.h"

class Indexer {
public:
    void buildIndex(const std::string& tokensDir);
    void saveIndex(const std::string& indexDir);

private:
    std::unordered_map<std::string, std::vector<Posting>> invertedIndex;

    void indexDocument(int docId, const std::vector<std::string>& tokens);
};
