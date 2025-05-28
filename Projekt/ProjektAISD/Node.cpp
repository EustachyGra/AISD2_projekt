#include "Node.hpp"

Node::Node(sf::Window& window, NodeType typ)
{
	type = typ;
    this->setPosition({ float(sf::Mouse::getPosition(window).x), float(sf::Mouse::getPosition(window).y) });
    this->setOrigin({ 40,40 });
    this->setSize({ 80,80 });
    this->setFillColor(sf::Color::Yellow);
}

Node::Node(sf::Vector2f pos)
{
	this->capacity = 0;

    this->setPosition(pos);
    this->setOrigin({ 10,10 });
    this->setSize({ 20,20 });
	type = NodeType::Crossroad;
    this->setFillColor(sf::Color::White);
}

NodeType Node::getType()
{
    return type;
}

void Node::setUsed(size_t used)
{
    this->used = used;
}
size_t Node::getUsed()
{
    return used;
}

void Node::setType(NodeType type)
{
	this->type = type;
}
size_t Node::getCapacity()
{
	return capacity;
}
void Node::setCapacity(size_t cap)
{
	capacity = cap;
}
