#include "BM.hpp"
std::unordered_map<char, int> buildBadChar(const std::string& pattern) {
    std::unordered_map<char, int> badChar;
    for (int i = 0; i < (int)pattern.size(); ++i) {
        badChar[pattern[i]] = i;
    }
    return badChar;
}

// Preprocessing good-suffix heuristic
std::vector<int> buildGoodSuffix(const std::string& pattern) {
    int m = pattern.size();
    std::vector<int> suffix(m + 1);
    std::vector<int> goodShift(m + 1);

    // Step 1: compute suffix lengths
    suffix[m] = m + 1;
    int g = m;
    int f = 0;
    for (int i = m; i > 0; --i) {
        while (g <= m && pattern[i - 1] != pattern[g - 1]) {
            g = suffix[g];
        }
        g--;
        suffix[i - 1] = g;
    }

    // Step 2: initialize shifts to m
    std::fill(goodShift.begin(), goodShift.end(), m);

    // Step 3: fill shifts based on suffixes
    for (int i = 0; i <= m; ++i) {
        int j = m - suffix[i];
        if (goodShift[j] > i - suffix[i]) {
            goodShift[j] = i - suffix[i];
        }
    }
    return goodShift;
}

// Boyer-Moore search using both heuristics
bool boyerMoore(const std::string& text, const std::string& pattern, std::unordered_map<char, int> badChar, std::vector<int> goodSuffix) {
    if (pattern.empty() || text.size() < pattern.size()) return false;

    int n = text.size();
    int m = pattern.size();


    int s = 0; // shift of the pattern with respect to text
    while (s <= n - m) {
        int j = m - 1;
        while (j >= 0 && pattern[j] == text[s + j]) {
            --j;
        }
        if (j < 0) {
            return true;  // match found at shift s
        }
        else {
            char bad = text[s + j];
            int bcShift = j - (badChar.count(bad) ? badChar[bad] : -1);
            int gsShift = goodSuffix[j + 1];
            s += std::max(1, std::max(bcShift, gsShift));
        }
    }
    return false;
}
