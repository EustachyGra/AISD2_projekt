#include "Menu.hpp"

Menu::Menu(sf::RenderWindow& window) : window(window)
{
	if (!img.loadFromFile("Textury/menu4.png") || !plank.loadFromFile("Textury/planks2.png"))
	{
		std::cerr << "Error loading image" << std::endl;
		window.close();
	}
	if (!font.openFromFile("Textury/Roboto-Regular.ttf"))
	{
		std::cerr << "Error loading font" << std::endl;
		window.close();
	}

	view.setSize({ 1280, 720 });
	background.setSize(view.getSize());
	background.setPosition(window.mapPixelToCoords({ 0,0 }, view));
	background.setOrigin({ 0,0 });
	background.setTexture(&img);

	Button startButton(sf::Vector2f(window.mapPixelToCoords(sf::Vector2i(window.getSize().x * 0.05, window.getSize().y * 0.6), view)), { 200, 50 }, font, "Start");
	Button loadButton(sf::Vector2f(window.mapPixelToCoords(sf::Vector2i(window.getSize().x * 0.05, window.getSize().y * 0.7), view)), { 200, 50 }, font, "Load");
	Button exitButton(sf::Vector2f(window.mapPixelToCoords(sf::Vector2i(window.getSize().x * 0.05, window.getSize().y * 0.8), view)), { 200, 50 }, font, "Exit");

	startButton.setTexture(&plank);
	exitButton.setTexture(&plank);
	loadButton.setTexture(&plank);

	buttons.push_back(startButton);
	buttons.push_back(loadButton);
	buttons.push_back(exitButton);

	window.clear();
	window.draw(background);
	for (auto& button : buttons)
	{
		button.draw(window, view);
	}
	window.display();
}

void Menu::run()
{
	while (window.isOpen())
	{
		while (std::optional event = window.pollEvent()) {
			if (event->is<sf::Event::Closed>()) {
				window.close();
				window.clear();
			}
		}
		if (!window.hasFocus())
		{
			sf::sleep(sf::milliseconds(250));
			continue;
		}
		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
		{
			if (error != nullptr) {
				error = nullptr;
				sf::sleep(sf::milliseconds(250));
				continue;
			}
			if (buttons[0].isMouseOver(MousePosView(window,view)))
			{
				std::cout << "Start button clicked" << std::endl;
				sf::sleep(sf::milliseconds(500));
				try {
					Game game(window, font, false);
					game.run();
				}
				catch (std::string& e) {
					error = new Button(window.mapPixelToCoords(sf::Vector2i(window.getSize().x / 2, window.getSize().y / 2), view), { 0,0 }, font, e);
					error->setFillColor(sf::Color::Red);
					error->setOutlineColor(sf::Color::Black);
					error->setOutlineThickness(2);
					isError = true;

					// Mo¿esz dodaæ komunikat na ekranie menu, np. "Nie uda³o siê za³adowaæ plików!"
				}
			}
			if (buttons[1].isMouseOver(MousePosView(window, view)))
			{
				std::cout << "Load button clicked" << std::endl;
				sf::sleep(sf::milliseconds(250));
				try {
					Game game(window, font, true);
					game.run();
				}
				catch (std::string& e) {
					error = new Button(window.mapPixelToCoords(sf::Vector2i(window.getSize().x / 2, window.getSize().y / 2), view), { 300,100 }, font, e);
					error->setFillColor(sf::Color::Red);
					error->setOutlineColor(sf::Color::Black);
					error->setOutlineThickness(2);
					isError = true;
					// Mo¿esz dodaæ komunikat na ekranie menu, np. "Nie uda³o siê za³adowaæ plików!"
				}
			}
			if (buttons[2].isMouseOver(MousePosView(window, view)))
			{
				std::cout << "Exit button clicked" << std::endl;
				window.close();
				window.clear();
			}

		}
		window.clear();
		window.draw(background);
		for (auto& button : buttons)
		{
			button.draw(window, view);
		}
		if (error != nullptr)
			error->draw(window, view);
		window.display();
	}
}
