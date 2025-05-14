#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Line.hpp"
#include "Node.hpp"
#include "Functions.hpp"
class Game {

public:
    Game(sf::RenderWindow& window);
    ~Game();
    void run();

    std::vector<Line> linie;
    std::vector<Node> farms;
	sf::Texture bg_texture;
	sf::RectangleShape background;
    std::vector<sf::RectangleShape*> all;
    std::vector<sf::RectangleShape*> ui;
private:
    void processEvents();
    void update();
    void render();

    void handleMouseInput();
    void handleKeyboardInput();

    sf::RenderWindow& window;
    sf::View view;
    sf::View uiView;

    sf::Texture tx_farm, tx_tav, tx_cross;
    std::shared_ptr<sf::Texture> tx_road;
    sf::RectangleShape button, button2, button3;

    bool change = false;
    bool isFarm = true;
};

#endif // GAME_HPP
