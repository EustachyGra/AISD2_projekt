#include <algorithm>
#include <climits>
#include <vector>
#include <queue>
#include <iostream>
#include "Line.hpp"
#include "Node.hpp"
#include "Functions.hpp"

bool BFS(std::vector<std::vector<size_t>>& adjMatrix, std::vector<size_t>& parent)
{
    fill(parent.begin(), parent.end(), -1);
    std::vector<bool> visited(adjMatrix[0].size(), false);
    std::queue<size_t> q;
    size_t s = 0, t = 1;
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

size_t edmondsKarp(std::vector<std::vector<size_t>> adjMatrix)
{
    std::vector<size_t> parent(adjMatrix[0].size());
    size_t s = 0, t = 1;
    size_t maxFlow = 0;

    while (BFS(adjMatrix, parent))
    {
        size_t pathFlow = INT_MAX;
        for (size_t v = t; v != s; v = parent[v])
        {
            size_t u = parent[v];
            pathFlow = std::min(pathFlow, (adjMatrix[u][v]));
        }

        for (size_t v = t; v != s; v = parent[v])
        {
            size_t u = parent[v];
            adjMatrix[u][v] -= pathFlow;
            adjMatrix[v][u] += pathFlow;
        }
        maxFlow += pathFlow;
    }
	printAdjMatrix(adjMatrix);
	std::cout << std::endl;
    return maxFlow;
}
