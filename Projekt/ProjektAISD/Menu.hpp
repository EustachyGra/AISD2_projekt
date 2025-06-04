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
	Button* error=nullptr;
	sf::RenderWindow& window;
	sf::View view;
	sf::Texture img, plank;
	sf::RectangleShape background;
	sf::Font font;
	bool isError = false;

public:
	Menu(sf::RenderWindow& window);
	void run();
	~Menu();

};

#endif // MENU_HPP