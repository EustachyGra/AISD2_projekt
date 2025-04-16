#ifndef LINE_HPP
#define LINE_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include "Node.hpp"

class Line : public sf::RectangleShape
{
private:
    std::shared_ptr<sf::Texture> texture;
    sf::Vector2f endPos;
	sf::Color green = sf::Color(0, 255, 0, 160);
	sf::Color red = sf::Color(255, 0, 0, 160);
    size_t capacity;
    size_t cost;
    //sf::Text text;

public:
    
    size_t startNode;
    size_t endNode;
    int startLine =-1;
    int CollideWith = -1;
    bool CanPlace = false;
    bool freePlace = false;

    Line();
    Line(sf::Vector2f pos, std::shared_ptr<sf::Texture>);
    Line(sf::RectangleShape x);
    sf::Vector2f getPosEnd() const;
    void setPosEnd(sf::Vector2f pos);
    void ConnectToNode(std::vector<Node> nodes, size_t tmpEnd = -1, size_t tmpStart = -1);
    void FollowMouse(sf::Vector2f mousePos);
    void Reset();
    std::shared_ptr<sf::Texture> GetTxt();
    bool SnapToLine(std::vector<Line>& linie, std::vector<Node> farms, sf::Vector2f mousePos);
    void SetGreen();
    void SetRed();
    void LookGood();
	size_t getCapacity();
	void setCapacity(size_t cap=0);
    void SetCost(size_t cost);
	size_t GetCost();
};

#endif // LINE_HPP
