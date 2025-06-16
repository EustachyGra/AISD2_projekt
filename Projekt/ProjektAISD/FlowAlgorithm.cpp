#include "FlowAlgorithm.hpp"

Edge::Edge() {}

void Edge::setEdge(size_t to, size_t rev, size_t cap, long cost, bool isPath, size_t mirEdge, size_t mirIndx)
{
	this->to = to;
	this->rev = rev;
	this->cap = cap;
	this->cost = cost;
	this->isPath = isPath;
	this->mirEdge = mirEdge;
	this->mirIndx = mirIndx;
}

FlowAlgorithm::FlowAlgorithm(size_t n) {
	this->n = n;
	size_t tmp_n = 2 * n + 2; // 2 warstwy + super source + super sink
	graph.resize(tmp_n + 2);
	dist.resize(tmp_n, max_size_t);
	parent.resize(tmp_n, -1);
	used.resize(tmp_n, false);
	line.resize(tmp_n, -1);
	potential.resize(tmp_n, max_size_t);
	source = tmp_n - 2; // Super source
	sink = tmp_n - 1; // Super sink

}
void FlowAlgorithm::AddEdgeBuilding(size_t u, size_t v, size_t cap, long cost) {
	Edge tmp;
	graph[u].push_back(tmp);
	graph[v].push_back(tmp);
	graph[u].back().setEdge(v, graph[v].size() - 1, cap, 0); // krawedz orginalna
	graph[v].back().setEdge(u, graph[u].size() - 1, 0, 0); // krawedz odwrtona dla grafu rezydualnego
}
void FlowAlgorithm::AddEdgeLine(size_t u, size_t v, size_t cap, long cost, size_t n) {
	Edge tmp;
	graph[u].push_back(tmp);
	graph[v].push_back(tmp);
	graph[u + n].push_back(tmp);
	graph[v + n].push_back(tmp);

	graph[u].back().setEdge(v, graph[v].size() - 1, cap, cost, true, u + n, graph[u+n].size()-1); // krawedz orginalna
	graph[v].back().setEdge(u, graph[u].size() - 1, 0, -cost, true, v + n, graph[v+n].size() - 1); // krawedz odwrtona dla grafu rezydualnego
	graph[u + n].back().setEdge(v + n, graph[v + n].size() - 1, cap, cost, true,u, graph[u].size() - 1); // krawedz orginalna w drugiej warstwie
	graph[v + n].back().setEdge(u + n, graph[u + n].size() - 1, 0, -cost, true, v,graph[v].size() - 1); // krawedz odwrtona dla grafu rezydualnego w drugiej warstwie
}



void FlowAlgorithm::MakeGraph(std::vector<Line>& lines, std::vector<Node>& nodes) {
	for (size_t i = 0; i < lines.size(); i++) {
		size_t u, v, cap, cost, n;
		u = lines[i].startNode;
		v = lines[i].endNode;
		n = nodes.size();
		cap = lines[i].getCapacity();
		cost = lines[i].GetCost();
		AddEdgeLine(u, v, cap, long(cost), n);
		AddEdgeLine(v, u, cap, long(cost), n); // Droga jest dwukierunkowa wiec dodajemy tez od drugiej strony
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
			AddEdgeBuilding(i, i + n, nodes[i].capacity, 0);
			continue;
		}
		if (type == NodeType::Tavern)
		{
			// Ujscie z tawern do super sink
			AddEdgeBuilding(i + n, sink, nodes[i].capacity, 0);
			continue;
		}
		if (type == NodeType::Farm)
		{
			// Ujscie z super source do farm
			AddEdgeBuilding(source, i, nodes[i].capacity, 0);
			continue;
		}
	}
	printGraph();
}

std::pair<size_t, size_t> FlowAlgorithm::Calculate()
{
	std::pair<size_t, size_t> output = { 0, 0 }; // MaxFLow, MinCost
	while (true) { 	//dziala dopoki nie zostanie przerwany w trakcie szukania najtanszej sciezki, dzieki temu znajdujemy maksymalny przeplyw
		// Dijkstra na grafie z potencja³ami
		std::priority_queue<std::pair<size_t, size_t>, std::vector<std::pair<size_t, size_t>>, std::greater<>> pq;
		//wierzcholek z najmniejsza dotychczasowa odlegloscia bedzie sprawdzany jako pierwszy
		fill(dist.begin(), dist.end(), max_size_t);
		dist[source] = 0;
		fill(used.begin(), used.end(), false);
		pq.push({ 0, source });
		while (!pq.empty()) {
			std::pair<size_t, size_t> tmp = pq.top();
			size_t d = tmp.first;
			size_t u = tmp.second;
			if(u == sink) {
				break;
			}
			pq.pop();
			if (used[u])
				continue; //jesli warunek spelniony to znaczy ze znaleziono juz do tego wierzcholka tansza droge
			used[u] = true; // oznaczamy wierzcholek jako uzyty, aby nie sprawdzac go ponownie
			for (size_t i = 0; i < graph[u].size(); i++) {
				Edge& e = graph[u][i];
				//std::cout << "Checking edge from " << u << " to " << e.to << ": " << i << std::endl;
				if (e.cap > 0 && !used[e.to]) {
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

		if (dist[sink] == max_size_t) {
			std::cout << "No more paths found." << std::endl;
			break; // brak sciezki, koniec algorytmu
		}


		//Aktualizacja potencjalow
		for (size_t i = 0; i < potential.size(); i++) {
			if (dist[i] < max_size_t)
				potential[i] += dist[i];
		}


		// Znalezienie maksymalnego przep³ywu na znalezionej œcie¿ce
		size_t flow = max_size_t;
		size_t v = sink;
		while (v != source) {
			size_t u = parent[v];
			Edge& e = graph[u][line[v]];
			// graph[parent[v][line[u]] -> parent przeszedl przez krawedz line[u] w grafie do v

			if (e.isPath)
			{
				Edge& mirEgde = graph[e.mirEdge][e.mirIndx]; // krawedz odwrotna w drugiej warstwie
				size_t maxCap =std::min(e.cap, mirEgde.cap); // sprawdzamy maksymalny przeplyw na linii
				if (parent[mirEgde.to] == e.mirEdge) // sprawdzamy czy krawedz odwrotna jest w sciezce
				{
					//std::cout << "Boht used " << e.cap << " " << flow;
					flow = std::min(flow, std::max(size_t(maxCap/ 2), size_t(1))); // sprawdzamy przeplyw na krawedzi odwrotnej
					//std::cout << " " << flow << std::endl;
					//sf::sleep(sf::milliseconds(250));
				}
				else
				{
				//	std::cout << "One used " << e.cap << " " << flow;
					flow = std::min(flow, maxCap);
					//std::cout << " " << flow << std::endl;

				}
			}
			else
			{
				flow = std::min(flow, e.cap);
				//std::cout << "Node path" << flow << std::endl;
				//sf::sleep(sf::milliseconds(250));
			}
			v = u;
		}

		v = sink;
		while (v != source) {
			size_t u = parent[v];
			Edge& e = graph[u][line[v]];
			if (e.cap == 0)
			{
				v = u;
				continue;
			}
			e.cap -= flow;
			graph[e.to][e.rev].cap += flow;
			if (e.isPath) {
				Edge& mirEgde = graph[e.mirEdge][e.mirIndx];
				//std::cout << e.to << " " << mirEgde.to << " " << e.rev << " " << mirEgde.rev << std::endl;
				mirEgde.cap = e.cap;
				graph[mirEgde.to][mirEgde.rev].cap = graph[e.to][e.rev].cap;
			}
			
			v = u;
		}

		output.first += flow; // dodajemy przeplyw do maksymalnego przeplywu
		output.second += flow * potential[sink]; // dodajemy koszt przeplywu do minimalnego kosztu
		//sf::sleep(sf::milliseconds(25)); // opóŸnienie dla lepszego zobrazowania dzia³ania algorytmu
		//std::cout << "Sent " << output.first << "/62500" << std::endl;

	}
	std::cout << "Max Flow: " << output.first << ", Min Cost: " << output.second << std::endl;
	return output;
}

void FlowAlgorithm::CalculatePotential()
{
	std::pair<size_t, size_t> start = { 0, source };
	std::priority_queue<std::pair<size_t, size_t>, std::vector<std::pair<size_t, size_t>>, std::greater<>> pq;
	pq.push(start);
	while (!pq.empty()) {
		std::pair<size_t, size_t> tmp = pq.top();
		size_t d = tmp.first;
		size_t u = tmp.second;
		pq.pop();
		if (d > dist[u])
			continue; //jesli warunek spelniony to znaczy ze znaleziono juz do tego wierzcholka tansza droge
		for (size_t i = 0; i < graph[u].size(); i++) {
			Edge& e = graph[u][i];
			if (e.cap > 0) {
				size_t nd = dist[u] + e.cost;
				if (nd < potential[e.to]) {
					potential[e.to] = nd;
					dist[e.to] = nd; // aktualizujemy odleglosc do wierzcholka
					pq.push({ nd, e.to });
				}
			}
		}
	}
	for(int i = 0; i < potential.size(); i++) {
		if (potential[i] == max_size_t) {
			potential[i] = 0; // jesli nie ma sciezki do wierzcholka to potencjal jest 0
		}
	}
	
}

void FlowAlgorithm::printGraph() {
	for (size_t i = 0; i < graph.size(); i++) {
		for (size_t j = 0; j < graph[i].size(); j++) {
			Edge& e = graph[i][j];
			if (e.cap < 51)
				continue;
			sf::sleep(sf::milliseconds(50));
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
		if (e.to == v && e.cost < 0)
			output += e.cap; // zwraca ile przeplywa przez linie w warstwie 1
	}
	//for (size_t i = 0; i < graph[u + n].size(); i++)
	//{
	//	//std::cout << "Checking edge from " << u << " to " << v << ": " << i << std::endl;
	//	Edge& e = graph[u + n][i];
	//	//std::cout << "Edge to: " << e.to << ", cost: " << e.cost << ", cap: " << e.cap << std::endl;
	//	if (e.to == v + n && e.cost < 0)
	//		output += e.cap; // zwraca ile przeplywa przez linie w warstwie 1
	//}
	return output;
}

size_t FlowAlgorithm::checkBuilding(size_t u, size_t v)
{
	for (size_t i = 0; i < graph[u].size(); i++)
	{
		Edge& e = graph[u][i];
		if (e.to == v)
			return graph[e.to][e.rev].cap;
	}
}
