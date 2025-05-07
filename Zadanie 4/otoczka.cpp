#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

struct Point {
	float x, y;
	float deg = -1;

};
bool CordSort(Point& A, Point& B) {
	if (A.y == B.y)
		return A.x < B.x;
	return A.y < B.y;
}

bool DegSort(Point& A, Point& B) {
	if (A.deg == B.deg)
		return CordSort(A, B);
	return A.deg < B.deg;
}

void CalcDeg(Point& A, Point& O) {
	Point tmp = { A.x - O.x , A.y - O.y };
	float d = abs(tmp.x) + abs(tmp.y);
	if (tmp.y >= 0)
	{
		if (tmp.x >= 0)
			A.deg = tmp.y / d;
		else
			A.deg = 2 - tmp.y / d;
	}
	else
	{
		if (tmp.x >= 0)
			A.deg = 4 + abs(tmp.y) / d;
		else
			A.deg = 2 - abs(tmp.y) / d;
	}

}

float CalcTurn(const Point& A, const Point& B, const Point& C) {
	return (B.x - A.x) * (C.y - A.y) - (B.y - A.y) * (C.x - A.x);
}
std::vector<Point> GrahamHull(std::vector<Point> points) {
	if (points.size() < 3) return points;

	std::sort(points.begin(), points.end(), CordSort);
	for (int i = 1; i < points.size(); i++)
	{
		CalcDeg(points[i], points[0]);
	}
	std::sort(points.begin() + 1, points.end(), DegSort);

	std::cout << std::endl;
	std::vector<Point> hull;
	for (int i = 0; i < points.size(); i++)
	{
		while (hull.size() > 2 && CalcTurn(hull[hull.size() - 2], hull[hull.size() - 1], points[i]) <= 0)
		{
			hull.pop_back();
		}
		hull.push_back(points[i]);
	}

	return hull;
}
std::vector<Point> JarvisHull(std::vector<Point>& points) {
	if (points.size() < 3) return points;

	std::sort(points.begin(), points.end(), CordSort);

	int p = 0, q;
	std::vector<Point> hull;

	do {
		hull.push_back(points[p]);
		q = (p + 1) % points.size();

		for (int i = 0; i < points.size(); i++) {
			if (CalcTurn(points[p], points[i], points[q]) > 0) {
				q = i;
			}
		}

		p = q;
	} while (p != 0);

	return hull;
}
int main() {
	std::vector<Point> points;
	int n;
	std::cout << "Podaj liczbe punktow: ";
	std::cin >> n;
	for (int i = 0; i < n; i++)
	{
		Point p;
		std::cout << "Podaj punkt " << i + 1 << " (x,y): ";
		std::cin >> p.x >> p.y;
		points.push_back(p);
	}

	std::vector<Point> hullG = GrahamHull(points);

	std::cout << "Algorytm Graham'a:\n";
	for (int i = 0; i < hullG.size();i++) {
		std::cout << "(" << hullG[i].x << ", " << hullG[i].y << ")\n";
	}
	std::vector<Point> hullJ = JarvisHull(points);

	std::cout << "Algorytm Jarvis'a:\n";
	for (int i = 0; i < hullJ.size(); i++) {
		std::cout << "(" << hullJ[i].x << ", " << hullJ[i].y << ")\n";
	}


	return 0;
}
