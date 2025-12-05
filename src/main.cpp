#include "crawler.h"
#include "html_parser.h"

#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>

namespace fs = std::filesystem;

int main() {
    try {
        // ========== STEP 1: CRAWLING ==========
        std::cout << "[INFO] Starting crawler...\n";

        Crawler crawler(10);                     // 10 = max pages / threads (depending on your implementation)
        crawler.addSeedUrl("https://example.com");
        crawler.start();                         // Make sure this blocks until crawling is finished

        std::cout << "[INFO] Crawling finished.\n";

        // ========== STEP 2: TEXT EXTRACTION ==========
        std::string rawPath   = "D:/Projects/searchengine/data/raw";
        std::string cleanPath = "D:/Projects/searchengine/data/clean";

        // Ensure clean directory exists
        fs::create_directories(cleanPath);

        std::cout << "[INFO] Starting text extraction from: " << rawPath << "\n";

        size_t fileCount = 0;

        for (const auto& entry : fs::directory_iterator(rawPath)) {
            if (!entry.is_regular_file()) continue;
            if (entry.path().extension() != ".html") continue;

            // Read raw HTML file
            std::ifstream fin(entry.path(), std::ios::binary);
            if (!fin) {
                std::cerr << "[WARN] Failed to open: " << entry.path() << "\n";
                continue;
            }

            std::string html(
                (std::istreambuf_iterator<char>(fin)),
                 std::istreambuf_iterator<char>()
            );

            // Extract clean text using your html_parser logic
            std::string text = extractText(html);

            // Save cleaned output
            fs::path outPath = fs::path(cleanPath) / entry.path().stem();
            outPath += ".txt";

            std::ofstream fout(outPath, std::ios::binary);
            if (!fout) {
                std::cerr << "[WARN] Failed to write: " << outPath << "\n";
                continue;
            }

            fout << text;
            fout.close();

            std::cout << "[OK] Extracted: " << outPath.string() << "\n";
            ++fileCount;
        }

        std::cout << "[INFO] Text extraction finished. Processed " << fileCount << " files.\n";
    }
    catch (const std::exception& ex) {
        std::cerr << "[ERROR] Exception: " << ex.what() << "\n";
        return 1;
    }
    catch (...) {
        std::cerr << "[ERROR] Unknown exception.\n";
        return 1;
    }

    return 0;
}
