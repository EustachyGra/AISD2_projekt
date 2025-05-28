#ifndef FLOW_ALGORITHM_HPP
#define FLOW_ALGORITHM_HPP
#include <vector>
#include <utility>
#include <iostream>
#include <limits>
#include <algorithm>
#include <queue>
#include "Line.hpp"
#include "Node.hpp"

struct Edge { //dodatkowa struktura, bo zajmie mniej miejsca niz Line, a trzeba podwoic liczbe i jeszcze dojdzie graf rezydualny
	size_t to, rev; // to - do którego wierzcho³ka prowadzi krawêdŸ, rev - indeks krawedzi w liscie sasiedztwa
	size_t cap;
	long cost;
	Edge(size_t to, size_t rev, size_t cap, long cost);
};

class FlowAlgorithm
{
private:
	size_t n;
	const size_t max = std::numeric_limits<size_t>::max();
	std::vector<std::vector<Edge>> graph;
	std::vector<size_t> dist;
	std::vector<size_t> parent, line;
	std::vector <size_t> potential;
	size_t source, sink;
public:
	/// <summary>
	/// Klasa do przep³ywu maksymalnego i minimalnego kosztu.
	/// Korzysta z algorytmu Busacker-Gowen
	/// z uzyciem algorytmu Dijkstry z potencjalami w celu znalezienia najtanszej drogi.
	/// Z potencjalami, poniewaz koszt krawedzi moze byc ujemny.
	/// </summary>
	/// <param name="n">Ilosc wierzcholkow na mapie</param>
	FlowAlgorithm(size_t n);
	/// <summary>
	/// Tworzy dwuwarstwowy graf rezydualny z lini i wierzcho³ków.
	/// - Warstwa pierwsza ma dostep do super Ÿród³a,
	/// - warstwa druga do super ujœcia,
	/// - przez browary mozna przejsc na druga warstwe
	/// </summary>
	/// <param name="lines">Vector linii</param>
	/// <param name="nodes">Vector wierzcholkow</param>
	void MakeGraph(std::vector<Line>& lines, std::vector<Node>& nodes);
	/// <summary>
	/// Dodaje krawêdŸ i jej krawedz odwrotna do obu warstw grafu rezydualnego.
	/// </summary>
	/// <param name="u">Wierzcholek z ktorego wychodzi droga</param>
	/// <param name="v">Wierzcholek do ktorego dochodzi droga</param>
	/// <param name="cap">Maksymalny przeplyw drogi</param>
	/// <param name="cost">Koszt utrzymania drogi</param>
	void AddEdge(size_t u, size_t v, size_t cap, long cost);
	/// <summary>
	/// Wylicza maksymalny przep³yw i minimalny koszt przep³ywu w grafie.
	/// </summary>
	/// <returns>MaxFlow, MinCost</returns>
	std::pair<size_t,size_t> Calculate();
	
	void printGraph();
	std::pair<size_t, size_t> checkLine(size_t u, size_t v);
	size_t checkBuilding(size_t u, size_t v);


};
#endif // FLOW_ALGORITHM_HPP



