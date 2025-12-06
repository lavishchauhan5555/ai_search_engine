#ifndef HTML_PARSER_H
#define HTML_PARSER_H

#include <string>
#include <vector>

struct ParsedHTML {
    std::string title;
    std::string meta_description;
    std::vector<std::string> headings;
    std::vector<std::string> paragraphs;
    std::vector<std::string> links;
    std::string clean_text;
};

ParsedHTML parseHTML(const std::string &html);

#endif
