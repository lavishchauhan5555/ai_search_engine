#ifndef CRAWLER_H
#define CRAWLER_H

#include <string>
#include <vector>
#include <queue>
#include <unordered_set>

class Crawler {
private:
    int maxPagesToCrawl;
    std::queue<std::string> urlQueue;
    std::unordered_set<std::string> visited;

public:
    Crawler(int maxPages);

    void addSeedUrl(const std::string& url);
    std::string fetchPage(const std::string& url);
    void savePage(const std::string& url, const std::string& html);

    // ✅ UPDATED SIGNATURE (must match .cpp)
    std::vector<std::string> extractUrls(const std::string& html,
                                         const std::string& baseUrl);

    std::string hashUrl(const std::string& url);
    void start();
};

#endif // CRAWLER_H
