#include "Game.hpp"
#include "Functions.hpp"
#include "Algorithms.hpp"
#include <iostream>
#include <ctime>
#include <cstdlib>

Game::Game(sf::RenderWindow& window)
    : window(window){
	view.setSize({ 1280, 720 });
    uiView.setSize({ 1920, 1080 });
    srand((unsigned int)time(NULL));

    if (!tx_farm.loadFromFile("Textury/farm_p3.png") ||
        !tx_tav.loadFromFile("Textury/tavern_p3.png") ||
        !(tx_road = std::make_shared<sf::Texture>())->loadFromFile("Textury/road.png")||
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
    view.setSize({ 1280, 720 });

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
    std::cout << ui.size() << std::endl;
	std::cout << ui.size() << std::endl;
	
	Node tmpS = Node({-250,-250});
    tmpS.setFillColor(sf::Color::Transparent);
    Node tmpT = Node({ -250,-250 });
    tmpS.setFillColor(sf::Color::Transparent);
	farms.push_back(tmpS);
	farms.push_back(tmpT);

}

Game::~Game()
{
	all.clear();
	ui.clear();
	farms.clear();
	linie.clear();
	tx_road.reset();
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
    handleMouseInput();
    handleKeyboardInput();
}

void Game::render() {
    all.clear();

    for (int i = 0; i < linie.size(); i++)
        all.push_back(&linie[i]);
	for (int i = 0; i < farms.size(); i++)
		all.push_back(&farms[i]);

    
    draw(ui, all, window, view, uiView);
}

void Game::handleMouseInput() {
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        if (button.getGlobalBounds().contains(MousePosView(window, uiView)))
        {
            change = !change;
            std::cout << "Change mode: " << std::endl;
            sf::sleep(sf::milliseconds(250));
            return;
        }
        if (button2.getGlobalBounds().contains(MousePosView(window, uiView)))
        {
            isFarm = !isFarm;
            if (isFarm)
            {
                std::cout << "Building farms" << std::endl;
            }
            else
            {
                std::cout << "Building taverns" << std::endl;
            }
            sf::sleep(sf::milliseconds(250));

            return;
        }
        if (button3.getGlobalBounds().contains(MousePosView(window, uiView)))
        {

			// vec[i][j] = capacity ?? z node i do node j przechodzi capacity
            // farm[0] = source S
			// farm[1] = target T
			// jesli farm[i] jest farma to macierzy vec[0][i] = capacity bo S (0) przesyla po niewidzialnej linii do noda i
			// jesli farm[i] jest tawerna to macierzy vec[i][1] = capacity bo i przesyla po niewidzialnej linii do T (1)
			// jesli farm[i] nie jest zadna z tych to po prostu jest i jest zalezna od linii ktora z niej wychodza

			std::vector<std::vector<size_t>> adj = adjMatrixCap(linie, farms);
            std::vector<std::vector<size_t>> lfspn = adjMatrixLifeSpan(linie, farms.size());
            //te nizej w razie potrzeby 
			//std::vector<std::vector<size_t>> adj2 = adjMatrixCost(linie, farms); // macierz sasiedzztwa kosztow
			//std::vector<std::vector<std::pair<size_t, size_t>>> adj3 = adjMatrixBoth(linie, farms); // macierz sasiedzztwa pojemnosci i kosztow
			
			printAdjMatrix(adj);
            std::cout << std::endl;
            printAdjMatrix(lfspn);
            std::cout << std::endl;

			std::cout << "Max flow:\n" << edmondsKarp(adj) << std::endl;

            sf::sleep(sf::milliseconds(250));

            return;
        }
        if (change) {
            int tmp;
            sf::Vector2f cp;
            bool L2L = false;
            Line linia;
            if ((tmp = HoverOverFarm(window, view, farms)) != -1)
            {
                linia = Line({ float(farms[tmp].getPosition().x), float(farms[tmp].getPosition().y) }, tx_road);
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
                linia = Line(crossroad.getPosition(), tx_road);
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
                all.clear();
                for (int i = 0; i < linie.size(); i++)
                {
                    all.push_back(&linie[i]);
                }
                for (int i = 0; i < farms.size(); i++)
                {
                    all.push_back(&farms[i]);
                }

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
            Node farm(window);
            if (isFarm)
            {
                farm.setTexture(&tx_farm);
                farm.isFarm = true;
                farm.isTavern = false;
            }
            else
            {
                farm.isTavern = true;
                farm.isFarm = false;
                farm.setTexture(&tx_tav);
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
				}
            }
            if(canPlace)
                farms.push_back(farm);
        }
        sf::sleep(sf::milliseconds(150));

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
					std::cout << "Line " << i << " (" << linie[i].startNode << "," << linie[i].endNode << "): " << linie[i].getCapacity() << ", " << linie[i].GetLifeSpan() <<": ";
                    std::cin >> tmpC >> tmpK;
                    linie[i].setCapacity(tmpC);
                    linie[i].SetLifeSpan(tmpK);
                }
            }
        }
        for (int i = 0; i < linie.size(); i++)
        {
            std::cout << "Line " << i << " (" << linie[i].startNode << "," << linie[i].endNode << "): " << linie[i].getCapacity() << ", " << linie[i].GetLifeSpan() << std::endl;
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

