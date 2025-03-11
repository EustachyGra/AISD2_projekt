#include <algorithm>
#include <climits>
#include <iostream>
#include <vector>
#include<iomanip>
using namespace std;

void floydWarshall(vector<vector<int>> &adjMatrix, vector<vector<int>> &optDecision)
{
    for (size_t k = 0; k < adjMatrix.size(); k++)
        for (size_t i = 0; i < adjMatrix.size(); i++)
            for (size_t j = 0; j < adjMatrix.size(); j++)
            {
                if ((adjMatrix[i][k] == INT_MAX) || (adjMatrix[k][j] == INT_MAX))
                    continue;
                if (adjMatrix[i][j] > adjMatrix[i][k] + adjMatrix[k][j])
                {
                    adjMatrix[i][j] = adjMatrix[i][k] + adjMatrix[k][j];
                    optDecision[i][j] = optDecision[k][j];
                }
            }
}
void printMatrix(vector<vector<int>> &adjMatrix)
{
   // cout << "Macierz odleglosci:" << endl;
    for (auto &row : adjMatrix)
    {
        for (int val : row)
            cout << setw(3) << val << " ";

        cout << endl;
    }
    cout << endl;
}

void printShortestPath(vector<vector<int>> &optDecision, int v, int u)
{
    int v2 = u;
    vector<int> path;
    while (optDecision[v][v2] != u && optDecision[v][v2]!=v)
    {
        path.push_back(v2);
        v2 = optDecision[v][v2];
    }
    path.push_back(v2);
    cout<< v+1<<" ";
    reverse(path.begin(), path.end());
    for (auto &i : path)
        cout << i + 1 << " ";
    cout << endl;
}

int main()
{
    size_t vertices, edges;
    cin >> vertices >> edges;

    vector<vector<int>> distanceMatrix(vertices, vector<int>(vertices, INT_MAX));

    size_t u, v, weight;
    // for (int i = 0; i < vertices; i++)
    //     adjacencyMatrix[i][i] = 0;

    for (size_t i = 0; i < edges; i++)
    {
        cin >> u >> v >> weight;
        distanceMatrix[u - 1][v - 1] = (int)weight;
    }

    vector<vector<int>> optimalDecision(vertices, vector<int>(vertices));
    for (size_t i = 0; i < vertices; i++)
        for (size_t j = 0; j < vertices; j++)
        {
            if (distanceMatrix[i][j] == INT_MAX)
                optimalDecision[i][j] = -1;
            else
                optimalDecision[i][j] = (int)i;
        }

    floydWarshall(distanceMatrix, optimalDecision);
    cout<<endl;
    printMatrix(distanceMatrix);
    size_t k;
    cin >> v >> u;
    cin >> k;
    cout << endl;
    //cout << "Najkrotsza droga miedzy wierzcholkami " << v << ", " << u << ":" << endl;
    cout << distanceMatrix[v - 1][u - 1] << " ";
    printShortestPath(optimalDecision, v - 1, u - 1);
    //cout << "Najkrotszy cykl zawierajacy wierzcholek " << k << ":" << endl;
    cout << distanceMatrix[k - 1][k - 1] << " ";
    printShortestPath(optimalDecision, k - 1, k - 1);

    return 0;
}
