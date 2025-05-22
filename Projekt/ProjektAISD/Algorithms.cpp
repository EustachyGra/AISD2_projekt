#include "Algorithms.hpp"

bool BFS(std::vector<std::vector<size_t>>& adjMatrix, std::vector<size_t>& parent, size_t s, size_t t)
{
    fill(parent.begin(), parent.end(), -1);
    std::vector<bool> visited(adjMatrix[0].size(), false);
    std::queue<size_t> q;
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
                {
					/*if (lastAle != nullptr)
					{
						*lastAle = s;
						std::cout << "Last ale: " << *lastAle << std::endl;
					}*/
                    return true;
                }
            }
        }
    }
    return false;
}

size_t edmondsKarp(std::vector<std::vector<size_t>> adjMatrix)
{
    std::vector<size_t> parent(adjMatrix[0].size());
    std::vector<size_t> parent2(adjMatrix[0].size());
    size_t SuperSource = 0, SuperSink = 1, SuperAle = 2;
    size_t maxFlow = 0;
    while (BFS(adjMatrix, parent, 0, 2))
    {
        if (!BFS(adjMatrix, parent2, 2, 1)) break;
        size_t pathFlow = INT_MAX;
        for (size_t v = SuperAle; v != SuperSource; v = parent[v])
        {
            size_t u = parent[v];
            pathFlow = std::min(pathFlow, (adjMatrix[u][v]));
        }
        for (size_t v = SuperSink; v != SuperAle; v = parent2[v])
        {
            size_t u = parent2[v];
            pathFlow = std::min(pathFlow, (adjMatrix[u][v]));
        }

        for (size_t v = SuperAle; v != SuperSource; v = parent[v])
        {
            size_t u = parent[v];
            adjMatrix[u][v] -= pathFlow;
            adjMatrix[v][u] += pathFlow;
        }
        for (size_t v = SuperSink; v != SuperAle; v = parent2[v])
        {
            size_t u = parent2[v];
            adjMatrix[u][v] -= pathFlow;
            adjMatrix[v][u] += pathFlow;
        }
        maxFlow += pathFlow;
    }
	printAdjMatrix(adjMatrix);
	std::cout << std::endl;
    return maxFlow;
}
