#ifndef FILE_COMPRESSION_HPP
#define FILE_COMPRESSION_HPP
#include <bitset>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>

using namespace std;

struct HuffmanNode
{
    char data;
    unsigned freq;
    HuffmanNode* left, * right;

    // Konstruktor
    HuffmanNode(char dt, unsigned frq)
    {
        this->data = dt;
        this->freq = frq;
        left = right = nullptr;
    }

    // Destruktor
    ~HuffmanNode()
    {
        delete left;
        delete right;
    }
};

// Komparator dla kolejki priorytetowej (aby stworzyæ min-kopiec)
struct Compare
{
	bool operator()(HuffmanNode* l, HuffmanNode* r)
	{
		return l->freq > r->freq; // Wêz³y z mniejsz¹ czêstotliwoœci¹ maj¹ wy¿szy priorytet
	}
};

HuffmanNode* buildHuffmanTree(unordered_map<char, unsigned>& freqMap);
void generateCodes(HuffmanNode* root, string str, unordered_map<char, string>& huffmanCode);
void serializeTree(HuffmanNode* root, ofstream& out);
HuffmanNode* deserializeTree(ifstream& in);
void compressString(const string& inputData, const string& outputFile);
void compress(string& inputFile, string& outputFile);
string decompressString(const string& inputFile);
void decompress(const string& inputFile, const string& outputFile);

#endif