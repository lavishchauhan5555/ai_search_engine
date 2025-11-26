#include <curl/curl.h>
#include <fstream>
#include <iostream>
#include <string>
#include <filesystem>

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

int main() {
    // Absolute path for output
    std::filesystem::path folder = "D:/Projects/searchengine/data/raw";
    std::filesystem::create_directories(folder);

    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "https://example.com");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            std::cerr << "cURL error: " << curl_easy_strerror(res) << "\n";
        } else {
            std::cout << "Downloaded " << readBuffer.size() << " bytes.\n";

            std::ofstream out(folder / "page_1.html");
            if(out.is_open()) {
                out << readBuffer;
                out.close();
                std::cout << "Saved HTML successfully!\n";
            } else {
                std::cerr << "Failed to open file for writing!\n";
            }
        }

        curl_easy_cleanup(curl);
    } else {
        std::cerr << "Failed to initialize cURL!\n";
    }
    curl_global_cleanup();

    return 0;
}
