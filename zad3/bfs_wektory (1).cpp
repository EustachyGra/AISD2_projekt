#include <iostream>
#include <vector>
#include <queue>
using namespace std;

void bfs(vector<int> *tab, bool *a, int *f,int s)
{
    queue <int> Q; //kolejka
    Q.push(s); //wstawianie na koniec kolejki przetwarzanego wierzcho³ka
    a[s]=true; //wierzcho³ek s jest odwiedzony
    while(Q.size()!=0) //dopóki koljeka nie jest pusta
    {
        s=Q.front(); //pobranie wierzcho³ka z poczatku kolejki
        Q.pop();//usunięcie pobranego wierzcho³ka z pocz¹tku kolejki
        for(int j=0; j<tab[s].size(); ++j) //dla wszystkich sasiadów wierzcho³ka s
        {
            int sasiad=tab[s][j];
            if(a[sasiad]==false) //jeœli s¹siad nie by³ odwiedzony
            {
                Q.push(sasiad); //wstawienie sasiada na koniec kolejki
                a[sasiad]=true; //sąsiad uzyskuje status odwiedzonego
                f[sasiad]=s;
            }
        }
    }
}
int main()
{
    int n,m,x,y;//n-liczba wierzcholkow, m-liczba krawedzi
    cin>>n;
    cin>>m;
    bool l[n]; //tablica odwiedzen
    int f[n]; //tablica ojców
    for (int i=0; i<n; i++)
      { l[i]=false;
        f[i]=-1;
       }//zerowanie tablicy odwiedzen, ustalenie poczatkowych wartoœci tablicy ojców na -1
    vector <int> tab[n]; //tablica n pustych wektorów
    for(int i=0;i<m;i++)
    {
        cin>>x;//pierwszy wierzcho³ek krawêdzi
        cin>>y;//drugi wierzcho³ek krawedzi
        tab[x].push_back(y);
        tab[y].push_back(x);
    }
    int i=0;
    bfs(tab,l,f,i); //wywołanie przeszukiwania od wierzchołka x
    for (int i=0; i<n; i++) cout<<f[i]<<" ";
    return 0;
}
/*
7 8
2 5
5 3
3 2
2 0
0 1
2 4
4 6
3 0
*/
