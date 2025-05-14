#include "Menu.hpp"

sf::Vector2f Cords(sf::RenderWindow& window, sf::View& view, sf::Vector2f vec)
{
	return sf::Vector2f(window.mapCoordsToPixel(vec, view));
}

Menu::Menu(sf::RenderWindow& window) : window(window)
{
	if(!img.loadFromFile("Textury/menu4.png") || !plank.loadFromFile("Textury/planks2.png"))
	{
		std::cerr << "Error loading image" << std::endl;
		window.close();
	}
	if (!(font = std::make_shared<sf::Font>())->openFromFile("Textury/arial.ttf"))
	{
		std::cerr << "Error loading font" << std::endl;
		window.close();
	}
	
	view.setSize({ 1280, 720 });
	window.setView(view);

	background.setSize({ 1280,720 });
	background.setPosition(window.mapPixelToCoords({ 0,0 }, view));
	background.setOrigin({0,0});
	background.setTexture(&img);

	Button startButton(sf::Vector2f(window.mapCoordsToPixel({ -200, 525}, view)), {200, 50},font, "Start");
	Button exitButton(sf::Vector2f(window.mapCoordsToPixel({ -200, 575 }, view)), { 200, 50 },font, "Exit");
	Button creditsButton(sf::Vector2f(window.mapCoordsToPixel({ -200, 625 }, view)), { 200, 50 },font, "Authors");

	startButton.setTexture(&plank);
	exitButton.setTexture(&plank);
	creditsButton.setTexture(&plank);

	buttons.push_back(startButton);
	buttons.push_back(exitButton);
	buttons.push_back(creditsButton);

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
			if (buttons[0].isMouseOver(window, view))
			{
				std::cout << "Start button clicked" << std::endl;
				sf::sleep(sf::milliseconds(200));
				break;
			}
			if (buttons[1].isMouseOver(window, view))
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
		window.display();
	}
}
