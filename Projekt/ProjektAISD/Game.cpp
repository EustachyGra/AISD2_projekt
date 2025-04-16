#include "Game.hpp"
#include "Functions.hpp"
#include <iostream>
#include <ctime>
#include <cstdlib>

Game::Game(sf::RenderWindow& window)
    : window(window), view(window.getView()), uiView(window.getDefaultView()) {
    srand(time(NULL));

    if (!icon.loadFromFile("Textury/beer2.png") ||
        !tx_farm.loadFromFile("Textury/farm_t2.png") ||
        !tx_tav.loadFromFile("Textury/tav_t.png") ||
        !(tx_road = std::make_shared<sf::Texture>())->loadFromFile("Textury/road.png")) {
        window.close();
    }

    window.setIcon(icon.getSize(), icon.getPixelsPtr());

    button.setSize({ 50, 50 });
    button.setFillColor(sf::Color::Red);
    button.setPosition({ 0, 0 });
    ui.push_back(&button);

    button2.setSize({ 50, 50 });
    button2.setFillColor(sf::Color::Blue);
    button2.setPosition({ 50, 0 });
    ui.push_back(&button2);

    button3.setSize({ 50, 50 });
    button3.setFillColor(sf::Color::Magenta);
    button3.setPosition({ 100, 0 });
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
	for (int i = 2; i < farms.size(); i++)
		all.push_back(&farms[i]);


    draw(ui, all, window, view, uiView);
}

void Game::handleMouseInput() {
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        sf::sleep(sf::milliseconds(150));

        if (button.getGlobalBounds().contains(MousePosView(window, uiView)))
        {
            change = !change;
            sf::sleep(sf::milliseconds(200));
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
            sf::sleep(sf::milliseconds(200));

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
            //te nizej w razie potrzeby 
			//std::vector<std::vector<size_t>> adj2 = adjMatrixCost(linie, farms); // macierz sasiedzztwa kosztow
			//std::vector<std::vector<std::pair<size_t, size_t>>> adj3 = adjMatrixBoth(linie, farms); // macierz sasiedzztwa pojemnosci i kosztow
			
			printAdjMatrix(adj); // dziala tylko dla adjMatrixCap
            sf::sleep(sf::milliseconds(200));

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
                tmp = HoverOverFarm(window, view, farms, linia.startNode);

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
					farms.push_back(tmp);
					linia.ConnectToNode(farms, farms.size() - 1);
                }
                if (L2L)
                {
                    if (linia.startLine < linia.CollideWith)
                    {
                        linia.CollideWith -= 1;
                    }
                    splitLine(linie[linia.startLine], linie, linia.startNode, linia.startLine, farms);
                }
                if (linia.CollideWith != -1)
                {
                    splitLine(linia.getPosEnd(), linie[linia.CollideWith], linie, linia.CollideWith, farms);
                    linia.endNode = farms.size() - 1;
                }
                // std::cout << "Setting line color to white." << std::endl; // Debugging line

                linia.ConnectToNode(farms);
                linia.setFillColor(sf::Color(255, 255, 255, 255));
                linia.setOutlineColor(sf::Color::Black);
                linia.setOutlineThickness(2);
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
					std::cout << "Line " << i << " (" << linie[i].startNode << "," << linie[i].endNode << "): " << linie[i].getCapacity() << ", " << linie[i].GetCost() <<": ";
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
}
