#ifndef HTML_PARSER_H
#define HTML_PARSER_H

#include <string>

std::string extractText(const std::string &html);
std::string stripTag(const std::string &html, const std::string &tag);

#endif
