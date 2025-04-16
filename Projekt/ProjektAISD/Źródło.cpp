#include "Game.hpp"
#include <iostream>
int main() {
    sf::RenderWindow window(sf::VideoMode().getDesktopMode(), "Not Enough Ale!", sf::Style::Default);
    std::cout<<window.getView().getSize().x<<" "<< window.getView().getSize().y << std::endl;
    Game game(window);
    game.run();
    return 0;
}
