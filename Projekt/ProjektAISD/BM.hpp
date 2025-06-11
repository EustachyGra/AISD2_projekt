#ifndef BM
#define BM

#include <iostream> 
#include <fstream>  
#include <vector>  
#include <string>   
#include <algorithm> 
#include <unordered_map> 

std::unordered_map<char, int> buildBadChar(const std::string& pattern);
std::vector<int> buildGoodSuffix(const std::string& pattern);
bool boyerMoore(const std::string& text, const std::string& pattern, std::unordered_map<char, int> badChar, std::vector<int> goodSuffix);
#endif // BM
