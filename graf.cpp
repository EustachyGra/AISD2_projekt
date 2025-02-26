#include<iostream>

using namespace std;
struct kraw{
    int v1;
    int v2;
    int w;
    bool di;
    edge(int t1, int t2, int tw=1, bool td=false)
    {
        v1=t1; //wierzcholek 1
        v2=t2; // wierzcholek 2
        w=tw;  // waga
        di = td // czy skierowane (jesli tak to idzie z v1 do v2)
    }

};


int main()
{

}
