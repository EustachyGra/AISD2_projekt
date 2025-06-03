#include "BM.hpp"
std::unordered_map<char, int> przygotujMapeOstatnichWystapien(const std::string& wzorzec) {
    std::unordered_map<char, int> mapa; 
    for (int i = 0; i < wzorzec.size(); i++) {
        mapa[wzorzec[i]] = i;
    }
    return mapa;
}

bool szukajWzorcaBoyerMoore(const std::string& tekst, const std::string& wzorzec, std::unordered_map<char, int> ostatnieWystapienie) {

    if (wzorzec.empty() || tekst.empty() || wzorzec.size() > tekst.size()) {
        return false;
    }

    int dlugoscTekstu = tekst.size();
    int dlugoscWzorca = wzorzec.size();
    int indeksTekstu = 0;
    while (indeksTekstu <= dlugoscTekstu - dlugoscWzorca) {
        int indeksWzorca = dlugoscWzorca - 1;


        while (indeksWzorca >= 0 && wzorzec[indeksWzorca] == tekst[indeksTekstu + indeksWzorca]) {
            indeksWzorca--; 
        }

        if (indeksWzorca < 0) {
            return true;

        }
        else {

            char zlyZnak = tekst[indeksTekstu + indeksWzorca];
            int indeksOstatniegoWystapieniaWzorcu = ostatnieWystapienie.count(zlyZnak) ? ostatnieWystapienie[zlyZnak] : -1;
            int przesuniecie = indeksWzorca - indeksOstatniegoWystapieniaWzorcu;
            indeksTekstu += std::max(1, przesuniecie);
        }
    }

    return false;
}