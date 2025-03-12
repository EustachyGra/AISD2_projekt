#include <iostream>
#include <vector>
#include <iomanip>
using namespace std;

void printAdjacencyMatrix(vector<vector<int>> &adjMatrix)
{
    cout << "Macierz sasiedztwa:" << endl;
    for (auto &row : adjMatrix)
    {
        for (int val : row)
            cout << setw(3) << val << " ";

        cout << endl;
    }
    cout << endl;
}

void dfs(vector<vector<int>> &adjMatrix,vector <int> &l,vector <int> &f,int v)
{
    l[v]=1;
    int s;
    for(int i=0; i<adjMatrix[v].size();i++)
    {
        s =adjMatrix[v][i];
        if(l[s]!=1)
        {   f[s]=v;
            dfs(adjMatrix,l,f,s);
        }
    }
}
int main()
{
    vector <int>l(n);
    vector <int>f(n);
    for (int i=0; i<n; i++)
    {
        l[i]=0;
        f[i]=-1;
    }
    vector<vector<int>> adjacencyMatrix(vertices, vector<int>(vertices, 0));
    size_t w,u, v, weight,edges;
    cout<<"Podaj lczbe wierzcholkow: ";
    cin>>w;
    cout<<"Podaj liczbe krawedzi: ";
    cin>>edges;
    for (size_t i = 0; i < edges; i++)
    {
        cin >> u >> v >> weight;

        adjacencyMatrix[u][v] = (int)weight;

        adjacencyList[u].push_back({(int)v, (int)weight});
    }
    printAdjacencyMatrix(adjacencyMatrix);
    for (int i=0; i<n; i++) if (l[i]==0) dfs(adjacencyMatrix,l,f,0);
    for (int i=0; i<n; i++) cout<<l[i]<<" ";
    cout<<endl;
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
