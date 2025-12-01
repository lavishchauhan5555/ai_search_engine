#include "crawler.h"
#include <curl/curl.h>
#include <fstream>
#include <iostream>
#include <regex>
#include <chrono>
#include <ctime>
#include <thread>
#include <filesystem>
#include <unordered_set>

/* ----------------- ALLOWED DOMAIN ----------------- */
std::string allowedDomain = "example.com";

/* Check if URL belongs to allowed domain */
bool isSameDomain(const std::string& url, const std::string& domain) {
    return url.find(domain) != std::string::npos;
}

/* Join Base URL + Relative or Protocol-relative link */
std::string joinURL(const std::string& base, std::string link) {
    if (link.empty()) return "";

    // Already absolute
    if (link.find("http://") == 0 || link.find("https://") == 0)
        return link;

    // Protocol-relative: //example.com
    if (link.rfind("//", 0) == 0)
        return "https:" + link;

    // Starts with slash → base + "/page"
    if (link[0] == '/')
        return base + link;

    // Relative: "page.html" → base + "/page.html"
    return base + "/" + link;
}

/* ----------------- CONSTRUCTOR ----------------- */
Crawler::Crawler(int maxPages) {
    this->maxPagesToCrawl = maxPages;
}

/* ----------------- cURL WRITE CALLBACK ----------------- */
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

/* ----------------- ADD SEED URL ----------------- */
void Crawler::addSeedUrl(const std::string& url) {
    urlQueue.push(url);
}

/* ----------------- FETCH PAGE ----------------- */
std::string Crawler::fetchPage(const std::string& url) {
    CURL* curl = curl_easy_init();
    std::string buffer = "";

    if (!curl) return buffer;

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "C++ WebCrawler 1.0");

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cout << "❌ Failed: " << url << "\n";
    }

    curl_easy_cleanup(curl);
    return buffer;
}

/* ----------------- SAVE HTML TO FILE ----------------- */
void Crawler::savePage(const std::string& url, const std::string& html) {
    std::string basePath = "D:/Projects/searchengine/data/raw";
    std::filesystem::create_directories(basePath);

    std::string filename = basePath + "/" + hashUrl(url) + ".html";

    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "❌ Failed to write file: " << filename << "\n";
        return;
    }

    auto now = std::chrono::system_clock::now();
    std::time_t timeNow = std::chrono::system_clock::to_time_t(now);

    file << "<!-- URL: " << url << " -->\n";
    file << "<!-- Timestamp: " << std::ctime(&timeNow) << " -->\n";
    file << html;

    file.close();

    std::cout << "✔ Saved: " << filename << "\n";
}

/* ----------------- EXTRACT ALL LINKS ----------------- */
bool endsWith(const std::string& str, const std::string& suffix) {
    if (suffix.size() > str.size()) return false;
    return std::equal(suffix.rbegin(), suffix.rend(), str.rbegin());
}

std::vector<std::string> Crawler::extractUrls(const std::string& html, const std::string& baseUrl) {
    const int MAX_URLS_PER_PAGE = 30;   // SET LIMIT HERE

    std::unordered_set<std::string> seen;  // remove duplicates per page
    std::vector<std::string> urls;

    std::regex linkRegex(R"(href=["']([^"']+)["'])");
    auto begin = std::sregex_iterator(html.begin(), html.end(), linkRegex);
    auto end   = std::sregex_iterator();

    for (auto it = begin; it != end; ++it) {
        if (urls.size() >= MAX_URLS_PER_PAGE) break; // ⬅️ LIMIT PER PAGE

        std::string link = (*it)[1].str();

        // Skip empty or useless links
        if (link.empty()) continue;
        if (link[0] == '#') continue;

        // Skip unwanted schemes
        if (link.rfind("mailto:", 0) == 0) continue;
        if (link.rfind("tel:", 0) == 0) continue;
        if (link.rfind("javascript:", 0) == 0) continue;

        // Skip images, CSS, JS
        if (endsWith(link, ".jpg") || endsWith(link, ".jpeg") ||
    endsWith(link, ".png") || endsWith(link, ".gif")  ||
    endsWith(link, ".svg") || endsWith(link, ".css")  ||
    endsWith(link, ".js"))
{
    continue;
}


        // Normalize → absolute URL
        link = joinURL(baseUrl, link);

        // Remove duplicates
        if (seen.count(link)) continue;
        seen.insert(link);

        // Limit to allowed domain
        // if (!isSameDomain(link,allowedDomain)) continue;

        urls.push_back(link);
    }

    return urls;
}


/* ----------------- HASH URL FOR FILE NAME ----------------- */
std::string Crawler::hashUrl(const std::string& url) {
    return std::to_string(std::hash<std::string>{}(url));
}

/* ----------------- START BFS CRAWLING ----------------- */
void Crawler::start() {
    int count = 0;

    while (!urlQueue.empty() && count < maxPagesToCrawl) {
        std::string currentUrl = urlQueue.front();
        urlQueue.pop();

        // Already visited
        if (visited.count(currentUrl)) 
            continue;

        visited.insert(currentUrl);

        std::cout << "Crawling (" << count + 1 << "): " << currentUrl << "\n";

        std::string html = fetchPage(currentUrl);
        if (html.empty()) continue;

        savePage(currentUrl, html);

        // Extract URLs (normalized + dedup + domain-limited)
        std::vector<std::string> urls = extractUrls(html, currentUrl);

        for (const auto& link : urls) {
            if (!visited.count(link)) {
                urlQueue.push(link);
            }
        }

        count++;

        std::this_thread::sleep_for(std::chrono::milliseconds(500)); // polite crawling delay
    }

    std::cout << "\nCrawling finished. Pages crawled: " << count << "\n";
}
