#include "crawler.h"
#include "html_parser.h"

#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

namespace fs = std::filesystem;

int main() {
    try {
        // ========== STEP 1: CRAWLING ==========
        std::cout << "[INFO] Starting crawler...\n";

        Crawler crawler(10);                     // number of threads/pages
        crawler.addSeedUrl("https://example.com");
        crawler.start();                         // must block until crawling finishes

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

            // Parse HTML with advanced parser
            ParsedHTML parsed = parseHTML(html);

            // ========== Build final structured output format ==========
            std::ostringstream out;

            out << "Title: " << parsed.title << "\n\n";

            out << "Meta Description: " << parsed.meta_description << "\n\n";

            out << "Headings:\n";
            for (const auto &h : parsed.headings) {
                out << "- " << h << "\n";
            }
            out << "\n";

            out << "Paragraphs:\n";
            for (const auto &p : parsed.paragraphs) {
                out << "- " << p << "\n";
            }
            out << "\n";

            out << "Links:\n";
            for (const auto &l : parsed.links) {
                out << "- " << l << "\n";
            }
            out << "\n";

            out << "Clean Full Text:\n" << parsed.clean_text << "\n";

            std::string finalText = out.str();

            // ========== Save cleaned output ==========
            fs::path outPath = fs::path(cleanPath) / entry.path().stem();
            outPath += ".txt";

            std::ofstream fout(outPath, std::ios::binary);
            if (!fout) {
                std::cerr << "[WARN] Failed to write: " << outPath << "\n";
                continue;
            }

            fout << finalText;
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
