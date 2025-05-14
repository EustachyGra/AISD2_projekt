#include "Button.hpp"
#include <iostream>
Button::Button(sf::Vector2f pos, sf::Vector2f size, std::shared_ptr<sf::Font> ft, std::string str)
{
	this->setSize(size);
	this->setPosition(pos);
	this->setFillColor(sf::Color::White);

	textString = str;
	font = ft;
	
}

bool Button::isMouseOver(sf::RenderWindow& window, sf::View& view)
{
	return getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window),view));
}

//sf::Text& Button::getText()
//{
//	return text;
//}
void Button::draw(sf::RenderWindow& w, sf::View& v)
{
	w.setView(v);
	w.draw(*this);
	sf::Text text(*font, textString, 24);
	text.setFillColor(sf::Color::White);
	text.setStyle(sf::Text::Bold);
	text.setOutlineColor(sf::Color::Black);
	text.setOutlineThickness(3);
	text.setPosition({ getPosition().x + getSize().x / 2 - text.getGlobalBounds().size.x / 2,
		getPosition().y + getSize().y / 2 - text.getGlobalBounds().size.y / 2 - 5});
	w.draw(text);
}
