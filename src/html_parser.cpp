#include "html_parser.h"
#include <regex>

std::string stripTag(const std::string &html, const std::string &tag) {
    std::regex pattern("<" + tag + "[\\s\\S]*?</" + tag + ">", std::regex::icase);
    return std::regex_replace(html, pattern, "");
}

std::string extractText(const std::string &html) {
    std::string cleaned = html;

    cleaned = stripTag(cleaned, "script");
    cleaned = stripTag(cleaned, "style");

    cleaned = std::regex_replace(cleaned, std::regex("<[^>]*>"), " ");
    cleaned = std::regex_replace(cleaned, std::regex("\\s+"), " ");

    return cleaned;
}
