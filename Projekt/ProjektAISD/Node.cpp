#include <SFML/Graphics.hpp>
#include "Node.hpp"

Node::Node(sf::Window& window)
{
    this->setPosition({ float(sf::Mouse::getPosition(window).x), float(sf::Mouse::getPosition(window).y) });
    this->setOrigin({ 40,40 });
    this->setSize({ 80,80 });
    this->setFillColor(sf::Color::Yellow);
}

Node::Node(sf::Vector2f pos)
{
    this->setPosition(pos);
    this->setOrigin({ 10,10 });
    this->setSize({ 20,20 });
    this->setFillColor(sf::Color::White);
}
