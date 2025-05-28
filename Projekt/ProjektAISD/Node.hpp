#ifndef NODE_HPP
#define NODE_HPP

#include <SFML/Graphics.hpp>
enum class NodeType
{
	Farm,
	Tavern,
	Alehouse,
	Crossroad
};
class Node : public sf::RectangleShape
{
private:
	size_t used;

public:
	size_t capacity = 50;
	NodeType type;

    Node(sf::Window& window, NodeType typ = NodeType::Farm);
    Node(sf::Vector2f pos);
	size_t getCapacity();
	void setCapacity(size_t cap = 0);
	// Type management
	void setType(NodeType type);
	NodeType getType();
	void setUsed(size_t used);
	size_t getUsed();
};

#endif // NODE_HPP
