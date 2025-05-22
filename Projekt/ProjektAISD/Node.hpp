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

	size_t production;

public:
	size_t capacity = 50;
	NodeType type;

    Node(sf::Window& window);
    Node(sf::Vector2f pos);
};

#endif // NODE_HPP
