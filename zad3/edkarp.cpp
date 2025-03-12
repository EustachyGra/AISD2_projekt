#include <climits>
#include <iomanip>
#include <iostream>
#include <vector>
#include <queue>

using namespace std;

void printMatrix(vector<vector<int>> &adjMatrix)
{
    for (auto &row : adjMatrix)
    {
        for (int val : row)
            cout << setw(3) << val << " ";

        cout << endl;
    }
    cout << endl;
}

bool BFS(vector<vector<int>> &adjMatrix, vector<int> &parent, size_t s, size_t t)
{
    fill(parent.begin(), parent.end(), -1);
    vector<bool> visited(adjMatrix[0].size(), false);
    queue<size_t> q;
    q.push(s);
    visited[s] = true;
    while (!q.empty())
    {
        s = q.front();
        q.pop();
        for (size_t i = 0; i < adjMatrix[0].size(); i++)
        {
            if (adjMatrix[s][i] != 0 && !visited[i])
            {
                q.push(i);
                visited[i] = true;
                parent[i] = s;
                if (i == t)
                    return true;
            }
        }
    }
    return false;
}

int edmondsKarp(vector<vector<int>> adjMatrix, size_t s, size_t t)
{
    vector<int> parent(adjMatrix[0].size());
    int maxFlow = 0;
    while (BFS(adjMatrix, parent, s, t))
    {
        int pathFlow = INT_MAX;
        for (size_t v = t; v != s; v = parent[v])
        {
            size_t u = parent[v];
            pathFlow = min(pathFlow, adjMatrix[u][v]);
        }
        for (size_t v = t; v != s; v = parent[v])
        {
            size_t u = parent[v];
            adjMatrix[u][v] -= pathFlow;
            adjMatrix[v][u] += pathFlow;
        }
        maxFlow += pathFlow;
    }
    return maxFlow
}

int main()
{
    size_t vertices, edges;
    cin >> vertices >> edges;

    vector<vector<int>> adjacencyMatrix(vertices, vector<int>(vertices, 0));

    size_t u, v, weight;

    for (size_t i = 0; i < edges; i++)
    {
        cin >> u >> v >> weight;
        adjacencyMatrix[u - 1][v - 1] = (int)weight;
    }

    return 0;
}
