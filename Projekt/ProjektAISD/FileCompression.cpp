#include "FileCompression.hpp"

using namespace std;

double compressionRatio;

// Funkcja do budowania drzewa Huffmana
HuffmanNode* buildHuffmanTree(unordered_map<char, unsigned>& freqMap)
{
    priority_queue<HuffmanNode*, vector<HuffmanNode*>, Compare> minHeap;

    // Tworzy w�ze� li�cia dla ka�dego znaku i dodaje go do min-kopca
    for (auto& pair : freqMap)
        minHeap.push(new HuffmanNode(pair.first, pair.second));

    // ��czy w�z�y, a� w kopcu zostanie tylko jeden w�ze� (korze� drzewa Huffmana)
    while (minHeap.size() != 1)
    {
        HuffmanNode* left = minHeap.top();
        minHeap.pop(); // Wyci�ga w�ze� z najmniejsz� cz�stotliwo�ci�
        HuffmanNode* right = minHeap.top();
        minHeap.pop(); // Wyci�ga nast�pny w�ze� z najmniejsz� cz�stotliwo�ci�

        // Tworzy nowy w�ze� wewn�trzny z sum� cz�stotliwo�ci dw�ch w�z��w
        HuffmanNode* top = new HuffmanNode('\0', left->freq + right->freq);
        top->left = left;
        top->right = right;
        minHeap.push(top); // Dodaje nowy w�ze� z powrotem do kopca
    }

    return minHeap.top(); // Zwraca korze� drzewa Huffmana
}

// Funkcja do generowania kod�w Huffmana
void generateCodes(HuffmanNode* root, string str, unordered_map<char, string>& huffmanCode)
{
    if (!root)
        return; // Je�li w�ze� jest pusty, zako�cz

    // Je�li w�ze� jest li�ciem, zapisz jego kod Huffmana
    if (!root->left && !root->right)
        huffmanCode[root->data] = str;

    // Przejd� lewe poddrzewo i dodaj '0' do kodu
    generateCodes(root->left, str + "0", huffmanCode);
    // Przejd� prawe poddrzewo i dodaj '1' do kodu
    generateCodes(root->right, str + "1", huffmanCode);
}

// Funkcja do serializacji drzewa Huffmana (zapisywania do pliku)
void serializeTree(HuffmanNode* root, ofstream& out)
{
    if (!root)
    {
        out.put(0); // 0 do oznaczenia pustego w�z�a
        return;
    }

    out.put(1);                      // 1 do oznaczenia niepustego w�z�a
    out.put(root->data);             // Zapisz znak w w�le
    serializeTree(root->left, out);  // Serializuj lewe poddrzewo
    serializeTree(root->right, out); // Serializuj prawe poddrzewo
}

// Funkcja do deserializacji drzewa Huffmana (odtwarzania z pliku)
HuffmanNode* deserializeTree(ifstream& in)
{
    char bit;
    in.get(bit); // Read the bit to check if the node is empty

    if (bit == 0)
        return nullptr; // Return nullptr if the node is empty

    char data;
    in.get(data);
    HuffmanNode* root = new HuffmanNode(data, 0);
    root->left = deserializeTree(in);  // Assign left child
    root->right = deserializeTree(in); // Assign right child
    return root;
}

// Funkcja do kompresji danych z ci�gu znak�w zamiast pliku
void compressString(const string& inputData, const string& outputFile)
{
    size_t origSize = inputData.size();

    ofstream out(outputFile, ios::binary);

    // 1. Oblicz cz�stotliwo�� ka�dego znaku w ci�gu wej�ciowym
    unordered_map<char, unsigned> freqMap;
    for (char ch : inputData)
        freqMap[ch]++;

    // 2. Zbuduj drzewo Huffmana
    HuffmanNode* root = buildHuffmanTree(freqMap);

    // 3. Wygeneruj kody Huffmana dla ka�dego znaku
    unordered_map<char, string> huffmanCode;
    generateCodes(root, "", huffmanCode);

    // 4. Serializuj drzewo Huffmana i zapisz je do pliku wyj�ciowego
    serializeTree(root, out);

    // 5. Zakoduj dane wej�ciowe za pomoc� kod�w Huffmana
    string encodedStr;
    for (char ch : inputData)
        encodedStr += huffmanCode[ch];

    // 6. Dodaj dope�nienie, aby d�ugo�� zakodowanego ci�gu by�a wielokrotno�ci� 8
    unsigned padding = 8 - (encodedStr.length() % 8);
    if (padding == 8) padding = 0;
    out.put(padding); // Zapisz warto�� dope�nienia do pliku wyj�ciowego

    for (unsigned i = 0; i < padding; i++)
        encodedStr += '0'; // Dodaj bity dope�nienia

    // 7. Zapisz zakodowany ci�g do pliku wyj�ciowego jako bajty
    for (unsigned int i = 0; i < encodedStr.length(); i += 8)
    {
        bitset<8> bits(encodedStr.substr(i, 8));
        out.put(static_cast<unsigned char>(bits.to_ulong()));
    }

    out.seekp(0, std::ios::end);
    size_t compSize = out.tellp();

    // Oblicz stopie� kompresji
    compressionRatio = origSize == 0 ? 0.0 : 100.0 * (origSize - compSize) / origSize;

    out.close();
    delete root;
}

// Funkcja do kompresji pliku
void compress(string& inputFile, string& outputFile)
{
    ifstream in(inputFile, ios::binary);
    if (!in)
    {
        cerr << "Nieprawidlowy plik wejsciowy.";
        exit(-1);
    }

    in.seekg(0, ios::end);
    size_t origSize = in.tellg();
    in.seekg(0, ios::beg);

    ofstream out(outputFile, ios::binary);

    // 1. Oblicz cz�stotliwo�� ka�dego znaku w pliku wej�ciowym
    unordered_map<char, unsigned> freqMap;
    char ch;
    while (in.get(ch))
        freqMap[ch]++;

    // 2. Zbuduj drzewo Huffmana
    HuffmanNode* root = buildHuffmanTree(freqMap);

    // 3. Wygeneruj kody Huffmana dla ka�dego znaku
    unordered_map<char, string> huffmanCode;
    generateCodes(root, "", huffmanCode);

    // 4. Serializuj drzewo Huffmana i zapisz je do pliku wyj�ciowego
    serializeTree(root, out);

    // 5. Zakoduj plik wej�ciowy za pomoc� kod�w Huffmana
    in.clear();
    in.seekg(0); // Zresetuj wska�nik pliku na pocz�tek

    string encodedStr;
    while (in.get(ch))
        encodedStr += huffmanCode[ch]; // Dodaj kod Huffmana dla ka�dego znaku

    // 6. Dodaj dope�nienie, aby d�ugo�� zakodowanego ci�gu by�a wielokrotno�ci� 8
    unsigned padding = 8 - (encodedStr.length() % 8);
    out.put(padding); // Zapisz warto�� dope�nienia do pliku wyj�ciowego

    for (unsigned i = 0; i < padding; i++)
        encodedStr += '0'; // Dodaj bity dope�nienia

    // 7. Zapisz zakodowany ci�g do pliku wyj�ciowego jako bajty
    for (unsigned int i = 0; i < encodedStr.length(); i += 8)
    {
        bitset<8> bits(encodedStr.substr(i, 8)); // Konwertuj 8 bit�w na bajt
        out.put(bits.to_ulong());                // Zapisz bajt do pliku wyj�ciowego
    }

    out.seekp(0, std::ios::end);
    size_t compSize = out.tellp();

    // Oblicz stopie� kompresji
    compressionRatio = 100.0 * (origSize - compSize) / origSize;

    in.close();
    out.close();
    delete root;
}

// Funkcja do dekompresji danych z pliku do ci�gu znak�w
string decompressString(const string& inputFile)
{
    ifstream in(inputFile, ios::binary);
    if (!in)
    {
        cerr << "Nieprawidlowy plik wejsciowy.";
        exit(-1);
    }

    // 1. Deserializuj drzewo Huffmana z pliku wej�ciowego
    HuffmanNode* root = deserializeTree(in);

    // 2. Przeczytaj warto�� dope�nienia
    char padding;
    in.get(padding);

    // 3. Przeczytaj zakodowany ci�g z pliku wej�ciowego
    string encodedStr;
    char byte;
    while (in.get(byte))
    {
        encodedStr += bitset<8>(static_cast<unsigned char>(byte)).to_string();
    }

    // 4. Usu� bity dope�nienia
    if (padding > 0 && encodedStr.length() >= static_cast<size_t>(padding))
        encodedStr = encodedStr.substr(0, encodedStr.length() - padding);

    // 5. Zdekoduj zakodowany ci�g za pomoc� drzewa Huffmana
    string output;
    HuffmanNode* curr = root;
    for (char bit : encodedStr)
    {
        curr = bit == '0' ? curr->left : curr->right;
        if (!curr->left && !curr->right)
        {
            output += curr->data;
            curr = root;
        }
    }

    in.close();
    delete root;
    return output;
}

// Funkcja do dekompresji pliku
void decompress(const string& inputFile, const string& outputFile)
{
    ifstream in(inputFile, ios::binary);
    if (!in)
    {
        cerr << "Nieprawidlowy plik wejsciowy.";
        exit(-1);
    }
    ofstream out(outputFile, ios::binary);

    // 1. Deserializuj drzewo Huffmana z pliku wej�ciowego
    HuffmanNode* root = deserializeTree(in);

    // 2. Przeczytaj warto�� dope�nienia
    char padding;
    in.get(padding);

    // 3. Przeczytaj zakodowany ci�g z pliku wej�ciowego
    string encodedStr;
    char byte;
    while (in.get(byte))
    {
        encodedStr += bitset<8>(byte).to_string(); // Konwertuj ka�dy bajt na 8 bit�w
    }

    // 4. Usu� bity dope�nienia
    encodedStr = encodedStr.substr(0, encodedStr.length() - padding);

    // 5. Zdekoduj zakodowany ci�g za pomoc� drzewa Huffmana
    HuffmanNode* curr = root;
    for (char bit : encodedStr)
    {
        curr = bit == '0' ? curr->left : curr->right; // Przejd� drzewo w zale�no�ci od bitu
        if (!curr->left && !curr->right)              // Je�li osi�gni�to li��
        {
            out.put(curr->data); // Zapisz znak do pliku wyj�ciowego
            curr = root;         // Ustaw korze� dla nast�pnego znaku
        }
    }

    in.close();
    out.close();
    delete root;
}
