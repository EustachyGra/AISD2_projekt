#ifndef ALGORITHMS_HPP
#define ALGORITHMS_HPP

#include <algorithm>
#include <climits>
#include <vector>
#include <queue>
#include "Line.hpp"
#include "Node.hpp"
#include "Functions.hpp"

bool BFS(std::vector<std::vector<size_t>>& adjMatrix, std::vector<size_t>& parent);
size_t edmondsKarp(std::vector<std::vector<size_t>> adjMatrix);

#endif // !ALGORITHMS_HPP
