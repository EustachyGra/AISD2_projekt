#include <algorithm>
#include <climits>
#include <iostream>
#include <vector>

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

void printShortestPath(vector<vector<int>> &optDecision, size_t v, size_t u)
{
    size_t v2 = u;
    vector<int> path;
    while (optDecision[v][v2] != (int)u)
    {
        path.push_back((int)v2);
        v2 = (size_t)optDecision[v][v2];
    }
    path.push_back((int)v2);

    reverse(path.begin(), path.end());
    for (auto &i : path)
        cout << i + 1 << " ";
    cout << endl;
}

int main()
{
    size_t vertices, edges;
    cin >> vertices >> edges;

    vector<vector<int>> adjacencyMatrix(vertices, vector<int>(vertices, INT_MAX));

    size_t u, v, weight;
    // for (int i = 0; i < vertices; i++)
    //     adjacencyMatrix[i][i] = 0;

    for (size_t i = 0; i < edges; i++)
    {
        cin >> u >> v >> weight;
        adjacencyMatrix[u - 1][v - 1] = (int)weight;
    }

    vector<vector<int>> optimalDecision(vertices, vector<int>(vertices));
    for (size_t i = 0; i < vertices; i++)
        for (size_t j = 0; j < vertices; j++)
        {
            if (adjacencyMatrix[i][j] == INT_MAX)
                optimalDecision[i][j] = -1;
            else
                optimalDecision[i][j] = (int)i;
        }

    floydWarshall(adjacencyMatrix, optimalDecision);

    size_t k;
    cin >> v >> u;
    cin >> k;
    cout << endl;
    cout << "Najkrotsza droga miedzy wierzcholkami " << v << ", " << u << ":" << endl;
    cout << adjacencyMatrix[v - 1][u - 1] << " ";
    printShortestPath(optimalDecision, v - 1, u - 1);
    cout << "Najkrotszy cykl zawierajacy wierzcholek " << v << ":" << endl;
    cout << adjacencyMatrix[k - 1][k - 1] << " " << v << " ";
    printShortestPath(optimalDecision, k - 1, k - 1);

    return 0;
}
