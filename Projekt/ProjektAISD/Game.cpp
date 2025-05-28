#include "Game.hpp"

Game::Game(sf::RenderWindow& window, sf::Font& ft, bool load)
	: window(window) {
	font = ft;
	view.setSize({ 1920, 1080 });
	view.zoom(2.0f); // Initial zoom level
	uiView.setSize({ 1920, 1080 });
	srand((unsigned int)time(NULL));

	if (!tx_farm.loadFromFile("Textury/farm_p3.png") ||
		!tx_tav.loadFromFile("Textury/tavern_p3.png") ||
		!tx_ale.loadFromFile("Textury/farm_b.png") ||
		!tx_road.loadFromFile("Textury/road.png")||
		!tx_cross.loadFromFile("Textury/cross.png")) {
		window.close();
	}
	if (!bg_texture.loadFromFile("Textury/grass3.png"))
	{
		std::cerr << "Error loading background texture" << std::endl;
		window.close();
	}
	background.setSize({ 1920, 1080 });
	background.setPosition(window.mapPixelToCoords({ 0, 0 }, uiView));
	background.setOrigin({ 0, 0 });
	background.setTexture(&bg_texture);
	ui.push_back(&background);
	//sf::View v;

	button.setSize({ 50, 50 });
	button.setFillColor(sf::Color::Red);
	button.setPosition(window.mapPixelToCoords({ 0, 0 }, uiView));
	ui.push_back(&button);

	button2.setSize({ 50, 50 });
	button2.setFillColor(sf::Color::Blue);
	button2.setPosition(window.mapPixelToCoords({ 50, 0 }, uiView));
	ui.push_back(&button2);

	button3.setSize({ 50, 50 });
	button3.setFillColor(sf::Color::Magenta);
	button3.setPosition(window.mapPixelToCoords({ 100, 0 }, uiView));
	ui.push_back(&button3);

	button4.setSize({ 50, 50 });
	button4.setFillColor(sf::Color::Black);
	button4.setPosition(window.mapPixelToCoords({ 150, 0 }, uiView));
	ui.push_back(&button4);

	button5.setSize({ 50, 50 });
	button5.setFillColor(sf::Color::Green);
	button5.setPosition(window.mapPixelToCoords({ 200, 0 }, uiView));
	ui.push_back(&button5);

	std::cout << ui.size() << std::endl;
	std::cout << ui.size() << std::endl;
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


void Game::render() {
	UpdateAll();
	draw(ui, all, window, view, uiView);
	sf::sleep(sf::milliseconds(100)); 
	
	/*for (int i = 0; i < convex.getPointCount(); i++)
	{
		std::cout<<i<<" ("<<convex.getPoint(i).x << ","<<convex.getPoint(i).y<<") ";
	}
	std::cout << std::endl;*/
}

void Game::handleMouseInput() {

	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
		ButtonHandler();
		if (change) {
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
				sf::sleep(sf::milliseconds(100));
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
				UpdateAll();


				draw(ui, all, window, view, uiView, &linia);
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
				//   std::cout << "Capacity: " << linia.getCapacity() << std::endl;

				sf::sleep(sf::milliseconds(100));
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
		else if (!change)
		{
			bool canPlace = false;
			Node farm(window, placeType);
			switch (placeMode)
			{
			case 0:
				farm.setTexture(&tx_farm);
				break;
			case 1:
				farm.setTexture(&tx_tav);
				break;
			case 2:
				farm.setTexture(&tx_ale);
				break;
			default:
				break;
			}
			while (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
			{
				farm.setPosition(MousePosView(window, view));
				draw(ui, all, window, view, uiView, &farm);
				if (checkIntersection(linie, farm, farms))
				{
					farm.setFillColor(sf::Color(255, 0, 0, 160));
					canPlace = false;
				}
				else
				{
					canPlace = true;
					farm.setFillColor(sf::Color(255, 255, 255, 255));
					if (farm.type == NodeType::Farm)
					{
						for (int i = 0; i < convexes.size(); i++)
						{
							if (convexes[i].Contains(farm.getPosition()))
							{
								std::cout << "Farm placed inside convex hull." << std::endl;
								farm.setFillColor(sf::Color::Yellow);
								break;
							}
						}
					}
			
				}
			}
			if (canPlace)
				farms.push_back(farm);
		}
		sf::sleep(sf::milliseconds(100));

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
		sf::Vector2f cr = MousePosView(window, view);
		sf::sleep(sf::milliseconds(10));
		sf::Vector2f mv = MousePosView(window, view) - cr;
		for (int i = 0; i < all.size(); i++)
		{
			all[i]->move(sf::Vector2f(mv));
		}
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
	}
}

void Game::ButtonHandler()
{
	if (button.getGlobalBounds().contains(MousePosView(window, uiView)))
	{
		change = !change;
		std::cout << "Change mode: " << std::endl;
		sf::sleep(sf::milliseconds(250));
		return;
	}
	if (button2.getGlobalBounds().contains(MousePosView(window, uiView)))
	{
		placeMode = (placeMode + 1) % 3;
		switch (placeMode)
		{
		case 0:
		{
			std::cout << "Placing farms" << std::endl;
			placeType = NodeType::Farm;
			break;
		}
		case 1:
		{
			std::cout << "Placing taverns" << std::endl;
			placeType = NodeType::Tavern;
			break;
		}
		case 2:
		{
			std::cout << "Placing alehouses" << std::endl;
			placeType = NodeType::Alehouse;
			break;
		}
		default:
			break;
		}
		sf::sleep(sf::milliseconds(250));

		return;
	}
	if (button3.getGlobalBounds().contains(MousePosView(window, uiView)))
	{
		TurnEnd();
		sf::sleep(sf::milliseconds(250));
		return;
	}
	if (button4.getGlobalBounds().contains(MousePosView(window, uiView)))
	{
		convexes.push_back(Convex(all));
		sf::sleep(sf::milliseconds(250));
		return;
	}
	if (button5.getGlobalBounds().contains(MousePosView(window, uiView)))
	{
		SaveGame();
		sf::sleep(sf::milliseconds(250));
		return;
	}
}
void Game::TurnEnd()
{
	turn++;
	FlowAlgorithm flow(farms.size());
	flow.MakeGraph(linie, farms);
	flow.printGraph();
	std::pair<size_t,size_t> delivery = flow.Calculate();
	for (size_t i = 0; i < linie.size(); i++)
	{
		std::pair<size_t, size_t> flowLine = flow.checkLine(linie[i].startNode, linie[i].endNode);
		std::pair<size_t, size_t> flowLine2 = flow.checkLine(linie[i].endNode, linie[i].startNode);
		flowLine.first += flowLine2.first; // Dodajemy przeplyw z drugiej warstwy
		flowLine.second += flowLine2.second; // Dodajemy koszt z drugiej warstwy
		linie[i].setUsed(flowLine);
	}
	size_t n = farms.size();
	std::cout << "Farms: " << n << std::endl;
	size_t max_wheat = 0, max_ale = 0, max_tavern = 0;
	for (size_t i = 0; i < n; i++)
	{

		Node& node = farms[i];
		if (node.getType() == NodeType::Farm)
		{
			node.setUsed(flow.checkBuilding(2*n, i));
			max_wheat += node.getCapacity();
		}
		else if (node.getType() == NodeType::Tavern)
		{
			node.setUsed(flow.checkBuilding(i+n , 2*n + 1));
			max_tavern += node.getCapacity();
		}
		else if (node.getType() == NodeType::Alehouse)
		{
			node.setUsed(flow.checkBuilding(i, i+n));
			max_ale += node.getCapacity();
		}
	}
	stats += "\nTurn: " + std::to_string(turn) + " Max Wheat: " + std::to_string(max_wheat) +
		"\nTurn: " + std::to_string(turn) + " Max Ale: " + std::to_string(max_ale)+
		"\nTurn: " + std::to_string(turn) + " Max Tavern: " + std::to_string(max_tavern) +
		"\nTurn: " + std::to_string(turn) + " Total Delivery: " + std::to_string(delivery.first) +
		"\nTurn: " + std::to_string(turn) + " Total Cost: " + std::to_string(delivery.second);
	printUsed();
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
			"\n\tto: "<<used.first << "/" << linie[i].getCapacity() <<
			"\n\tfrom " << used.second << "/" << linie[i].getCapacity() << std::endl;
	}
}