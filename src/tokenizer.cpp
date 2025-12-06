#include "tokenizer.h"
#include "porter_stemmer.h"
#include <algorithm>
#include <cctype>

Tokenizer::Tokenizer(const StopwordLoader& sw)
    : stopwords(sw) {}

std::string Tokenizer::cleanString(const std::string& input) {
    std::string out;
    out.reserve(input.size());

    for (char c : input) {
        if (isalpha(c))
            out.push_back(std::tolower(c));
        else
            out.push_back(' ');
    }

    return out;
}

std::vector<std::string> Tokenizer::split(const std::string& text) {
    std::vector<std::string> tokens;
    std::string current;

    for (char c : text) {
        if (c == ' ') {
            if (!current.empty()) {
                tokens.push_back(current);
                current.clear();
            }
        } else {
            current += c;
        }
    }

    if (!current.empty()) tokens.push_back(current);

    return tokens;
}

std::vector<std::string> Tokenizer::process(const std::string& text) {
    std::string cleaned = cleanString(text);
    std::vector<std::string> words = split(cleaned);

    std::vector<std::string> finalTokens;

    for (auto& w : words) {
        if (w.empty()) continue;
        if (stopwords.isStopword(w)) continue;

        std::string stem = w;
        porter2::stem(stem);

        finalTokens.push_back(stem);
    }

    return finalTokens;
}
