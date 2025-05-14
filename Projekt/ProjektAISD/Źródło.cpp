#include "Game.hpp"
#include "Menu.hpp"
#include <iostream>
int main() {
    sf::Image icon;
    sf::RenderWindow window(sf::VideoMode().getDesktopMode(), "Not Enough Ale!", sf::Style::Default);
    if (!icon.loadFromFile("Textury/beer2.png"))
    {
		std::cerr << "Error loading icon" << std::endl;
		return -1;
    }
    window.setIcon(icon.getSize(), icon.getPixelsPtr());

	Menu menu(window);
	menu.run();
    Game game(window);
    game.run();
    return 0;
}
