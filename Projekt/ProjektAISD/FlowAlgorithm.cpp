#include "FlowAlgorithm.hpp"

Edge::Edge(size_t to, size_t rev, size_t cap, long cost) {
	this->to = to;
	this->rev = rev;
	this->cap = cap;
	this->cost = cost;
}

FlowAlgorithm::FlowAlgorithm(size_t n) {
	this->n = n;
	size_t tmp_n = 2 * n + 2; // 2 warstwy + super source + super sink
	graph.resize(tmp_n + 2);
	dist.resize(tmp_n, std::numeric_limits<size_t>::max());
	parent.resize(tmp_n, -1);
	line.resize(tmp_n, -1);
	potential.resize(tmp_n, 0);
	source = tmp_n - 2; // Super source
	sink = tmp_n - 1; // Super sink

}
void FlowAlgorithm::AddEdge(size_t u, size_t v, size_t cap, long cost) {
	Edge og = Edge(v, graph[v].size(), cap, cost); // krawedz orginalna
	graph[u].push_back(og);
	Edge rev = Edge(u, graph[u].size() - 1, 0, -cost); // krawedz odwrtona dla grafu rezydualnego
	graph[v].push_back(rev);
}



void FlowAlgorithm::MakeGraph(std::vector<Line>& lines, std::vector<Node>& nodes) {
	for (size_t i = 0; i < lines.size(); i++) {
		size_t u, v, cap, cost;
		u = lines[i].startNode;
		v = lines[i].endNode;
		cap = lines[i].getCapacity();
		cost = lines[i].GetCost();
		AddEdge(u, v, cap, long(cost));
		AddEdge(u + n, v + n, cap, long(cost)); // Dodajemy krawedz do drugiej warstwy grafu rezydualnego
		AddEdge(v, u, cap, long(cost)); // Droga jest dwukierunkowa wiec dodajemy tez od drugiej strony
		AddEdge(v + n, u + n, cap, long(cost));
	}
	for (size_t i = 0; i < nodes.size(); i++)
	{
		NodeType type = nodes[i].getType();

		if (type == NodeType::Crossroad)
		{
			continue;
		}
		if (type == NodeType::Alehouse)
		{
			// Przejscie z pierwszej warstwy do drugiej przez browar
			AddEdge(i, i + n, nodes[i].capacity, 0);
			continue;
		}
		if (type == NodeType::Tavern)
		{
			// Ujscie z tawern do super sink
			AddEdge(i + n, sink, nodes[i].capacity, 0);
			continue;
		}
		if (type == NodeType::Farm)
		{
			// Ujscie z super source do farm
			AddEdge(source, i, nodes[i].capacity, 0);
			continue;
		}
	}
}

std::pair<size_t, size_t> FlowAlgorithm::Calculate()
{
	std::pair<size_t, size_t> output = { 0, 0 }; // MaxFLow, MinCost
	while (true) { 	//dziala dopoki nie zostanie przerwany w trakcie szukania najtanszej sciezki, dzieki temu znajdujemy maksymalny przeplyw
		// Dijkstra na grafie z potencja³ami
		std::priority_queue<std::pair<size_t, size_t>, std::vector<std::pair<size_t, size_t>>, std::greater<>> pq; 
		//wierzcholek z najmniejsza dotychczasowa odlegloscia bedzie sprawdzany jako pierwszy
		fill(dist.begin(), dist.end(), max);
		dist[source] = 0;
		pq.push({ 0, source });
		while (!pq.empty()) {
			std::pair<size_t, size_t> tmp = pq.top();
			size_t d = tmp.first;
			size_t u = tmp.second;
			pq.pop();
			if (d != dist[u]) 
				continue; //jesli warunek spelniony to znaczy ze znaleziono juz do tego wierzcholka tansza droge

			for (size_t i = 0; i < graph[u].size(); i++) {
				Edge& e = graph[u][i];
				if (e.cap > 0) {
					size_t nd = dist[u] + e.cost + potential[u] - potential[e.to]; // dzieki temu nie ma ujemnych kosztow, dzieki czemu mozna korzystac z Dijkstry
					if (nd < dist[e.to]) {
						dist[e.to] = nd;
						parent[e.to] = u; // poprzedni wierzcholek na sciezce
						line[e.to] = i; // indeks krawedzi, ktorej uzyto, w liscie sasiedztwa
						pq.push({ nd, e.to });
					}
				}
			}
		}

		if (dist[sink] == max)
			break; // brak sciezki, koniec algorytmu


		//Aktualizacja potencjalow
		for (size_t i = 0; i < potential.size(); i++) {
			if (dist[i] < max)
				potential[i] += dist[i];
		}


		// Znalezienie maksymalnego przep³ywu na znalezionej œcie¿ce
		size_t flow = max;
		size_t v = sink;
		while (v != source) {
			size_t u = parent[v];
			Edge& e = graph[u][line[v]];
			// graph[parent[v][line[u]] -> parent przeszedl przez krawedz line[u] w grafie do v
			flow = std::min(flow, e.cap);
			v = u;
		}

		v = sink;
		while (v != source) {
			size_t u = parent[v];
			Edge& e = graph[u][line[v]];
			e.cap -= flow;
			graph[v][e.rev].cap += flow;

			v = u;
		}

		output.first += flow; // dodajemy przeplyw do maksymalnego przeplywu
		output.second += flow * potential[sink]; // dodajemy koszt przeplywu do minimalnego kosztu
		}
	std::cout << "Max Flow: " << output.first << ", Min Cost: " << output.second << std::endl;
	return output;
}

void FlowAlgorithm::printGraph() {
	for (size_t i = 0; i < graph.size(); i++) {
		for (size_t j = 0; j < graph[i].size(); j++) {
			Edge& e = graph[i][j];

			std::cout << i << " " << e.to << " " << e.cap << " " << e.cost << std::endl;

		}
	}
}

size_t FlowAlgorithm::checkLine(size_t u, size_t v)
{
	size_t output = 0;
	for (size_t i = 0; i < graph[u].size(); i++)
	{
		//std::cout << "Checking edge from " << u << " to " << v << ": "<<i<<std::endl;
		Edge& e = graph[u][i];
		//std::cout << "Edge to: " << e.to << ", cost: " << e.cost << ", cap: " << e.cap << std::endl;
		if (e.to == v && e.cost<0)
			output += e.cap; // zwraca ile przeplywa przez linie w warstwie 1
	}
	for (size_t i = 0; i < graph[u+n].size(); i++)
	{
		//std::cout << "Checking edge from " << u << " to " << v << ": " << i << std::endl;
		Edge& e = graph[u+n][i];
		//std::cout << "Edge to: " << e.to << ", cost: " << e.cost << ", cap: " << e.cap << std::endl;
		if (e.to == v+n && e.cost < 0)
			output += e.cap; // zwraca ile przeplywa przez linie w warstwie 1
	}
	return output;
}

size_t FlowAlgorithm::checkBuilding(size_t u, size_t v)
{
	std::cout << source << " " << sink << std::endl;
	std::cout << "Checking building from " << u << " to " << v << std::endl;
	for (size_t i = 0; i < graph[u].size(); i++)
	{
		Edge& e = graph[u][i];
		if (e.to == v)
			return graph[e.to][e.rev].cap;
	}
}
