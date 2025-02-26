#include<iostream>
#include<vector>

using namespace std;

struct kraw{
    int v1;
    int v2;
    int w;
    bool di;

    kraw(int t1, int t2, int tw=1, bool td=false)
    {
        v1=t1; //wierzcholek 1
        v2=t2; // wierzcholek 2
        w=tw;  // waga
        di = td; // czy skierowane (jesli tak to idzie z v1 do v2)
    }

};

void macierz(int nV,const vector<kraw>& E, int** tab)
{
    for(auto it = E.begin(); it != E.end(); it++)
    {
        tab[(*it).v1][(*it).v2] = (*it).w;
        tab[(*it).v2][(*it).v1] = (*it).w;
    }

    for(int i = 0; i < nV; i++){
        for(int j = 0; j < nV; j++)
            cout<<tab[i][j]<<" ";
        cout<<endl;
    }
}

int main()
{
    int nV;
    int nE;
    cin>>nV>>nE; // ilosc wierzchlokow i ilosc krawedzi;

    //wczytywanie i zapisywanie krawedzi
    vector<kraw> E;
    for(int i = 0; i < nE; i++)
    {
        int tv1, tv2, tw;
        cin>>tv1>>tv2>>tw;
        kraw tmp(tv1,tv2,tw);
        E.push_back(tmp);
    }

    //tablica macierzy sasiedztwa
    cout<<endl<<"Macierz sasiedztwa"<<endl;
    int** tab= new int*[nV];
    for(int i = 0; i < nV; i++)
        tab[i] = new int[nV]{0};
    cout<<endl;

    macierz(nV, E, tab);

}
