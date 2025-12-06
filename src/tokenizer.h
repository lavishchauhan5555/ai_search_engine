#pragma once
#include <string>
#include <vector>
#include "stopwords.h"

class Tokenizer {
public:
    Tokenizer(const StopwordLoader& swLoader);

    std::vector<std::string> process(const std::string& text);

private:
    std::string cleanString(const std::string& input);
    std::vector<std::string> split(const std::string& text);
    const StopwordLoader& stopwords;
};
