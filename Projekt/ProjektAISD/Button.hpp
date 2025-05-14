#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <SFML/Graphics.hpp>

class Button : public sf::RectangleShape
{
	std::shared_ptr<sf::Font> font;
	std::string textString;
public:
	Button( sf::Vector2f pos, sf::Vector2f size, std::shared_ptr<sf::Font> ft, std::string str);
	/*void setText(std::string str);
	void setFont(sf::Font font);
	void setSize(sf::Vector2f size);
	void setPosition(sf::Vector2f pos);
	void setColor(sf::Color color);
	void setOutlineColor(sf::Color color);
	void setOutlineThickness(float thickness);
	sf::RectangleShape getButton();*/
	bool isMouseOver(sf::RenderWindow& window, sf::View& view);
	//sf::Text& getText();
	void draw(sf::RenderWindow& w, sf::View& v);

};
#endif// BUTTON_HPP