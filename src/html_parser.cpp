#include "html_parser.h"
#include <regex>

std::string extractTag(const std::string &html, const std::string &tag) {
    std::regex rg("<" + tag + "[^>]*>([\\s\\S]*?)</" + tag + ">", std::regex::icase);
    std::smatch match;
    if (std::regex_search(html, match, rg)) {
        return match[1];
    }
    return "";
}

std::string extractMetaDescription(const std::string &html) {
    std::regex rg("<meta[^>]*name=[\"']description[\"'][^>]*content=[\"']([^\"']+)[\"'][^>]*>", std::regex::icase);
    std::smatch match;
    if (std::regex_search(html, match, rg)) {
        return match[1];
    }
    return "";
}

std::vector<std::string> extractAll(const std::string &html, const std::string &tag) {
    std::vector<std::string> result;
    std::regex rg("<" + tag + "[^>]*>([\\s\\S]*?)</" + tag + ">", std::regex::icase);

    auto begin = std::sregex_iterator(html.begin(), html.end(), rg);
    auto end   = std::sregex_iterator();

    for (auto it = begin; it != end; ++it) {
        result.push_back((*it)[1]);
    }
    return result;
}

std::vector<std::string> extractLinks(const std::string &html) {
    std::vector<std::string> result;
    std::regex rg("<a[^>]*href=[\"']([^\"']+)[\"'][^>]*>", std::regex::icase);

    auto begin = std::sregex_iterator(html.begin(), html.end(), rg);
    auto end   = std::sregex_iterator();

    for (auto it = begin; it != end; ++it) {
        result.push_back((*it)[1]);
    }
    return result;
}

std::string cleanText(const std::string &html) {
    std::string cleaned = html;

    cleaned = std::regex_replace(cleaned, std::regex("<script[\\s\\S]*?</script>", std::regex::icase), "");
    cleaned = std::regex_replace(cleaned, std::regex("<style[\\s\\S]*?</style>", std::regex::icase), "");

    cleaned = std::regex_replace(cleaned, std::regex("<[^>]*>"), " ");
    cleaned = std::regex_replace(cleaned, std::regex("\\s+"), " ");

    return cleaned;
}

ParsedHTML parseHTML(const std::string &html) {
    ParsedHTML p;

    p.title = extractTag(html, "title");
    p.meta_description = extractMetaDescription(html);

    for (int i = 1; i <= 6; i++) {
        auto h = extractAll(html, "h" + std::to_string(i));
        p.headings.insert(p.headings.end(), h.begin(), h.end());
    }

    p.paragraphs = extractAll(html, "p");
    p.links = extractLinks(html);
    p.clean_text = cleanText(html);

    return p;
}
