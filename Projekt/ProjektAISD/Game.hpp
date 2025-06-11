#ifndef GAME_HPP
#define GAME_HPP

#include "Functions.hpp"
#include "Line.hpp"
#include "Node.hpp"
#include "FlowAlgorithm.hpp"
#include "Convex.hpp"
#include "FileCompression.hpp"
#include "Button.hpp"
#include "BM.hpp"

#include <memory>
#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <fstream>
#include <sstream>

class Game {

public:
	Game(sf::RenderWindow& window, sf::Font& ft, bool load = false);
	~Game();
	void run();
	size_t turn = 0; // Current turn number
	std::vector<Line> linie;
	std::vector<Node> farms;
	std::vector<Convex> convexes;
	std::string stats, input;
	sf::Texture bg_texture;
	sf::RectangleShape background;
	std::vector<sf::Shape*> all;
	std::vector<sf::RectangleShape*> ui;
private:
	void processEvents();
	void update();
	void render(sf::RectangleShape* obj = nullptr);

	void handleMouseInput();
	void LeftMouseClick();
	void RightMouseClick();
	void handleKeyboardInput();
	void SaveGame();
	void LoadGame();
	void TurnEnd();
	bool ButtonHandler();
	void UpdateAll();

	sf::Vector2f lastMousePos;
	bool dragging;
	sf::RenderWindow& window;
	sf::View view;
	sf::View uiView;
	sf::Font font;
	sf::Texture tx_farm, tx_tav, tx_cross, tx_ale, tx_road, tx_cancel, tx_save, tx_stats, tx_turn, tx_mob;
	Button cash_txt, ale_txt;
	Button statsButton, saveButton, turnButton, endButton;
	bool endGame = false, lost = false;
	Button *message = nullptr;
	UpgradeBox* upgrade = nullptr; // Pointer to the upgrade box, if it exists
	size_t upgrade_type=0,upgrade_id = 0;
	std::vector<Button> buildButtons;
	TextBox* statsBox = nullptr;
	Line* newLine = nullptr;
	Node* newNode = nullptr;
	long long cash = 2000; // Starting cash
	size_t quota = 100,cost;
	int placeMode = 0; // 0 - nic, 1 - road, 2 - farm, 3 - tavern, 4 - alehouse
	NodeType placeType = NodeType::Farm;
	void printUsed();
};

#endif // GAME_HPP
