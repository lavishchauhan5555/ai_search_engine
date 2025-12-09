#include "crawler.h"
#include "html_parser.h"

#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

// ===== Week 3 includes =====
#include "tokenizer.h"
#include "stopwords.h"
#include "indexer.h"

namespace fs = std::filesystem;

int main()
{
    try
    {
        // ============================================================
        // STEP 1: CRAWLING
        // ============================================================
        std::cout << "[INFO] Starting crawler...\n";

        Crawler crawler(2);
        crawler.addSeedUrl("https://example.com");
        crawler.start(); // blocking

        std::cout << "[INFO] Crawling finished.\n";

        // ============================================================
        // STEP 2: HTML PARSING + CLEAN TEXT EXTRACTION
        // ============================================================
        std::string rawPath = "D:/Projects/searchengine/data/raw";
        std::string cleanPath = "D:/Projects/searchengine/data/clean";
        std::string tokenPath = "D:/Projects/searchengine/data/tokens";

        // Ensure output directories exist
        fs::create_directories(cleanPath);
        fs::create_directories(tokenPath);

        std::cout << "[INFO] Starting text extraction from: " << rawPath << "\n";

        size_t fileCount = 0;

        for (const auto &entry : fs::directory_iterator(rawPath))
        {
            if (!entry.is_regular_file())
                continue;
            if (entry.path().extension() != ".html")
                continue;

            // Read HTML file
            std::ifstream fin(entry.path(), std::ios::binary);
            if (!fin)
            {
                std::cerr << "[WARN] Failed to open: " << entry.path() << "\n";
                continue;
            }

            std::string html(
                (std::istreambuf_iterator<char>(fin)),
                std::istreambuf_iterator<char>());

            ParsedHTML parsed = parseHTML(html);

            // Build formatted clean output
            std::ostringstream out;

            out << "Title: " << parsed.title << "\n\n";
            out << "Meta Description: " << parsed.meta_description << "\n\n";

            out << "Headings:\n";
            for (const auto &h : parsed.headings)
                out << "- " << h << "\n";
            out << "\n";

            out << "Paragraphs:\n";
            for (const auto &p : parsed.paragraphs)
                out << "- " << p << "\n";
            out << "\n";

            out << "Links:\n";
            for (const auto &l : parsed.links)
                out << "- " << l << "\n";
            out << "\n";

            out << "Clean Full Text:\n"
                << parsed.clean_text << "\n";

            std::string finalText = out.str();

            // Save clean file
            fs::path outPath = fs::path(cleanPath) / (entry.path().stem().string() + ".txt");
            std::ofstream fout(outPath, std::ios::binary);
            if (!fout)
            {
                std::cerr << "[WARN] Failed to write clean file: " << outPath << "\n";
                continue;
            }
            fout << finalText;
            fout.close();

            std::cout << "[OK] Extracted: " << outPath.string() << "\n";
            ++fileCount;
        }

        std::cout << "[INFO] Text extraction finished. Processed " << fileCount << " files.\n";

        // ============================================================
        // STEP 3 (WEEK 3): TOKENIZATION + STEMMING + STOPWORD REMOVAL
        // ============================================================

        std::cout << "\n[INFO] Starting tokenization...\n";

        // Load stopwords
        StopwordLoader stopwords;
        stopwords.load("config/stopwords.txt");

        // Create tokenizer
        Tokenizer tokenizer(stopwords);

        size_t tokenFileCount = 0;

        // Loop clean text directory
        for (const auto &entry : fs::directory_iterator(cleanPath))
        {
            if (!entry.is_regular_file())
                continue;
            if (entry.path().extension() != ".txt")
                continue;

            std::ifstream fin(entry.path());
            if (!fin)
            {
                std::cerr << "[WARN] Failed to read clean file: " << entry.path() << "\n";
                continue;
            }

            // read full clean text
            std::stringstream buffer;
            buffer << fin.rdbuf();
            std::string text = buffer.str();

            // tokenize
            std::vector<std::string> tokens = tokenizer.process(text);

            // save token file
            // Define base directory for tokens
            std::string TOKEN_BASE_DIR = "D:/Projects/searchengine/data/tokens";

            // Make sure directory exists
            fs::create_directories(TOKEN_BASE_DIR);

            // Build final output file path
            fs::path tokenOutPath = fs::path(TOKEN_BASE_DIR) / (entry.path().stem().string() + ".tokens.txt");

            // Save tokens
            std::ofstream fout(tokenOutPath);
            if (!fout)
            {
                std::cerr << "[WARN] Failed to write tokens: " << tokenOutPath << "\n";
                continue;
            }

            for (auto &t : tokens)
            {
                fout << t << "\n";
            }

            fout.close();

            std::cout << "[TOKENS] Saved: " << tokenOutPath.string()
                      << " (" << tokens.size() << " tokens)\n";

            ++tokenFileCount;
        }

        std::cout << "[INFO] Tokenization complete. Processed " << tokenFileCount << " files.\n";
         // indexer fouth week task


   Indexer idx;

    idx.buildIndex("D:/Projects/searchengine/data/tokens");
    idx.saveIndex("D:/Projects/searchengine/data/index");

    std::cout << "Index built successfully.\n";


    }
    catch (const std::exception &ex)
    {
        std::cerr << "[ERROR] Exception: " << ex.what() << "\n";
        return 1;
    }
    catch (...)
    {
        std::cerr << "[ERROR] Unknown exception.\n";
        return 1;
    }
  

   
    return 0;
}
