#include "Game.hpp"

Game::Game(sf::RenderWindow& window, sf::Font& ft, bool load)
	: window(window) {
	font = ft;
	view.setSize({ 1920,1080 });
	view.zoom(1.0f); // Initial zoom level
	uiView.setSize({ 1920,1080 });
	srand((unsigned int)time(NULL));


	if (!(tx_farm.loadFromFile("Textury/farm_p3.png"))) {
		throw std::string("Brak pliku tekstur farmy!\n\'Textury/farm_p3.png\'");
	}
	if (!(tx_tav.loadFromFile("Textury/tavern_p3.png"))) {
		throw std::string("Brak pliku tekstur tawerny!\n\'Textury/tavern_p3.png\'");
	}
	if (!(tx_ale.loadFromFile("Textury/browar.png"))) {
		throw std::string("Brak pliku tekstur browara!\n\'Textury/browar.png\'");
	}
	if (!(tx_road.loadFromFile("Textury/raodv4.png"))) {
		throw std::string("Brak pliku tekstur drogi!\n\'Textury/raodv2.png\'");
	}
	if (!(tx_cross.loadFromFile("Textury/cross.png"))) {
		throw std::string("Brak pliku tekstur skrzyzowania!\n\'Textury/cross.png\'");
	}
	if (!(tx_cancel.loadFromFile("Textury/canclev2.png"))) {
		throw std::string("Brak pliku tekstur anulowania!\n\'Textury/canclev2.png\'");
	}
	if (!(tx_stats.loadFromFile("Textury/stats.png"))) {
		throw std::string("Brak tekstur ikony statystyk!\n\'Textury/stats.png\'");
	}
	if (!(tx_save.loadFromFile("Textury/save.png"))) {
		throw std::string("Brak tekstur ikony zapisu!\n\'Textury/save.png\'");
	}
	if (!(tx_turn.loadFromFile("Textury/turn.png"))) {
		throw std::string("Brak tekstur ikony konca tury!\n\'Textury/turn.png\'");
	}
	if (!(tx_mob.loadFromFile("Textury/mob.png"))) {
		throw std::string("Brak tekstur ikony wiadomosci koncowej!\n\'Textury/mob.png\'");
	}
	if (!(bg_texture.loadFromFile("Textury/grass3.png")))
	{
		throw std::string("Brak pliku tekstur tla!");
	}


	background.setSize(view.getSize());
	background.setPosition(window.mapPixelToCoords({ 0, 0 }, uiView));
	background.setOrigin({ 0, 0 });
	background.setTexture(&bg_texture);


	cash_txt = Button(window.mapPixelToCoords({ 5,0 }, uiView), { 0, 0 }, ft, "Bank: $" + std::to_string(cash));
	cash_txt.setFillColor(sf::Color::Transparent);
	ale_txt = Button(window.mapPixelToCoords(sf::Vector2i(0, cash_txt.getSize().y), uiView), { 0, 0 }, ft, "Quota: " + std::to_string(quota) + " barrels");
	ale_txt.setFillColor(sf::Color::Transparent);
	std::cout << std::string(ale_txt.text2->getString()) << std::endl;


	buildButtons.push_back(Button(window.mapPixelToCoords(sf::Vector2i(window.getSize().x * 0.02, window.getSize().y * 0.9), uiView), { 50,50 }, ft, "$100", tx_road, tx_cancel));
	buildButtons.push_back(Button(buildButtons[0].getPosition() + sf::Vector2f(50, -25), buildButtons[0].getSize(), ft, "$200", tx_farm, tx_cancel));
	buildButtons.push_back(Button(buildButtons[1].getPosition() + sf::Vector2f(50, -25), { 50,50 }, ft, "$150", tx_ale, tx_cancel));
	buildButtons.push_back(Button(buildButtons[2].getPosition() + sf::Vector2f(50, -25), { 50,50 }, ft, "$150", tx_tav, tx_cancel));



	turnButton = Button(window.mapPixelToCoords(sf::Vector2i(window.getSize().x * 0.95, window.getSize().y * 0.02), uiView), { 75, 75 }, ft, "");
	turnButton.setTexture(&tx_turn);
	turnButton.setOutlineColor(sf::Color::Black);
	turnButton.setOutlineThickness(2);
	saveButton = Button(window.mapPixelToCoords(sf::Vector2i(window.getSize().x * 0.95, window.getSize().y * 0.95), uiView), { 50, 50 }, ft, "");
	saveButton.setTexture(&tx_save);
	saveButton.setFillColor(sf::Color(34, 118, 255));
	saveButton.setOutlineColor(sf::Color::Black);
	saveButton.setOutlineThickness(2);
	statsButton = Button(window.mapPixelToCoords(sf::Vector2i(window.getSize().x * 0.92, window.getSize().y * 0.95), uiView), { 50, 50 }, ft, "");
	statsButton.setTexture(&tx_stats);
	statsButton.setOutlineColor(sf::Color::Black);
	statsButton.setOutlineThickness(2);
	if (load)
		LoadGame();
}

Game::~Game()
{
	all.clear();
	ui.clear();
	farms.clear();
	linie.clear();
	std::cout << "Game destroyed" << std::endl;
	// window.close(); // Uncomment if you want to close the window when the game is destroyed
}

void Game::run() {
	while (window.isOpen()) {
		processEvents();
		update();
		render();
	}
}

void Game::processEvents() {
	while (const std::optional event = window.pollEvent()) {
		if (event->is<sf::Event::Closed>()) {
			all.clear();
			ui.clear();
			window.close();
		}
		if (statsBox != nullptr) {
			if (event->is<sf::Event::TextEntered>()) {
				char c;
				c = event->getIf<sf::Event::TextEntered>()->unicode;
				if (c == 13)
				{
					//enter
					if (!input.empty())
					{
						std::string line, output = "";
						std::transform(input.begin(), input.end(), input.begin(), [](char c) {return std::tolower(c); });
						std::istringstream isStats(stats);
						std::unordered_map<char, int> inputMap = przygotujMapeOstatnichWystapien(input);
						while (std::getline(isStats, line))
						{
							std::string origial = line;
							std::transform(line.begin(), line.end(), line.begin(), [](char c) {return std::tolower(c); });
							if (szukajWzorcaBoyerMoore(line, input, inputMap))
								output += origial + "\n"; // Append original line to output if it matches the input pattern
						}
						std::cout << "Searching for: " << input << std::endl;
						std::cout << output << std::endl;
						if (!output.empty())
						{
							statsBox->TextView.TextUpdate(output);
						}
					}
					else
					{
						statsBox->TextView.TextUpdate(stats);
					}
					input = "";
					statsBox->Input.TextUpdate(input, true, true); // Clear input box
				}
				else if (c == 27)
				{
					//esc
					delete statsBox;
					statsBox = nullptr;
					input = "";
					sf::sleep(sf::milliseconds(250));
				}
				else if (c == 8)
				{
					if (input.size() > 0)
					{
						input.pop_back();
						statsBox->Input.TextUpdate(input, true, true);
					}
				}
				else
				{
					input += c;
					statsBox->Input.TextUpdate(input, true);
				}
				sf::sleep(sf::microseconds(250));
			}
		}
	}
}

void Game::update() {
	if (!window.hasFocus()) return;
	handleMouseInput();
	handleKeyboardInput();
}
void Game::UpdateAll()
{
	all.clear();
	for (int i = 0; i < convexes.size(); i++)
		all.push_back(&convexes[i]);
	for (int i = 0; i < linie.size(); i++)
		all.push_back(&linie[i]);
	for (int i = 0; i < farms.size(); i++)
		all.push_back(&farms[i]);

}


void Game::render(sf::RectangleShape* obj) {
	//UpdateAll();
	//draw(ui, all, window, view, uiView);

	window.clear(sf::Color::White);
	window.setView(uiView);
	window.draw(background);

	window.setView(view);
	for (size_t i = 0; i < convexes.size(); i++)
	{
		window.draw(convexes[i]);
	}
	for (size_t i = 0; i < linie.size(); i++)
	{
		window.draw(linie[i]);
	}
	for (size_t i = 0; i < farms.size(); i++)
	{
		window.draw(farms[i]);
	}
	if (obj != nullptr)
		window.draw(*obj);

	window.setView(uiView);
	for (size_t i = 0; i < buildButtons.size(); i++)
	{
		buildButtons[i].draw(window, uiView);
	}
	cash_txt.draw(window, uiView);
	ale_txt.draw(window, uiView);

	if (statsBox != nullptr) {
		statsBox->draw(window, uiView, statsButton.getPosition());
	}
	if (message != nullptr) {
		message->draw(window, uiView);
	}
	if (upgrade != nullptr) {
		upgrade->draw(window, uiView);
	}
	statsButton.draw(window, uiView);
	saveButton.draw(window, uiView);
	turnButton.draw(window, uiView);

	window.setView(view);
	window.display();
	sf::sleep(sf::milliseconds(50));

}

void Game::handleMouseInput() {

	if (message != nullptr) {
		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
			if (message->isMouseOver(MousePosView(window, uiView))) {
				delete message;
				message = nullptr;
				sf::sleep(sf::milliseconds(250));
			}
		}
		return;
	}
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
		if (ButtonHandler())
			return;
		if (placeMode == 0)
		{
			int tmp = HoverOverFarm(window, view, farms);
			if (tmp != -1) {
				if (farms[tmp].getType() == NodeType::Crossroad) return; // Cannot upgrade crossroads
				std::string up_str = "Used capacity: " + std::to_string(farms[tmp].getUsed()) +
					"\nFarm capacity: " + std::to_string(farms[tmp].getCapacity()) + "->" + std::to_string(size_t(farms[tmp].getCapacity() * 1.2));

				if (farms[tmp].getType() == NodeType::Farm)
					cost = 200;
				else
					cost = 150;
				upgrade_id = tmp;
				upgrade_type = 1;
				upgrade = new UpgradeBox(window.mapPixelToCoords(sf::Vector2i(window.getSize().x * 0.5, 0), uiView), { 0, 0 }, font, up_str, farms[tmp].getPosition(), farms[tmp].getSize());
				sf::sleep(sf::milliseconds(250));
				return;
			}
			sf::Vector2f mousePos = MousePosView(window, view);
			for (size_t i = 0; i < linie.size(); i++)
			{
				if (linie[i].getGlobalBounds().contains(mousePos))
					if (CursorNearLine(linie[i], mousePos, farms))
					{
						std::string up_str = "Used capacity:  1->2: " + std::to_string(linie[i].getUsed().first) + "\t 2->1: " + std::to_string(linie[i].getUsed().second) +
							"\nUpgrade capacity: "
							+ std::to_string(linie[i].getCapacity()) + "->" + std::to_string(size_t(linie[i].getCapacity() * 1.2)) +
							", Lower cost: $"
							+ std::to_string(linie[i].GetCost()) + "->" + std::to_string((linie[i].GetCost() > 0) ? (linie[i].GetCost() - 1) : 0);
						cost = 100;
						upgrade_id = i;
						upgrade_type = 0;
						upgrade = new UpgradeBox(window.mapPixelToCoords(sf::Vector2i(window.getSize().x * 0.5, 0), uiView), { 0, 0 }, font, up_str, &farms, linie[i].startNode, linie[i].endNode);
						sf::sleep(sf::milliseconds(250));
						return;
					}
			}
		}
		if (placeMode == 1) {
			if (cost > cash) {
				for (int i = 0; i < buildButtons.size(); i++)
				{
					buildButtons[i].ResetTx();
					placeMode = 0;
				}
				return;
			}
			int tmp;
			sf::Vector2f cp;
			bool L2L = false;
			Line linia;
			if ((tmp = HoverOverFarm(window, view, farms)) != -1)
			{
				linia = Line({ float(farms[tmp].getPosition().x), float(farms[tmp].getPosition().y) });
				linia.startNode = tmp;
			}
			else
			{
				if (linie.size() == 0) return;
				cp = closestPointOnLine(farms[linie[0].startNode].getPosition(), farms[linie[0].endNode].getPosition(), MousePosView(window, view));
				MemCP mem(-1, INFINITY, { 0,0 });
				for (int i = 0; i < linie.size(); i++)
				{
					if (!CursorNearLine(linie[i], MousePosView(window, view), farms)) continue;
					cp = closestPointOnLine(farms[linie[i].startNode].getPosition(), farms[linie[i].endNode].getPosition(), MousePosView(window, view));
					if (dl(cp, MousePosView(window, view)) < mem.dist)
					{
						mem = MemCP(i, dl(cp, MousePosView(window, view)), cp);
					}
				}
				if (mem.i == -1) return;
				Node crossroad(mem.pos);
				crossroad.setTexture(&tx_cross);
				farms.push_back(crossroad);
				linia = Line(crossroad.getPosition());
				linia.startNode = farms.size() - 1;
				linia.startLine = mem.i;
				L2L = true;
			}
			while (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
				//sf::sleep(sf::milliseconds(100));
				linia.Reset();
				linia.FollowMouse(MousePosView(window, view));
				tmp = HoverOverFarm(window, view, farms, (int)linia.startNode);

				if (tmp != -1)
				{
					linia.ConnectToNode(farms, tmp);
				}
				if (tmp == -1 && !(linia.SnapToLine(linie, farms, MousePosView(window, view))))
				{
					linia.CanPlace = true;
					linia.freePlace = true;
				}

				if (linia.CanPlace)
				{
					if (linia.freePlace) {
						Node tmp = Node(linia.getPosEnd());
						if (checkIntersection(linie, tmp, farms))
						{
							linia.SetRed();
							linia.CanPlace = false;
						}

					}

					if (checkIntersection(linie, linia, farms))
					{
						linia.SetRed();
						linia.CanPlace = false;
					}
				}
				render(&linia);
				// std::cout << "Creating line with start node: " << linia.startNode << std::endl; // Debugging line

			}
			if (linia.CanPlace)
			{
				if (linia.freePlace)
				{
					Node tmp = Node(linia.getPosEnd());
					tmp.setTexture(&tx_cross);
					farms.push_back(tmp);
					linia.ConnectToNode(farms, farms.size() - 1);

				}
				if (L2L)
				{
					if (linia.startLine < linia.CollideWith)
					{
						linia.CollideWith -= 1;
					}
					splitLine(linie[linia.startLine], linie, (int)linia.startNode, linia.startLine, farms);
				}
				if (linia.CollideWith != -1)
				{
					splitLine(linia.getPosEnd(), linie[linia.CollideWith], linie, linia.CollideWith, farms, tx_cross);
					linia.endNode = farms.size() - 1;
				}
				// std::cout << "Setting line color to white." << std::endl; // Debugging line

				linia.ConnectToNode(farms);
				linia.setFillColor(sf::Color(255, 255, 255, 255));
				/* linia.setOutlineColor(sf::Color::Black);
				 linia.setOutlineThickness(2);*/
				linia.setCapacity();
				linia.setTexture(&tx_road);
				linie.push_back(linia);
				cash -= cost;
				cash_txt.TextUpdate("Bank: $" + std::to_string(cash));
				saveButton.setFillColor(sf::Color(34, 118, 255));
				//   std::cout << "Capacity: " << linia.getCapacity() << std::endl;

				//sf::sleep(sf::milliseconds(100));
				//std::cout << linie.size() << std::endl; // Debugging line

			}
			else
			{
				if (L2L)
				{
					farms.pop_back();
				}
				//std::cout << "Cannot place line, color not set to white." << std::endl; // Debugging line
			}
			std::cout << farms.size() << std::endl;

		}
		else if (placeMode > 1)
		{
			bool canPlace = false;
			Node farm(window, placeType);
			switch (placeMode)
			{
			case 2:
				farm.setTexture(&tx_farm);
				break;
			case 3:
				farm.setTexture(&tx_ale);
				break;
			case 4:
				farm.setTexture(&tx_tav);
				break;
			default:
				break;
			}
			while (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
			{
				farm.setPosition(MousePosView(window, view));
				render(&farm);
				if (checkIntersection(linie, farm, farms))
				{
					farm.setFillColor(sf::Color(255, 0, 0, 160));
					canPlace = false;
				}
				else
				{
					canPlace = true;
					farm.setFillColor(sf::Color(155, 255, 58));
					if (farm.type == NodeType::Farm)
					{
						bool fertile = false;
						for (int i = 0; i < convexes.size(); i++)
						{
							if (convexes[i].Contains(farm.getPosition()))
							{
								std::cout << "Farm placed inside convex hull." << std::endl;
								farm.setFillColor(sf::Color::Green);
								fertile = true;
								break;
							}
						}
						farm.setFertile(fertile);
					}

				}
			}
			if (canPlace) {
				farm.setFillColor(sf::Color::White);
				farm.setCapacity();
				farms.push_back(farm);
				saveButton.setFillColor(sf::Color(34, 118, 255));
				cash -= cost;
				cash_txt.TextUpdate("Bank: $" + std::to_string(cash));

			}
		}
		//sf::sleep(sf::milliseconds(100));

	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
		// Logika dla prawego przycisku myszy
		 /*sf::Vector2f cr = MousePosView(window, view);
			sf::sleep(sf::milliseconds(10));
			sf::Vector2f mv = MousePosView(window, view) - cr;
			for (int i = 0; i < all.size(); i++)
			{
				all[i]->move(sf::Vector2f(mv));
			}
			sf::sleep(sf::milliseconds(100));*/
		for (int i = 0; i < linie.size(); i++)
		{
			if (linie[i].getGlobalBounds().contains(MousePosView(window, view)))
			{
				if (CursorNearLine(linie[i], MousePosView(window, view), farms))
				{
					size_t tmpC, tmpK;
					std::cout << "Line " << i << " (" << linie[i].startNode << "," << linie[i].endNode << "): " << linie[i].getCapacity() << ", " << linie[i].GetCost() << ": ";
					std::cin >> tmpC >> tmpK;
					linie[i].setCapacity(tmpC);
					linie[i].SetCost(tmpK);
				}
			}
		}
		for (int i = 0; i < linie.size(); i++)
		{
			std::cout << "Line " << i << " (" << linie[i].startNode << "," << linie[i].endNode << "): " << linie[i].getCapacity() << ", " << linie[i].GetCost() << std::endl;
		}
	}
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Middle)) {
		sf::Vector2f current = MousePosView(window, view);
		if (!dragging) {
			lastMousePos = current;
			dragging = true;
		}
		sf::Vector2f delta = current - lastMousePos;
		for (int i = 0; i < all.size(); i++) {
			all[i]->move(delta);
		}
		lastMousePos = current;
	}
	else {
		dragging = false;
	}
}

void Game::handleKeyboardInput() {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Add)) {
		view.zoom(0.98f);
		window.setView(view);
		sf::sleep(sf::milliseconds(50));
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Subtract)) {
		view.zoom(1.02f);
		window.setView(view);
		sf::sleep(sf::milliseconds(50));
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
		if (statsBox != nullptr) {
			statsBox->move(10);
		}
		sf::sleep(sf::milliseconds(50));
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
		if (statsBox != nullptr) {
			statsBox->move(-10);
		}
		sf::sleep(sf::milliseconds(50));
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
		all.clear();
		ui.clear();
		window.close();
	}
}

void Game::SaveGame()
{
	std::ostringstream oss;
	oss << convexes.size();
	for (size_t i = 0; i < convexes.size(); i++)
	{
		Convex& convex = convexes[i];
		oss << std::endl << convex.getPosition().x << " " << convex.getPosition().y << " ";
		oss << convex.getPointCount() << " ";
		for (size_t j = 0; j < convex.getPointCount(); j++) {
			const auto& point = convex.getPoint(j);
			oss << point.x << " " << point.y << " ";
		}
	}
	oss << std::endl << farms.size();
	for (size_t i = 0; i < farms.size(); i++)
	{
		Node& node = farms[i];
		oss << std::endl << node.getPosition().x << " " << node.getPosition().y << " " << node.getCapacity();
		if (node.type == NodeType::Farm) {
			oss << " F";
		}
		else if (node.type == NodeType::Tavern) {
			oss << " T";
		}
		else if (node.type == NodeType::Alehouse) {
			oss << " A";
		}
		else {
			oss << " C"; // Crossroad
		}
	}
	oss << std::endl << linie.size();
	for (size_t i = 0; i < linie.size(); i++)
	{
		Line& line = linie[i];
		oss << std::endl << line.startNode << " " << line.endNode << " " << line.getCapacity() << " " << line.GetCost();
	}
	oss << std::endl << turn;
	oss << std::endl << cash;
	oss << std::endl << quota;
	oss << std::endl << stats;
	

	std::string gameData = oss.str();
	compressString(gameData, "save.txt");

	std::cout << "Game saved successfully (compressed)." << std::endl;
}

void Game::LoadGame()
{
	std::string decompressedData = decompressString("save.txt");
	std::istringstream file(decompressedData);
	if (file) {
		size_t convexCount;
		file >> convexCount;
		convexes.clear();
		for (size_t i = 0; i < convexCount; i++) {
			sf::Vector2f pos;
			file >> pos.x >> pos.y;
			size_t pointCount;
			file >> pointCount;
			std::vector<sf::Vector2f> points;
			for (size_t j = 0; j < pointCount; j++) {
				sf::Vector2f point;
				file >> point.x >> point.y;
				points.push_back(point);
			}
			convexes.push_back(Convex(pos, points));
		}
		std::cout << "Loaded " << convexes.size() << " convexes." << std::endl;
		size_t farmCount;
		file >> farmCount;
		farms.clear();
		for (size_t i = 0; i < farmCount; i++) {
			sf::Vector2f pos;
			size_t capacity;
			char typeChar;
			file >> pos.x >> pos.y >> capacity >> typeChar;
			Node node(window);
			switch (typeChar)
			{
			case 'F':
				node.setType(NodeType::Farm);
				node.setTexture(&tx_farm);
				break;
			case 'T':
				node.setType(NodeType::Tavern);
				node.setTexture(&tx_tav);
				break;
			case 'A':
				node.setType(NodeType::Alehouse);
				node.setTexture(&tx_ale);
				break;
			case 'C':
				node = Node(pos); // Crossroad
				node.setTexture(&tx_cross);
				break;
			}
			node.setPosition(pos);
			node.setCapacity(capacity);
			node.setFillColor(sf::Color::White);
			farms.push_back(node);
		}
		std::cout << "Loaded " << farms.size() << " farms." << std::endl;
		size_t lineCount;
		file >> lineCount;
		linie.clear();
		for (size_t i = 0; i < lineCount; i++) {
			size_t startNode, endNode;
			size_t capacity, cost;
			file >> startNode >> endNode >> capacity >> cost;
			Line line(farms[startNode].getPosition());
			line.ConnectToNode(farms, startNode, endNode);
			line.setCapacity(capacity);
			line.SetCost(cost);
			line.setFillColor(sf::Color(255, 255, 255, 255));
			line.startNode = startNode;
			line.endNode = endNode;
			line.setTexture(&tx_road);
			linie.push_back(line);
		}
		std::cout << "Loaded " << linie.size() << " lines." << std::endl;
		file >> turn;
		file >> cash;
		file >> quota;
		std::getline(file, stats); // consume the newline after turn
		stats.clear();
		while (file) {
			std::string s;
			std::getline(file, s);
			if (!s.empty())
				stats += s + '\n';
		}
		if (!stats.empty())
			stats[stats.size() - 1] = '\0'; // Remove last newline character
		std::cout << "Game loaded successfully." << std::endl;
		std::cout << cash << " " << quota << std::endl;

		std::cout << "Cash: $" << cash << ", Quota: " << quota << " barrels." << std::endl;
		cash_txt.TextUpdate("Bank: $" + std::to_string(cash));
		ale_txt.TextUpdate("Quota: " + std::to_string(quota) + " barrels");
	}
}

bool Game::ButtonHandler()
{
	if (buildButtons[0].isMouseOver(MousePosView(window, view)))
	{
		if (placeMode == 1)
			placeMode = 0; // Switch back to no placement mode
		else
		{
			placeMode = 1; // Switch to road placement mode
			cost = 100;
			if (cost > cash)
			{
				sf::sleep(sf::milliseconds(250));
				return true; // Not enough cash
			}
		}
		std::cout << "Change to road " << std::endl;
		for (int i = 1; i < buildButtons.size(); i++)
		{
			buildButtons[i].ResetTx();
		}
		buildButtons[0].changeTx();
		cost = 100;
		sf::sleep(sf::milliseconds(250));
		return true;
	}

	if (buildButtons[1].isMouseOver(MousePosView(window, view)))
	{
		if (placeMode == 2)
		{
			placeMode = 0; // Switch back to no placement mode
		}
		else
		{
			cost = 200;
			if (cost > cash)
			{
				sf::sleep(sf::milliseconds(250));
				return true; // Not enough cash
			}
			placeMode = 2; // Switch to farm placement mode
			placeType = NodeType::Farm; // Set the type to Farm
		}
		buildButtons[1].changeTx();
		for (int i = 0; i < buildButtons.size(); i++)
		{
			if (i == 1) continue; // Skip the farm button
			buildButtons[i].ResetTx();
		}
		sf::sleep(sf::milliseconds(250));
		return true;

	}
	if (buildButtons[2].isMouseOver(MousePosView(window, view)))
	{
		if (placeMode == 3)
		{
			placeMode = 0; // Switch back to no placement mode
		}
		else
		{
			cost = 150;
			if (cost > cash)
			{
				sf::sleep(sf::milliseconds(250));
				return true; // Not enough cash
			}
			placeMode = 3; // Switch to tavern placement mode
			placeType = NodeType::Alehouse; // Set the type to Tavern
		}
		buildButtons[2].changeTx();
		for (int i = 0; i < buildButtons.size(); i++)
		{
			if (i == 2) continue; // Skip the tavern button
			buildButtons[i].ResetTx();
		}
		sf::sleep(sf::milliseconds(250));

		return true;
	}
	if (buildButtons[3].isMouseOver(MousePosView(window, view)))
	{
		if (placeMode == 4)
		{
			placeMode = 0; // Switch back to no placement mode
		}
		else
		{
			cost = 150;
			if (cost > cash)
			{
				sf::sleep(sf::milliseconds(250));
				return true; // Not enough cash
			}
			placeMode = 4; // Switch to alehouse placement mode
			placeType = NodeType::Tavern; // Set the type to Alehouse
		}
		buildButtons[3].changeTx();
		for (int i = 0; i < buildButtons.size(); i++)
		{
			if (i == 3) continue; // Skip the alehouse button
			buildButtons[i].ResetTx();
		}
		sf::sleep(sf::milliseconds(250));

		return true;

	}
	if (saveButton.isMouseOver(MousePosView(window, view)))
	{
		SaveGame();
		saveButton.setFillColor(sf::Color::Green);
		sf::sleep(sf::milliseconds(250));
		return true;

	}

	if (turnButton.isMouseOver(MousePosView(window, view)))
	{
		//for (int i = 0; i < farms.size(); i++)
		//	farms[i].capacity = 50; // Reset used capacity for all farms
		TurnEnd();
		if (statsBox != nullptr) {
			delete statsBox;
			statsBox = nullptr; // Clear the stats box
			input = ""; // Clear the input string
		}
		if (upgrade != nullptr) {
			delete upgrade;
			upgrade = nullptr; // Clear the upgrade button
		}
		sf::sleep(sf::milliseconds(250));
		return true;
	}
	if (statsButton.isMouseOver(MousePosView(window, view)))
	{
		if (stats.empty())
		{
			sf::sleep(sf::milliseconds(250));
			return true;
		}
		/*convexes.push_back(Convex(all));*/
		if (statsBox == nullptr)
			statsBox = new TextBox({ 0,0 }, { 0,0 }, font, stats);
		else
		{
			input = ""; // Clear the input string
			delete statsBox;
			statsBox = nullptr; // Clear the stats box
		}
		sf::sleep(sf::milliseconds(250));
		return true;

	}
	if (upgrade != nullptr) {
		if (upgrade->upgradeButton.isMouseOver(MousePosView(window, uiView)) && cash>=cost)
		{
			if (upgrade_type == 0) {
				Line& line = linie[upgrade_id];
				line.setCapacity(size_t(line.getCapacity() * 1.2));
				line.SetCost((line.GetCost() == 0) ? 0 : line.GetCost() - 1);
			}
			else
			{
				Node& node = farms[upgrade_id];
				node.setCapacity(node.getCapacity() * 1.2);
			}
			saveButton.setFillColor(sf::Color(34, 118, 255)); // Reset save button color
			cash -= cost;
			cash_txt.TextUpdate("Bank: $" + std::to_string(cash));
			delete upgrade;
			upgrade = nullptr;
			return true; // Upgrade successful
		}
		else if (!upgrade->upgradeButton.isMouseOver(MousePosView(window, uiView))) {
			delete upgrade;
			upgrade = nullptr; // Clear the upgrade button
			return true;
		}
		else {
			return true;
		}
	}
	return false;

}
void Game::TurnEnd()
{
	FlowAlgorithm flow(farms.size());
	flow.MakeGraph(linie, farms);
	flow.printGraph();
	std::pair<size_t, size_t> delivery = flow.Calculate();
	if (delivery.first == 0)
	{
		message = new Button(window.mapPixelToCoords(sf::Vector2i(window.getSize().x / 2, window.getSize().y / 2), view), { 0,0 }, font, "Build at least one delivery path.\n(Farma->Browar->Tawerna)");
		message->SetPosAll(message->getPosition() - message->getSize() / 2.0f);
		message->setFillColor(sf::Color::Red);
		message->setOutlineColor(sf::Color::Black);
		message->setOutlineThickness(2);
		return;
	}
	if (delivery.first < quota)
	{
		message = new Button(window.mapPixelToCoords(sf::Vector2i(window.getSize().x / 2, window.getSize().y / 2), view), { 400,400 }, font, "You haven't delivered your quota.\n The residents consider you unfit to hold the office of mayor.\nYou're fired!");
		message->TextUpdate("You haven't fulfilled your quota.\n The residents consider you unfit to hold the office of mayor.\nYou're fired!", true, true, 29);
		message->setOutlineColor(sf::Color::Black);
		message->setOutlineThickness(2);
		message->SetPosAll(message->getPosition() - message->getSize() / 2.0f);
		message->setTexture(&tx_mob);
		return; // Not enough ale delivered
	}
	turn++;
	cash += delivery.first * 10; // Add the total delivery to cash
	cash -= delivery.second; // Subtract the total cost from cash
	if (cash < 0)
	{
		message = new Button(window.mapPixelToCoords(sf::Vector2i(window.getSize().x / 2, window.getSize().y / 2), view), { 400,400 }, font, "You bankrupted the city.\n The residents consider you unfit to hold the office of mayor.\nYou're fired!");
		message->TextUpdate("You bankrupted the city.\n The residents consider you unfit to hold the office of mayor.\nYou're fired!", true, true, 29);
		message->setOutlineColor(sf::Color::Black);
		message->setOutlineThickness(2);
		message->SetPosAll(message->getPosition() - message->getSize() / 2.0f);
		message->setTexture(&tx_mob);
		return; // Not enough cash
	}

	quota *= 1.35;
	for (size_t i = 0; i < linie.size(); i++)
	{
		std::pair<size_t, size_t> flowLine;
		flowLine.first = flow.checkLine(linie[i].endNode, linie[i].startNode);
		flowLine.second = flow.checkLine(linie[i].startNode, linie[i].endNode);
		linie[i].setUsed(flowLine);
	}

	// Update the cash text
	cash_txt.TextUpdate("Bank: $" + std::to_string(cash));
	ale_txt.TextUpdate("Quota: " + std::to_string(quota) + " barrels");
	size_t n = farms.size();
	std::cout << "Farms: " << n << std::endl;
	size_t max_wheat = 0, max_ale = 0, max_tavern = 0;
	for (size_t i = 0; i < n; i++)
	{

		Node& node = farms[i];
		if (node.getType() == NodeType::Farm)
		{
			node.setUsed(flow.checkBuilding(2 * n, i));
			max_wheat += node.getCapacity();
		}
		else if (node.getType() == NodeType::Tavern)
		{
			node.setUsed(flow.checkBuilding(i + n, 2 * n + 1));
			max_tavern += node.getCapacity();
		}
		else if (node.getType() == NodeType::Alehouse)
		{
			node.setUsed(flow.checkBuilding(i, i + n));
			max_ale += node.getCapacity();
		}
	}
	stats += "\nTurn: " + std::to_string(turn) + " Max Wheat: " + std::to_string(max_wheat) +
		"\nTurn: " + std::to_string(turn) + " Max Ale: " + std::to_string(max_ale) +
		"\nTurn: " + std::to_string(turn) + " Max Tavern: " + std::to_string(max_tavern) +
		"\nTurn: " + std::to_string(turn) + " Total Delivery: " + std::to_string(delivery.first) +
		"\nTurn: " + std::to_string(turn) + " Total Cost: " + std::to_string(delivery.second);
	printUsed();
	if (turn % 5 == 0)
	{
		UpdateAll();
		convexes.push_back(Convex(all));
		all.clear();
	}
	std::cout << stats << std::endl;
}

void Game::printUsed()
{
	for (size_t i = 0; i < farms.size(); i++)
	{
		if (farms[i].getType() == NodeType::Crossroad) continue; // Skip crossroads
		std::cout << "Farm " << i << ": " << farms[i].getUsed() << "/" << farms[i].getCapacity() << std::endl;
	}
	for (size_t i = 0; i < linie.size(); i++)
	{
		std::pair<size_t, size_t> used = linie[i].getUsed();
		if (used.first == 0 && used.second == 0) continue; // Skip lines with no flow
		std::cout << "Line " << i << ": " <<
			"\n\tto: " << used.first << "/" << linie[i].getCapacity() <<
			"\n\tfrom " << used.second << "/" << linie[i].getCapacity() << std::endl;
	}
}