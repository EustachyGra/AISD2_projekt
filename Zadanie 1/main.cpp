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
    for (size_t i = 0; i < adjList.size(); i++)
    {
        cout << i << ": ";
        for (auto &neighbor : adjList[i])
            cout << "(" << neighbor.first << ", " << neighbor.second << ") ";

        cout << endl;
    }
    cout << endl;
}

void printTwoArrays(vector<int> &indexArray, vector<pair<int, int>> &neighborArray,
                    vector<vector<pair<int, int>>> &adjList)
{
    int c = 0;
    for (size_t i = 0; i < adjList.size(); i++)
    {
        indexArray[i] = c;
        for (auto &p : adjList[i])
        {
            neighborArray.push_back(p);
            c++;
        }
    }

    cout << "Dwie tablice:" << endl;
    cout << "Indeksy: ";
    for (auto val : indexArray)
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
    size_t vertices, edges;
    cin >> vertices >> edges;

    // Metoda macierzy sąsiedztwa
    vector<vector<int>> adjacencyMatrix(vertices, vector<int>(vertices, 0));

    // Metoda listy sąsiadów
    vector<vector<pair<int, int>>> adjacencyList(vertices);

    // Metoda dwóch tablic
    vector<int> indexArray(vertices, 0);  // Indeksy
    vector<pair<int, int>> neighborArray; // Sąsiedzi (para: sąsiad, waga)

    size_t u, v, weight;
    for (size_t i = 0; i < edges; i++)
    {
        cin >> u >> v >> weight;

        adjacencyMatrix[u][v] = (int)weight;

        adjacencyList[u].push_back({(int)v, (int)weight});
    }

    printAdjacencyMatrix(adjacencyMatrix);
    printAdjacencyList(adjacencyList);
    printTwoArrays(indexArray, neighborArray, adjacencyList);

    return 0;
}