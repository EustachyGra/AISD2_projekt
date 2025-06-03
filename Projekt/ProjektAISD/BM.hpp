#ifndef BM
#define BM

#include <iostream> 
#include <fstream>  
#include <vector>  
#include <string>   
#include <algorithm> 
#include <unordered_map> 

std::unordered_map<char, int> przygotujMapeOstatnichWystapien(const std::string& wzorzec);
bool szukajWzorcaBoyerMoore(const std::string& tekst, const std::string& wzorzec, std::unordered_map<char, int> ostatnieWystapienie);
#endif // BM
