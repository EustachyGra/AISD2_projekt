#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <vector>
#include <algorithm>
#include "Functions.hpp"
#include "Line.hpp"
#include "Node.hpp"
#include <iomanip> // tylko do printAdjMatrix

float dl(sf::Vector2f a, sf::Vector2f b)
{
    return (float)sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}
float dl(float ax, float ay, float bx, float by)
{
	return (float)sqrt(pow(ax - bx, 2) + pow(ay - by, 2));
}
sf::Vector2f closestPointOnLine(sf::Vector2f a, sf::Vector2f b, sf::Vector2f m)
{
    float t = ((m.x - a.x) * (b.x - a.x) + (m.y - a.y) * (b.y - a.y)) / ((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y));
    t = std::max(0.f, std::min(1.f, t)); // Clamp t to the range [0, 1]
    if (t < 0.05)
		t = 0.05f;
    else if (t > 0.95)
        t = 0.95f;
    sf::Vector2f d = { a.x + t * (b.x - a.x), a.y + t * (b.y - a.y) };
    return d;
}

bool CursorOnLine(const Line& line, const Line& newLine, std::vector<Node> nodes)
{
    float dlLineFrontNew = dl(newLine.getPosEnd(), nodes[line.startNode].getPosition());
    float dlLineBackNew = dl(newLine.getPosEnd(),nodes[line.endNode].getPosition());
    return (dlLineFrontNew + dlLineBackNew < line.getSize().x + 1);
}
bool CursorNearLine(const Line& line, sf::Vector2f pos, std::vector<Node> nodes)
{
    float dlLineFrontNew = dl(pos, nodes[line.startNode].getPosition());
    float dlLineBackNew = dl(pos, nodes[line.endNode].getPosition());
    return (dlLineFrontNew + dlLineBackNew < line.getSize().x + 5);
}

void splitLine(sf::Vector2f mid, Line ogLine, std::vector<Line>& linie, int i, std::vector<Node>& farms, sf::Texture& tx)
{
    Node crossroad({ 20, 20 });
    crossroad.setFillColor(sf::Color::White);
    crossroad.setOrigin({ 10, 10 });
    crossroad.setTexture(&tx);
    crossroad.setPosition(closestPointOnLine(farms[ogLine.startNode].getPosition(), farms[ogLine.endNode].getPosition(), mid));
    farms.push_back(crossroad);
    mid = crossroad.getPosition();
    Line linia1(ogLine);
    linia1.ConnectToNode(farms, farms.size() - 1, ogLine.startNode);
	linia1.SetLifeSpan(ogLine.GetLifeSpan());
	linia1.setCapacity(ogLine.getCapacity());
	linia1.LookGood();
    Line linia2(ogLine);
    linia2.ConnectToNode(farms, ogLine.endNode, farms.size() - 1);
    linia2.SetLifeSpan(ogLine.GetLifeSpan());
	linia2.setCapacity(ogLine.getCapacity());
	linia2.LookGood();
    linie.erase(linie.begin() + i);
    linie.push_back(linia1);
    linie.push_back(linia2);
}
void splitLine(Line ogLine, std::vector<Line>& linie, int nodeId, int lineId, std::vector<Node>& farms)
{
    Line linia1(ogLine);
    linia1.ConnectToNode(farms, nodeId, ogLine.startNode);
	linia1.SetLifeSpan(ogLine.GetLifeSpan());
    linia1.LookGood();
    Line linia2(ogLine);
    linia2.ConnectToNode(farms, ogLine.endNode, nodeId);
	linia2.SetLifeSpan(ogLine.GetLifeSpan());
    linia2.LookGood();
    linie.erase(linie.begin() + lineId);
    linie.push_back(linia1);
    linie.push_back(linia2);
}
void draw(std::vector<sf::RectangleShape*>& ui, std::vector<sf::RectangleShape*>& all, sf::RenderWindow& w, sf::View& view, sf::View& uiView, sf::RectangleShape* obj)
{
    w.clear(sf::Color::White);
    w.setView(uiView);
    for (size_t i = 0; i < ui.size(); i++)
    {
        w.draw(*ui[i]);
    }

    w.setView(view);
    for (size_t i = 0; i < all.size(); i++)
    {   
        w.draw(*all[i]);
    }
    if (obj != nullptr)
        w.draw(*obj);
    
    w.display();
}

int HoverOverFarm(sf::RenderWindow& w, sf::View& view, const std::vector<Node> farms, int j)
{
    for (int i = 3; i < farms.size(); i++) {
        if (i == j) continue;
        if (farms[i].getGlobalBounds().contains(MousePosView(w, view)))
        {
            return i;
        }
    }
    return -1;
}
bool LineCrossing(sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f q1, sf::Vector2f q2)
{
    float x1 = p1.x;//nodes[linie[i].startNode].getPosition().x;
    float y1 = p1.y;//nodes[linie[i].startNode].getPosition().y;
    float x2 = p2.x;//nodes[linie[i].endNode].getPosition().x;
    float y2 = p2.y;//nodes[linie[i].endNode].getPosition().y;
    float x3 = q1.x;//nodes[linia.startNode].getPosition().x;
    float y3 = q1.y;//nodes[linia.startNode].getPosition().y;
    float x4 = q2.x;//linia.getPosEnd().x;
    float y4 = q2.y;//linia.getPosEnd().y;
    float d1 = round((x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1));
    float d2 = round((x2 - x1) * (y4 - y1) - (y2 - y1) * (x4 - x1));
    float d3 = round((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3));
    float d4 = round((x4 - x3) * (y2 - y3) - (y4 - y3) * (x2 - x3));

    //std::cout << "d1: " << d1 << " d2: " << d2 << " d3: " << d3 << " d4: " << d4 << std::endl;
    if (d1 * d2 <= 0 && d3 * d4 <= 0)
    {
        return true;
    }
    return false;
}
bool checkIntersection(std::vector<Line>& linie, Line& linia, std::vector<Node> nodes)
{
    if(linia.startLine != -1)
		if (linie[linia.startLine].startNode == linia.endNode || linie[linia.startLine].endNode == linia.endNode)
			return true;
    for (int i = 3; i < nodes.size(); i++)
    {
        if (i == linia.startNode || i == linia.endNode) continue;
		sf::Vector2f size = nodes[i].getSize();
		sf::Vector2f bounds_og = nodes[i].getGlobalBounds().position;
        sf::Vector2f bounds_1 = { bounds_og.x - 10, bounds_og.y - 10 };
		sf::Vector2f bounds_2 = { bounds_og.x + size.x + 10, bounds_og.y + size.y + 10 };
		if (LineCrossing(bounds_1, bounds_2, linia.getPosEnd(), nodes[linia.startNode].getPosition())) 
		{
			//std::cout << "Intersection with node " << i << std::endl;
			return true;
		}
		bounds_1 = { bounds_og.x - 10, bounds_og.y + size.y + 10 };
		bounds_2 = { bounds_og.x + size.x + 10, bounds_og.y - 10};
        if (LineCrossing(bounds_1, bounds_2, linia.getPosEnd(), nodes[linia.startNode].getPosition()))
        {
			//std::cout << "Intersection with node " << i << std::endl;
			return true;
        }
        
    }
    for (int i = 0; i < linie.size(); i++)
    {
		if (linia.startLine == i) continue;
        if ((linia.endNode == linie[i].endNode && linia.startNode == linie[i].startNode) ||
            (linia.endNode == linie[i].startNode && linia.startNode == linie[i].endNode))
        {
            //std::cout << "Same line " << linie[i].startNode << " " << linie[i].endNode << std::endl;
            return true;
        }
        if (linie[i].startNode == linia.startNode || linie[i].endNode == linia.startNode || linia.CollideWith == i
            || linie[i].startNode == linia.endNode || linie[i].endNode == linia.endNode)
        {
            //std::cout << "Same node " << linie[i].startNode << " " << linie[i].endNode << std::endl;
            continue;
        }


        //std::cout << "Checking line " << linie[i].startNode << " " << linie[i].endNode << std::endl;

        if (LineCrossing(nodes[linie[i].startNode].getPosition(), nodes[linie[i].endNode].getPosition(), nodes[linia.startNode].getPosition(), linia.getPosEnd()))
        {
            //std::cout << "Intersection " << linie[i].startNode << " " << linie[i].endNode << std::endl;
            return true;
        }

       /* float tolerance = 2.0f;
        if (dl(x3, y3, x1, y1) + dl(x3, y3, x2, y2) - dl(x1, y1, x2, y2) < tolerance ||
            dl(x4, y4, x1, y1) + dl(x4, y4, x2, y2) - dl(x1, y1, x2, y2) < tolerance)
        {
            std::cout << "Touching " << linie[i].startNode << " " << linie[i].endNode << std::endl;
            return true;
        }*/
    }
    //std::cout << "No intersection" << std::endl;
    return false;
}

bool checkIntersection(std::vector<Line>& linie, Node& node, std::vector<Node> nodes)
{
	for (int i = 3; i < nodes.size(); i++)
	{
		if (node.getGlobalBounds().findIntersection(nodes[i].getGlobalBounds()))
		{
			std::cout << "Intersection with node " << i << std::endl;
			return true;
		}
	}
    sf::Vector2f size = node.getSize();
    sf::Vector2f bounds_og = node.getGlobalBounds().position;
    sf::Vector2f bounds_1 = { bounds_og.x - 10, bounds_og.y - 10 };
    sf::Vector2f bounds_2 = { bounds_og.x + size.x + 10, bounds_og.y + size.y + 10 };
    sf::Vector2f bounds_3 = { bounds_og.x - 10, bounds_og.y + size.y + 10 };
    sf::Vector2f bounds_4 = { bounds_og.x + size.x + 10, bounds_og.y - 10 };
    for (int i = 0; i < linie.size(); i++)
    {
        if (LineCrossing(bounds_1, bounds_2, nodes[linie[i].startNode].getPosition(), nodes[linie[i].endNode].getPosition()))
        {
            std::cout << "Intersection with line " << i << std::endl;
            return true;
        }

        if (LineCrossing(bounds_3, bounds_4, nodes[linie[i].startNode].getPosition(), nodes[linie[i].endNode].getPosition()))
        {
            std::cout << "Intersection with line " << i << std::endl;
            return true;
        }
    }
    
    return false;
}

sf::Vector2f MousePosView(sf::RenderWindow& w, sf::View& view)
{
    sf::Vector2i pixelPos = sf::Mouse::getPosition(w);
    sf::Vector2f worldPos = w.mapPixelToCoords(pixelPos,view); 
    return worldPos;
}

std::vector<std::vector<size_t>> adjMatrixCap(std::vector<Line>& linie, std::vector<Node>& farms)
{
    std::vector<std::vector<size_t>> CapMatrix(farms.size());
    for (int i = 0; i < farms.size(); i++)
    {
        CapMatrix[i].resize(farms.size(), 0);
        if (farms[i].type == NodeType::Farm)
            CapMatrix[0][i] = farms[i].capacity;
        else if (farms[i].type == NodeType::Tavern)
            CapMatrix[i][1] = farms[i].capacity;
        else if (farms[i].type == NodeType::Alehouse) {
            CapMatrix[i][2] = farms[i].capacity;
			CapMatrix[2][i] = farms[i].capacity;
        }

    }
	std::cout << "Adjacency matrix crrate:\n";
    for (int i = 0; i < linie.size(); i++)
    {
        CapMatrix[linie[i].startNode][linie[i].endNode] = linie[i].getCapacity();
        CapMatrix[linie[i].endNode][linie[i].startNode] = linie[i].getCapacity();
    }
    return CapMatrix;
}


void printAdjMatrix(std::vector<std::vector<size_t>>& adj)
{
	for (int i = 0; i < adj.size(); i++)
	{
		for (int j = 0; j < adj[i].size(); j++)
		{
			std::cout <<std::setw(2)<< adj[i][j] << " ";
		}
		std::cout << std::endl;
	}
}


