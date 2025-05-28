#ifndef MENU_HPP
#define MENU_HPP

#include <SFML/Graphics.hpp>
#include "Button.hpp"
#include <vector>
#include <iostream>
#include "Game.hpp"
class Menu
{
	std::vector<Button> buttons;
	sf::RenderWindow& window;
	sf::View view;
	sf::Texture img, plank;
	sf::RectangleShape background;
	sf::Font font;

public:
	Menu(sf::RenderWindow& window);
	void run();

};

#endif // MENU_HPP