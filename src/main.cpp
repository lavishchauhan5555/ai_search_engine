#include "crawler.h"

int main() {
    Crawler crawler(200);      // crawl 200 pages
    crawler.addSeedUrl("https://example.com");
    crawler.start();
    return 0;
}
