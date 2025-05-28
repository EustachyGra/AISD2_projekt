#ifndef CONVEX_HPP
#define CONVEX_HPP
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Functions.hpp"
#include <vector>
#include <algorithm>
#include <cmath>
#include <ctime>
#include <stdlib.h>

//struct Point {
//	float x, y;
//	float deg = -1;
//
//};
class Convex : public sf::ConvexShape
{
public:
	Convex();
	Convex(sf::Vector2f pos);
	Convex(std::vector<sf::Shape*>& all);
	Convex(sf::Vector2f pos, std::vector<sf::Vector2f> points);
	std::vector<sf::Vector2f> GrahamHull(std::vector<sf::Vector2f> points);
	std::vector<sf::Vector2f> MakePoints(sf::Vector2f pos);
	bool Contains(sf::Vector2f point);

private:
	std::vector<sf::Vector2f> hullPoints;
};

bool CordSort(sf::Vector2f& A, sf::Vector2f& B);
bool DegSort(sf::Vector2f& A, sf::Vector2f& B, sf::Vector2f O);
//void CalcDeg(Point& A, Point& O);

#endif // CONVEX_HPP
