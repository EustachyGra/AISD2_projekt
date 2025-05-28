#ifndef GAME_HPP
#define GAME_HPP

#include "Functions.hpp"
#include "Line.hpp"
#include "Node.hpp"
#include "FlowAlgorithm.hpp"
#include "Convex.hpp"

#include <memory>
#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <fstream>

class Game {

public:
	Game(sf::RenderWindow& window, sf::Font& ft, bool load = false);
	~Game();
	void run();
	size_t turn = 0; // Current turn number
	std::vector<Line> linie;
	std::vector<Node> farms;
	std::vector<Convex> convexes;
	std::string stats;
	sf::Texture bg_texture;
	sf::RectangleShape background;
	std::vector<sf::Shape*> all;
	std::vector<sf::RectangleShape*> ui;
private:
	void processEvents();
	void update();
	void render();

	void handleMouseInput();
	void handleKeyboardInput();
	void SaveGame();
	void LoadGame();
	void TurnEnd();
	void ButtonHandler();
	void UpdateAll();
	sf::RenderWindow& window;
	sf::View view;
	sf::View uiView;
	sf::Font font;
	sf::Texture tx_farm, tx_tav, tx_cross, tx_ale, tx_road;
	sf::RectangleShape button, button2, button3, button4, button5;

	bool change = false;
	int placeMode = 0; //  0 - farm, 1 - tavern, 2 - alehouse
	NodeType placeType = NodeType::Farm;
	void printUsed();
};

#endif // GAME_HPP
