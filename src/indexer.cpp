#include <fstream>
#include <filesystem>
#include <sstream>
#include <algorithm>

#include "indexer.h"

namespace fs = std::filesystem;

void Indexer::buildIndex(const std::string& tokensDir)
{
    int docId = 1;

    for (const auto& file : fs::directory_iterator(tokensDir))
    {
        std::ifstream in(file.path());
        std::vector<std::string> tokens;
        std::string token;

        while (in >> token)
            tokens.push_back(token);

        indexDocument(docId, tokens);
        docId++;
    }
}

void Indexer::indexDocument(int docId, const std::vector<std::string>& tokens)
{
    std::unordered_map<std::string, std::vector<int>> positionMap;

    for (int i = 0; i < tokens.size(); i++)
        positionMap[tokens[i]].push_back(i);

    for (auto& [term, posList] : positionMap)
    {
        Posting p;
        p.docId = docId;
        p.frequency = posList.size();
        p.positions = posList;

        invertedIndex[term].push_back(p);
    }
}

void Indexer::saveIndex(const std::string& indexDir)
{
    fs::create_directories(indexDir);

    std::string dictFile = indexDir + "/dict.txt";
    std::string postingsFile = indexDir + "/postings.bin";

    std::ofstream dict(dictFile);
    std::ofstream postings(postingsFile, std::ios::binary);

    long offset = 0;

    for (auto& [term, postingList] : invertedIndex)
    {
        std::sort(postingList.begin(), postingList.end(),
            [](auto& a, auto& b) { return a.docId < b.docId; });

        long start = offset;

        for (auto& p : postingList)
        {
            postings.write((char*)&p.docId, sizeof(int));
            postings.write((char*)&p.frequency, sizeof(int));

            int count = p.positions.size();
            postings.write((char*)&count, sizeof(int));
            postings.write((char*)p.positions.data(), sizeof(int) * count);

            offset += sizeof(int) * (3 + count);
        }

        long end = offset;
        dict << term << " " << start << " " << end << " " << postingList.size() << "\n";
    }
}
