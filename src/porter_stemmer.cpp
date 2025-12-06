#include "porter_stemmer.h"
#include <algorithm>

namespace porter2 {

static bool isVowel(char c) {
    return c=='a' || c=='e' || c=='i' || c=='o' || c=='u';
}

static bool measure(const std::string& w, int start, int end) {
    bool vowelSeen = false;
    int m = 0;

    for (int i = start; i <= end; i++) {
        if (isVowel(w[i])) {
            vowelSeen = true;
        } else if (vowelSeen) {
            m++;
            vowelSeen = false;
        }
    }
    return m > 0;
}

static bool endsWith(const std::string& w, const std::string& suffix) {
    if (w.size() < suffix.size()) return false;
    return w.compare(w.size()-suffix.size(), suffix.size(), suffix) == 0;
}

static void replaceEnd(std::string& w, const std::string& suffix, const std::string& repl) {
    if (endsWith(w, suffix))
        w.replace(w.size()-suffix.size(), suffix.size(), repl);
}

void stem(std::string& w) {
    if (w.size() <= 2)
        return;

    // Step 1a
    if (endsWith(w, "sses")) w.replace(w.size()-4, 4, "ss");
    else if (endsWith(w, "ies")) w.replace(w.size()-3, 3, "i");
    else if (endsWith(w, "ss")) {}
    else if (endsWith(w, "s")) w.erase(w.size()-1);

    // Step 1b
    if (endsWith(w, "eed")) {
        if (measure(w, 0, w.size()-4))
            w.erase(w.size()-1);
    }
    else if (endsWith(w, "ed")) {
        std::string stem = w.substr(0, w.size()-2);
        for (char c : stem) {
            if (isVowel(c)) {
                w = stem;
                break;
            }
        }
    }
    else if (endsWith(w, "ing")) {
        std::string stem = w.substr(0, w.size()-3);
        for (char c : stem) {
            if (isVowel(c)) {
                w = stem;
                break;
            }
        }
    }

    // Step 1c
    if (endsWith(w, "y")) {
        if (w.size() >= 2 && !isVowel(w[w.size()-2]))
            w[w.size()-1] = 'i';
    }

    // Step 2
    replaceEnd(w, "ational", "ate");
    replaceEnd(w, "tional", "tion");
    replaceEnd(w, "enci", "ence");
    replaceEnd(w, "anci", "ance");
    replaceEnd(w, "izer", "ize");
    replaceEnd(w, "bli", "ble");
    replaceEnd(w, "alli", "al");
    replaceEnd(w, "entli", "ent");
    replaceEnd(w, "eli", "e");
    replaceEnd(w, "ousli", "ous");
    replaceEnd(w, "ization", "ize");
    replaceEnd(w, "ation", "ate");
    replaceEnd(w, "ator", "ate");
    replaceEnd(w, "alism", "al");
    replaceEnd(w, "iveness", "ive");
    replaceEnd(w, "fulness", "ful");
    replaceEnd(w, "ousness", "ous");
    replaceEnd(w, "aliti", "al");
    replaceEnd(w, "iviti", "ive");
    replaceEnd(w, "biliti", "ble");

    // Step 3
    replaceEnd(w, "icate", "ic");
    replaceEnd(w, "ative", "");
    replaceEnd(w, "alize", "al");
    replaceEnd(w, "iciti", "ic");
    replaceEnd(w, "ical", "ic");
    replaceEnd(w, "ful", "");
    replaceEnd(w, "ness", "");

    // Step 4
    if (endsWith(w, "al") || endsWith(w, "ance") || endsWith(w, "ence") ||
        endsWith(w, "er") || endsWith(w, "ic") || endsWith(w, "able") ||
        endsWith(w, "ible") || endsWith(w, "ant") || endsWith(w, "ement") ||
        endsWith(w, "ment") || endsWith(w, "ent") || endsWith(w, "ion") ||
        endsWith(w, "ou") || endsWith(w, "ism") || endsWith(w, "ate") ||
        endsWith(w, "iti") || endsWith(w, "ous") || endsWith(w, "ive") ||
        endsWith(w, "ize")) 
    {
        w.erase(w.size()-2);
    }

    // Step 5
    if (endsWith(w, "e") && measure(w, 0, w.size()-2))
        w.erase(w.size()-1);

    if (endsWith(w, "l") && w.size() >= 2 && w[w.size()-1]=='l' && w[w.size()-2]=='l')
        w.erase(w.size()-1);
}

}
