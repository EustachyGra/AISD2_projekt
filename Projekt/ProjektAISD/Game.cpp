#include "Game.hpp"

Game::Game(sf::RenderWindow& window, sf::Font& ft, bool load)
	: window(window) {
	font = ft;
	view.setSize({ 1920,1080 });
	view.zoom(1.0f); // Initial zoom level
	uiView.setSize({ 1920,1080 });
	srand((unsigned int)time(NULL));


	if (!(tx_farm.loadFromFile("Textury/farm_p3.png"))) {
		throw std::string("Coulbn't load texture!\n\'Textury/farm_p3.png\'");
	}
	if (!(tx_tav.loadFromFile("Textury/tavern_p3.png"))) {
		throw std::string("Coulbn't load texture!\n\'Textury/tavern_p3.png\'");
	}
	if (!(tx_ale.loadFromFile("Textury/browar.png"))) {
		throw std::string("Coulbn't load texture!\n\'Textury/browar.png\'");
	}
	if (!(tx_road.loadFromFile("Textury/raodv4.png"))) {
		throw std::string("Coulbn't load texture!\n\'Textury/raodv2.png\'");
	}
	if (!(tx_cross.loadFromFile("Textury/cross.png"))) {
		throw std::string("Coulbn't load texture!\n\'Textury/cross.png\'");
	}
	if (!(tx_cancel.loadFromFile("Textury/canclev2.png"))) {
		throw std::string("Coulbn't load texture!\n\'Textury/canclev2.png\'");
	}
	if (!(tx_stats.loadFromFile("Textury/stats.png"))) {
		throw std::string("Coulbn't load texture!\n\'Textury/stats.png\'");
	}
	if (!(tx_save.loadFromFile("Textury/save.png"))) {
		throw std::string("Coulbn't load texture!\n\'Textury/save.png\'");
	}
	if (!(tx_turn.loadFromFile("Textury/turn.png"))) {
		throw std::string("Coulbn't load texture!\n\'Textury/turn.png\'");
	}
	if (!(tx_mob.loadFromFile("Textury/mob.png"))) {
		throw std::string("Coulbn't load texture!\n\'Textury/mob.png\'");
	}
	if (!(bg_texture.loadFromFile("Textury/grass3.png")))
	{
		throw std::string("Coulbn't load texture!\n'Textury/mob.png");
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


	buildButtons.push_back(Button(window.mapPixelToCoords(sf::Vector2i(window.getSize().x * 0.02, window.getSize().y * 0.9), uiView), { 50,50 }, ft, "Road", "$100", tx_road, tx_cancel));
	buildButtons.push_back(Button(buildButtons[0].getPosition() + sf::Vector2f(60, -25), buildButtons[0].getSize(), ft, "Farm", "$200", tx_farm, tx_cancel));
	buildButtons.push_back(Button(buildButtons[1].getPosition() + sf::Vector2f(60, -25), { 50,50 }, ft, "Alehouse", "$150", tx_ale, tx_cancel));
	buildButtons.push_back(Button(buildButtons[2].getPosition() + sf::Vector2f(60, -25), { 50,50 }, ft, "Tavern", "$150", tx_tav, tx_cancel));



	turnButton = Button(window.mapPixelToCoords(sf::Vector2i(window.getSize().x * 0.95, window.getSize().y * 0.02), uiView), { 75, 75 }, ft, "");
	turnButton.setTexture(&tx_turn);
	turnButton.setOutlineColor(sf::Color::Black);
	turnButton.setOutlineThickness(2);
	saveButton = Button(window.mapPixelToCoords(sf::Vector2i(window.getSize().x * 0.93, window.getSize().y * 0.95), uiView), { 50, 50 }, ft, "");
	saveButton.setTexture(&tx_save);
	saveButton.setFillColor(sf::Color(34, 118, 255));
	saveButton.setOutlineColor(sf::Color::Black);
	saveButton.setOutlineThickness(2);
	statsButton = Button(window.mapPixelToCoords(sf::Vector2i(window.getSize().x * 0.90, window.getSize().y * 0.95), uiView), { 50, 50 }, ft, "");
	statsButton.setTexture(&tx_stats);
	statsButton.setOutlineColor(sf::Color::Black);
	statsButton.setOutlineThickness(2);
	endButton = Button(window.mapPixelToCoords(sf::Vector2i(window.getSize().x * 0.96, window.getSize().y * 0.95), uiView), { 50, 50 }, ft, "Quit");
	endButton.setFillColor(sf::Color::Red);
	endButton.setOutlineColor(sf::Color::Black);
	endButton.setOutlineThickness(2);
	if (load)
		LoadGame();
}

Game::~Game()
{
	all.clear();
	ui.clear();
	farms.clear();
	linie.clear();
	convexes.clear();
	delete statsBox;
	delete message;
	delete upgrade;
	delete newLine;
	delete newNode;
	std::cout << "Game destroyed" << std::endl;
	// window.close(); // Uncomment if you want to close the window when the game is destroyed
}

void Game::run() {
	while (window.isOpen() and !endGame) {
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
						std::vector<int> goodSuffix = buildGoodSuffix(input);
						std::unordered_map<char, int> badCharMap = buildBadChar(input);
						while (std::getline(isStats, line))
						{
							std::string origial = line;
							std::transform(line.begin(), line.end(), line.begin(), [](char c) {return std::tolower(c); });
							if (boyerMoore(line, input, badCharMap, goodSuffix))
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
	if (newLine != nullptr)
	{
		window.draw(*newLine);
	}
	if (newNode != nullptr)
	{
		window.draw(*newNode);
	}
	if (obj != nullptr)
		window.draw(*obj);

	window.setView(uiView);
	cash_txt.draw(window, uiView);
	ale_txt.draw(window, uiView);

	if (statsBox != nullptr) {
		statsBox->draw(window, uiView, statsButton.getPosition());
	}
	if (message != nullptr) {
		message->draw(window, uiView);
	}
	if (upgrade != nullptr) {
		upgrade->draw(window, uiView, view);
	}
	statsButton.draw(window, uiView);
	endButton.draw(window, uiView);
	if (lost == false) {
		for (size_t i = 0; i < buildButtons.size(); i++)
		{
			buildButtons[i].draw(window, uiView);
		}
		saveButton.draw(window, uiView);
		turnButton.draw(window, uiView);
	}
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
		LeftMouseClick();
	}
	else
	{
		if (newLine != nullptr)
		{
			if (newLine->CanPlace) {
				if (newLine->freePlace)
				{
					Node tmp = Node(newLine->getPosEnd());
					tmp.setTexture(&tx_cross);
					farms.push_back(tmp);
					newLine->ConnectToNode(farms, farms.size() - 1);

				}
				if (newLine->L2L)
				{
					if (newLine->startLine < newLine->CollideWith)
					{
						newLine->CollideWith -= 1;
					}
					splitLine(linie[newLine->startLine], linie, (int)newLine->startNode, newLine->startLine, farms);
				}
				if (newLine->CollideWith != -1)
				{
					splitLine(newLine->getPosEnd(), linie[newLine->CollideWith], linie, newLine->CollideWith, farms, tx_cross);
					newLine->endNode = farms.size() - 1;
				}
				// std::cout << "Setting line color to white." << std::endl; // Debugging line

				newLine->ConnectToNode(farms);
				newLine->setFillColor(sf::Color::White);
				newLine->SetCost(1 + rand() % 5);
				newLine->setCapacity();
				newLine->setTexture(&tx_road);
				linie.push_back(*newLine);
				cash -= cost;
				cash_txt.TextUpdate("Bank: $" + std::to_string(cash));
				saveButton.setFillColor(sf::Color(34, 118, 255));
				//   std::cout << "Capacity: " << linia.getCapacity() << std::endl;

				//sf::sleep(sf::milliseconds(100));
				//std::cout << linie.size() << std::endl; // Debugging line

			}
			else
			{
				if (newLine->L2L)
				{
					farms.pop_back();
				}
				//std::cout << "Cannot place line, color not set to white." << std::endl; // Debugging line
			}
			delete newLine;
			newLine = nullptr;
		}
		if (newNode != nullptr)
		{
			if (newNode->canPlace) {
				newNode->setFillColor(sf::Color::White);
				newNode->setCapacity();
				farms.push_back(*newNode);
				saveButton.setFillColor(sf::Color(34, 118, 255));
				cash -= cost;
				cash_txt.TextUpdate("Bank: $" + std::to_string(cash));

			}
			delete newNode;
			newNode = nullptr;
		}
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
		sf::Vector2f current = MousePosView(window, view);
		if (!dragging) {
			lastMousePos = current;
			dragging = true;
		}
		sf::Vector2f delta = current - lastMousePos;
		UpdateAll();
		for (int i = 0; i < all.size(); i++) {
			all[i]->move(delta);
		}
		if (newLine != nullptr) {
			newLine->move(delta);
		}
		if (upgrade != nullptr) {
			upgrade->move(delta);
		}
		lastMousePos = current;

	}
	else {
		dragging = false;
	}
}

void Game::LeftMouseClick()
{
	if (ButtonHandler())
		return;
	if (placeMode == 0) //upgrade
	{
		int tmp = HoverOverFarm(window, view, farms);
		if (tmp != -1) {
			if (farms[tmp].getType() == NodeType::Crossroad) return; // Cannot upgrade crossroads
			std::string up_str = "Used capacity: " + std::to_string(farms[tmp].getUsed()) + "/" + std::to_string(farms[tmp].capacity) +
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
					std::string up_str = "Used capacity:  1->2: " + std::to_string(linie[i].getUsed().first) + "/" + std::to_string(linie[i].getCapacity()) + "\t 2->1: " + std::to_string(linie[i].getUsed().second) + "/" + std::to_string(linie[i].getCapacity()) +
						"\nUpgrade capacity: "
						+ std::to_string(linie[i].getCapacity()) + "->" + std::to_string(size_t(linie[i].getCapacity() * 1.2)) +
						", Lower cost: $"
						+ std::to_string(linie[i].GetCost()) + "->" + std::to_string((linie[i].GetCost() > 1) ? (linie[i].GetCost() - 1) : 1);
					cost = 100;
					upgrade_id = i;
					upgrade_type = 0;
					upgrade = new UpgradeBox(window.mapPixelToCoords(sf::Vector2i(window.getSize().x * 0.5, 0), uiView), { 0, 0 }, font, up_str, &farms, linie[i].startNode, linie[i].endNode);
					sf::sleep(sf::milliseconds(250));
					return;
				}
		}
	}
	//linia
	if (placeMode == 1 && newLine == nullptr) {
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
		if ((tmp = HoverOverFarm(window, view, farms)) != -1)
		{
			newLine = new Line({ float(farms[tmp].getPosition().x), float(farms[tmp].getPosition().y) });
			newLine->startNode = tmp;
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
			newLine = new Line(crossroad.getPosition());
			newLine->startNode = farms.size() - 1;
			newLine->startLine = mem.i;
			newLine->L2L = true;
		}
	}
	else if (placeMode == 1 && newLine != nullptr)
	{			//sf::sleep(sf::milliseconds(100));
		newLine->Reset();
		newLine->FollowMouse(MousePosView(window, view));
		int tmp = HoverOverFarm(window, view, farms, (int)newLine->startNode);

		if (tmp != -1)
		{
			newLine->ConnectToNode(farms, tmp);
		}
		if (tmp == -1 && !(newLine->SnapToLine(linie, farms, MousePosView(window, view))))
		{
			newLine->CanPlace = true;
			newLine->freePlace = true;
		}

		if (newLine->CanPlace)
		{
			if (newLine->freePlace) {
				Node tmp_n = Node(newLine->getPosEnd());
				if (checkIntersection(linie, tmp_n, farms))
				{
					newLine->SetRed();
					newLine->CanPlace = false;
				}

			}

			if (checkIntersection(linie, *newLine, farms))
			{
				newLine->SetRed();
				newLine->CanPlace = false;
			}
		}
		// std::cout << "Creating line with start node: " << linia.startNode << std::endl; // Debugging line

	}



	if (placeMode > 1 && newNode == nullptr)
	{
		newNode = new Node(window, placeType);
		switch (placeMode)
		{
		case 2:
			newNode->setTexture(&tx_farm);
			break;
		case 3:
			newNode->setTexture(&tx_ale);
			break;
		case 4:
			newNode->setTexture(&tx_tav);
			break;
		default:
			break;
		}
	}
	if (placeMode > 1 && newNode != nullptr)
	{
		newNode->setPosition(MousePosView(window, view));
		if (checkIntersection(linie, *newNode, farms))
		{
			newNode->setFillColor(sf::Color(255, 0, 0, 160));
			newNode->canPlace = false;
		}
		else
		{
			newNode->canPlace = true;
			newNode->setFillColor(sf::Color(155, 255, 58));
			if (newNode->type == NodeType::Farm)
			{
				bool fertile = false;
				for (int i = 0; i < convexes.size(); i++)
				{
					if (convexes[i].Contains(newNode->getPosition()))
					{
						std::cout << "Farm placed inside convex hull." << std::endl;
						newNode->setFillColor(sf::Color::Green);
						fertile = true;
						break;
					}
				}
				newNode->setFertile(fertile);
			}

		}
	}
	//sf::sleep(sf::milliseconds(100));

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
		oss << std::endl << node.getPosition().x << " " << node.getPosition().y << " " << node.getCapacity() << " " << node.getUsed();
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
		std::pair<size_t, size_t> used = line.getUsed();
		oss << std::endl << line.startNode << " " << line.endNode << " " << line.getCapacity() << " " << line.GetCost() << " " << used.first << " " << used.second;
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
			size_t capacity, used;
			char typeChar;
			file >> pos.x >> pos.y >> capacity >> used >> typeChar;
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
			node.setUsed(used);
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
			std::pair<size_t, size_t> used;
			file >> startNode >> endNode >> capacity >> cost >> used.first >> used.second;
			Line line(farms[startNode].getPosition());
			line.ConnectToNode(farms, startNode, endNode);
			line.setCapacity(capacity);
			line.SetCost(cost);
			line.setUsed(used);
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
} // hufman

bool Game::ButtonHandler()
{
	if (statsButton.isMouseOver(MousePosView(window, uiView)))
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
	if (endButton.isMouseOver(MousePosView(window, uiView)))
	{
		endGame = true; // Set the flag to end the game
		return true;

	}
	if (lost == true)
		return true;
	if (buildButtons[0].isMouseOver(MousePosView(window, uiView)))
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

	if (buildButtons[1].isMouseOver(MousePosView(window, uiView)))
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
	if (buildButtons[2].isMouseOver(MousePosView(window, uiView)))
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
	if (buildButtons[3].isMouseOver(MousePosView(window, uiView)))
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
	if (saveButton.isMouseOver(MousePosView(window, uiView)))
	{
		SaveGame();
		saveButton.setFillColor(sf::Color::Green);
		sf::sleep(sf::milliseconds(250));
		return true;

	}

	if (turnButton.isMouseOver(MousePosView(window, uiView)))
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
	
	if (upgrade != nullptr) {
		if (upgrade->upgradeButton.isMouseOver(MousePosView(window, uiView)) && cash >= cost)
		{
			if (upgrade_type == 0) {
				Line& line = linie[upgrade_id];
				line.setCapacity(size_t(line.getCapacity() * 1.2));
				line.SetCost((line.GetCost() == 1) ? 1 : line.GetCost() - 1);
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
	//flow.printGraph();
	flow.CalculatePotential();
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
		lost = true;
		message = new Button(window.mapPixelToCoords(sf::Vector2i(window.getSize().x / 2, window.getSize().y / 2), view), { 400,400 }, font, "You haven't delivered your quota.\n The residents consider you unfit to hold the office of mayor.\nYou're fired!");
		message->text2->setOutlineThickness(4);
		message->TextUpdate("You haven't fulfilled your quota.\n The residents consider you unfit to hold the office of mayor.\nYou're fired!", true, true, 29);
		message->setOutlineColor(sf::Color::Black);
		message->setOutlineThickness(2);
		message->SetPosAll(message->getPosition() - message->getSize() / 2.0f);
		message->setTexture(&tx_mob);
		return; // Not enough ale delivered
	}
	long long tmp_cash = cash; // Temporary cash to check for bankruptcy;
	tmp_cash += delivery.first * 9;
	tmp_cash -= delivery.second;
	if (tmp_cash < 0)
	{
		lost = true;
		cash_txt.TextUpdate("Bank: $" + std::to_string(tmp_cash));
		message = new Button(window.mapPixelToCoords(sf::Vector2i(window.getSize().x / 2, window.getSize().y / 2), view), { 400,400 }, font, "You bankrupted the city.\n The residents consider you unfit to hold the office of mayor.\nYou're fired!");
		message->text2->setOutlineThickness(4);
		message->TextUpdate("You bankrupted the city.\n The residents consider you unfit to hold the office of mayor.\nYou're fired!", true, true, 29);
		message->setOutlineColor(sf::Color::Black);
		message->setOutlineThickness(2);
		message->SetPosAll(message->getPosition() - message->getSize() / 2.0f);
		message->setTexture(&tx_mob);
		return; // Not enough cash
	}
	else
	{
		std::string output = "You have successfully delivered " + std::to_string(delivery.first) + " barrels of ale.\n" +
			"You have earned $" + std::to_string(delivery.first * 11) + " but spent $" + std::to_string(delivery.second) + " on roads maintenance.\n" +
			"You have fulfilled your quota of " + std::to_string(quota) + " barrels.";
		message = new Button(window.mapPixelToCoords(sf::Vector2i(window.getSize().x / 2, window.getSize().y / 2), view), { 0,400 }, font, output);
		message->setOutlineColor(sf::Color::Yellow);
		message->setOutlineThickness(2);
		message->setFillColor(sf::Color::Blue);
		message->SetPosAll(message->getPosition() - message->getSize() / 2.0f);
	}

	turn++;
	cash = tmp_cash;
	quota *= 1.35;
	for (size_t i = 0; i < linie.size(); i++)
	{
		std::pair<size_t, size_t> flowLine;
		flowLine.first = flow.checkLine(linie[i].endNode, linie[i].startNode);
		flowLine.second = flow.checkLine(linie[i].startNode, linie[i].endNode);
		linie[i].setUsed(flowLine);
		if(linie[i].getUsed().first > linie[i].getCapacity() || linie[i].getUsed().second > linie[i].getCapacity())
		{
			std::cout << "Warning: Line " << i << " from "<< linie[i].startNode << " to " << linie[i].endNode << "has used capacity exceeding its limit : " 
				<< linie[i].getUsed().first << "/" << linie[i].getCapacity() << ", " 
				<< linie[i].getUsed().second << "/" << linie[i].getCapacity() << std::endl;
		}
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
		if(node.getUsed() > node.getCapacity())
		{
			std::cout << "Warning: Node " << i << " has used capacity exceeding its limit : " 
				<< node.getUsed() << "/" << node.getCapacity() << std::endl;
		}
	}
	stats += "\nTurn: " + std::to_string(turn) + " Max Wheat: " + std::to_string(max_wheat) +
		"\nTurn: " + std::to_string(turn) + " Max Ale: " + std::to_string(max_ale) +
		"\nTurn: " + std::to_string(turn) + " Max Tavern: " + std::to_string(max_tavern) +
		"\nTurn: " + std::to_string(turn) + " Total Delivery: " + std::to_string(delivery.first) +
		"\nTurn: " + std::to_string(turn) + " Total Cost: " + std::to_string(delivery.second);
	//printUsed();
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


