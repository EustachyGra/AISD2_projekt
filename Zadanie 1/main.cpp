#include <algorithm>
#include <iomanip>
#include <iostream>
#include <vector>

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

void printAdjacencyList(vector<vector<pair<int, int>>> &adjList)
{
    cout << "Lista sasiadow:" << endl;
    for (int i = 0; i < adjList.size(); i++)
    {
        cout << i << ": ";
        for (auto &neighbor : adjList[i])
            cout << "(" << neighbor.first << ", " << neighbor.second << ") ";

        cout << endl;
    }
    cout << endl;
}

void printTwoArrays(vector<int> &indexArray, vector<pair<int, int>> &neighborArray)
{
    cout << "Dwie tablice:" << endl;
    cout << "Indeksy: ";
    for (int val : indexArray)
        cout << val << " ";

    cout << endl;
    cout << "Sasiedzi: ";
    for (auto &neighbor : neighborArray)
    {
        cout << "(" << neighbor.first << ", " << neighbor.second << ") ";
    }
    cout << endl;
}

int main()
{
    int vertices, edges;
    cin >> vertices >> edges;

    // Metoda macierzy sąsiedztwa
    vector<vector<int>> adjacencyMatrix(vertices, vector<int>(vertices, 0));

    // Metoda listy sąsiadów
    vector<vector<pair<int, int>>> adjacencyList(vertices);

    // Metoda dwóch tablic
    vector<int> indexArray(vertices, 0);  // Indeksy
    vector<pair<int, int>> neighborArray; // Sąsiedzi (para: sąsiad, waga)

    int u, v, weight;
    for (int i = 0; i < edges; i++)
    {
        cin >> u >> v >> weight;

        adjacencyMatrix[u][v] = weight;

        adjacencyList[u].push_back({v, weight});
    }

    int c = 0;
    for (int i = 0; i < vertices; i++)
    {
        indexArray[i] = c;
        for (auto &p : adjacencyList[i])
        {
            neighborArray.push_back(p);
            c++;
        }
    }

    printAdjacencyMatrix(adjacencyMatrix);
    printAdjacencyList(adjacencyList);
    printTwoArrays(indexArray, neighborArray);

    return 0;
}