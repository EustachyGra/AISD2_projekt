#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include "Line.hpp"
#include "Node.hpp"
struct MemCP
{
	int i;
	float dist;
	sf::Vector2f pos;
	MemCP(int i, float dist, sf::Vector2f pos) : i(i), dist(dist), pos(pos) {}
};

float dl(sf::Vector2f a, sf::Vector2f b);
float dl(float ax, float ay, float bx, float by);
sf::Vector2f closestPointOnLine(sf::Vector2f a, sf::Vector2f b, sf::Vector2f m);
bool CursorOnLine(const Line& line, const Line& newLine, std::vector<Node> nodes);
bool CursorNearLine(const Line& line, sf::Vector2f pos, std::vector<Node> nodes);
void splitLine(sf::Vector2f mid, Line ogLine, std::vector<Line>& linie, int i, std::vector<Node>& farms, sf::Texture& tx);
void splitLine(Line ogLine, std::vector<Line>& linie, int nodeId, int lineId, std::vector<Node>& farms);
bool LineCrossing(sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f q1, sf::Vector2f q2);
void draw(std::vector<sf::RectangleShape*>& ui, std::vector<sf::RectangleShape*>& all, sf::RenderWindow& w, sf::View& view, sf::View& uiView, sf::RectangleShape* obj = nullptr);
int HoverOverFarm(sf::RenderWindow& w, sf::View& view, const std::vector<Node> farms, int j = -1);
bool checkIntersection(std::vector<Line>& linie, Line& linia, std::vector<Node> nodes);
bool checkIntersection(std::vector<Line>& linie, Node& node, std::vector<Node> nodes);
sf::Vector2f MousePosView(sf::RenderWindow& w, sf::View& view);
std::vector<std::vector<size_t>> adjMatrixCap(std::vector<Line>& linie, std::vector<Node>& farms);

void printAdjMatrix(std::vector<std::vector<size_t>>& adj);
#endif // FUNCTIONS_HPP