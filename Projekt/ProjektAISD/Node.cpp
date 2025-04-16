#include <SFML/Graphics.hpp>
#include "Node.hpp"

Node::Node(sf::Window& window)
{
    this->setPosition({ float(sf::Mouse::getPosition(window).x), float(sf::Mouse::getPosition(window).y) });
    this->setOrigin({ 25,25 });
    this->setSize({ 50,50 });
    this->setFillColor(sf::Color::Yellow);
}

Node::Node(sf::Vector2f pos)
{
    this->setPosition(pos);
    this->setOrigin({ 10,10 });
    this->setSize({ 20,20 });
    this->setFillColor(sf::Color::White);
}
