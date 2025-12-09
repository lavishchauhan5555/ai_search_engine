#pragma once
#include <vector>

struct Posting {
    int docId;
    int frequency;
    std::vector<int> positions;
};
