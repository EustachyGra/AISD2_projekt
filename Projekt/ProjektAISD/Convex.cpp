#include "Convex.hpp"
bool CordSort(sf::Vector2f& A, sf::Vector2f & B) {
	if (A.y == B.y)
		return A.x < B.x;
	return A.y < B.y;
}

bool DegSort(sf::Vector2f& A, sf::Vector2f& B, sf::Vector2f O) {
	float tr = turn(O, A, B);
	if (tr > 0) return true;
	if (tr < 0) return false;
	if (tr == 0) return CordSort(A, B);
}

std::vector<sf::Vector2f> Convex::GrahamHull(std::vector<sf::Vector2f> points) {
	if (points.size() < 3) return points;

	std::sort(points.begin(), points.end(), CordSort);
	sf::Vector2f p0 = points[0];
	std::sort(points.begin() + 1, points.end(), [p0](sf::Vector2f& a, sf::Vector2f& b) { return DegSort(a, b, p0); });
	for (std::vector<sf::Vector2f>::iterator it = points.begin()+1; it!=points.end()-1; it++) // usuwanie punktow na tej samej krawedzi
	{
		if (turn(p0,*it,*(it+1)) == 0) {
			if (dl(*it, p0) < dl(*(it + 1), p0))
				it = points.erase(it);
			else
				it = points.erase(it + 1);
			it--;
		}
	}
	std::cout << std::endl;
	std::vector<sf::Vector2f> hull;
	for (int i = 0; i < points.size(); i++)
	{
		while (hull.size() > 2 && turn(hull[hull.size() - 2], hull[hull.size() - 1], points[i]) <= 0)
		{
			hull.pop_back();
		}
		hull.push_back(points[i]);
	}

	return hull;
}

Convex::Convex()
{

}

Convex::Convex(sf::Vector2f pos)
{
	this->setPosition(pos);
	this->setOrigin({ 0, 0 });
	this->setFillColor(sf::Color(0, 128, 0, 128));
	std::vector<sf::Vector2f> hull = GrahamHull(MakePoints(pos));
	this->setPointCount(hull.size());
	for (int i = 0; i < hull.size();i++) {
		 sf::Vector2f cord = { hull[i].x+pos.x, hull[i].y+pos.y };
		this->setPoint(i, cord);
	}
}
Convex::Convex(std::vector<sf::Shape*>& all)
{
	srand(time(NULL));
	this->setFillColor(sf::Color(0,128,0,128));
	sf::Vector2f topLeft = all[0]->getGlobalBounds().position;
	sf::Vector2f bottomRight = all[0]->getGlobalBounds().position + all[0]->getGlobalBounds().size;
	for (size_t i = 1; i < all.size(); i++)
	{
		if (all[i]->getGlobalBounds().position.x < topLeft.x)
			topLeft.x = all[i]->getGlobalBounds().position.x;
		if (all[i]->getGlobalBounds().position.y < topLeft.y)
			topLeft.y = all[i]->getGlobalBounds().position.y;
		if (all[i]->getGlobalBounds().position.x + all[i]->getGlobalBounds().size.x > bottomRight.x)
			bottomRight.x = all[i]->getGlobalBounds().position.x + all[i]->getGlobalBounds().size.x;
		if (all[i]->getGlobalBounds().position.y + all[i]->getGlobalBounds().size.y > bottomRight.y)
			bottomRight.y = all[i]->getGlobalBounds().position.y + all[i]->getGlobalBounds().size.y;
		
	}

	float x, y;
	switch (rand()%4)
	{
	case 0: //gora
		std::cout << "gora" << std::endl;
		y = topLeft.y - 100 - rand() % 100;
		x = topLeft.x - 100 + rand() % long(bottomRight.x - topLeft.x + 200);
		break; 
	case 1: //dol
		std::cout << "dol" << std::endl;
		y = bottomRight.y + 100 + rand() % 100;
		x = topLeft.x - 100 + rand() % long(bottomRight.x - topLeft.x + 200);
		break; 
	case 2: //lewo
		std::cout << "lewo" << std::endl;
		x = topLeft.x - 100 - rand() % 100;
		y = topLeft.y - 100 + rand() % long(bottomRight.y - topLeft.y + 200);
		break; 
	case 3: //prawo
		std::cout << "prawo" << std::endl;
		x = bottomRight.x + 100 + rand() % 100;
		y = topLeft.y - 100 + rand() % long(bottomRight.y - topLeft.y + 200);
		break;
	default:
		break;
	}
	topLeft = { x, y };
	std::vector<sf::Vector2f> hull = GrahamHull(MakePoints(topLeft));
	this->setPosition(topLeft);
	this->setOrigin({ 0, 0 });
	this->setPointCount(hull.size());
	for (int i = 0; i < hull.size(); i++) {
		std::cout << hull[i].x << " " << hull[i].y << std::endl;
		sf::Vector2f cord = { hull[i].x, hull[i].y };
		this->setPoint(i, cord);
		hullPoints.push_back(cord+topLeft);
	}
}
Convex::Convex(sf::Vector2f pos,std::vector<sf::Vector2f> points)
{
	this->setPosition(pos);
	this->setOrigin({ 0, 0 });
	this->setPointCount(points.size());
	this->setFillColor(sf::Color(0, 128, 0, 128));

	for (int i = 0; i < points.size(); i++) {
		this->setPoint(i, points[i]);
		hullPoints.push_back(points[i] + pos);

	}

}
std::vector<sf::Vector2f> Convex::MakePoints(sf::Vector2f pos) {
	std::vector<sf::Vector2f> points;
	srand(time(NULL));
	for (int i = 0; i < 10; i++) {
		sf::Vector2f p;
		p.x = rand() % 200 - 100;
		p.y = rand() % 200 - 100;
		points.push_back(p);
	}
	return points;
}

bool Convex::Contains(sf::Vector2f point)
{
	size_t l = 1, p = hullPoints.size() - 1;

	// sprawdzamy czy jest w "trojkacie" o wierzcholku 0 i nieskonczonych bokach (ktore przechodza przez wierzcholek l lub p)
	if (turn(hullPoints[0], hullPoints[l], point) < 0 || turn(hullPoints[0], hullPoints[p], point) > 0) 
		return false;

	// wyszukiwanie binarne
	while (p - l > 1) {
		size_t s = (l + p) / 2;
		float tr = turn(hullPoints[0], hullPoints[s], point);
		if (tr > 0)
			l = s;
		else {
			p = s;
			if (tr == 0)
				break;
		}
	}

	// wiemy ze jest pomiedzy l i p wzgledem 0, sprawdzamy czy nie jest na zewnatrz
	float t = turn(hullPoints[l], hullPoints[p], point);
	return t >= 0;
}
