#include <iomanip>
#include <iostream>
#include <vector>
#include <limits.h>
#include <queue>
#include <cstring>
#include <stack>

using namespace std;

void printMatrix(vector<vector<int>> &adjMatrix){
    for(auto &row : adjMatrix){
        for (int val : row)
            cout << setw(3) << val << " ";

        cout << endl;
    }
    cout << endl;
}

bool dfs(vector<vector<int>> &rGraph, int s, int t, vector<int> &parent){
    int vertices = rGraph.size();
    vector<bool> visited(vertices, false);

    stack<int> stack;
    stack.push(s);
    visited[s] = true;
    parent[s] = -1;

    while(!stack.empty()){
        int u = stack.top();
        stack.pop();

        for(int v = 0; v < vertices; v++){
            if (!visited[v] && rGraph[u][v] > 0){
                stack.push(v);
                parent[v] = u;
                visited[v] = true;
                if (v == t)
                    return true;
            }
        }
    }

    return false;
}

long long fordFulkerson(vector<vector<int>> &graph, int s, int t){
    int vertices = graph.size();
    vector<vector<int>> rGraph(vertices, vector<int>(vertices, 0));

    for (int u = 0; u < vertices; u++)
        for (int v = 0; v < vertices; v++)
            rGraph[u][v] = graph[u][v];

    vector<int> parent(vertices);
    long long maxFlow = 0;

    while (dfs(rGraph, s, t, parent)){
        int pathFlow = INT_MAX;
        for (int v = t; v != s; v = parent[v]){
            int u = parent[v];
            pathFlow = min(pathFlow, rGraph[u][v]);
        }

        for (int v = t; v != s; v = parent[v]){
            int u = parent[v];
            rGraph[u][v] -= pathFlow;
            rGraph[v][u] += pathFlow;
        }
        maxFlow += pathFlow;
    }
    return maxFlow;
}

int main()
{
    size_t vertices, edges;
    cin >> vertices >> edges;
    vector<vector<int>> weightMatrix(vertices, vector<int>(vertices, 0));
    size_t u, v, weight;

    for (size_t i = 0; i < edges; i++){
        cin >> u >> v >> weight;
        weightMatrix[u - 1][v - 1] = (int)weight;
    }

    int source = 0, sink = vertices - 1;

    cout << "The maximum possible flow is " << fordFulkerson(weightMatrix, source, sink) << endl;

    return 0;
}

