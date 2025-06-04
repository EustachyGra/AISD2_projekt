#include "Button.hpp"
Button::Button()
{
}
Button::Button(sf::Vector2f pos, sf::Vector2f size, sf::Font& ft, std::string str)
{
	this->setOrigin({ 0,0 });
	this->setSize(size);
	this->setPosition(pos);
	this->setFillColor(sf::Color::White);

	textString = str;
	font = ft;
	text2 = new sf::Text(ft, str, 24);
	text2->setFillColor(sf::Color::White);
	text2->setStyle(sf::Text::Bold);
	text2->setOutlineColor(sf::Color::Black);
	text2->setOutlineThickness(3);
	text2->setOrigin(text2->getGlobalBounds().size * 0.5f);
	if (text2->getGlobalBounds().size.x > size.x)
	{
		this->setSize({ text2->getGlobalBounds().size.x * 1.2f, getSize().y });
	}
	if (text2->getGlobalBounds().size.y > size.y)
	{
		this->setSize({ getSize().x, text2->getGlobalBounds().size.y * 1.4f });
	}
	this->setPosition(pos);
	text2->setPosition(getPosition() + getSize() * 0.5f);

}
Button::Button(sf::Vector2f pos, sf::Vector2f size, sf::Font& ft, std::string str, sf::Texture& tx, sf::Texture& c_tx)
{
	this->setOrigin(size / 2.0f);
	this->setSize(size);
	this->setPosition(pos + getOrigin());
	this->setFillColor(sf::Color::White);
	this->setOutlineColor(sf::Color::Black);
	this->setOutlineThickness(2);
	this->setTexture(&tx);
	textString = str;
	font = ft;
	button_tx = tx;
	cancel_tx = c_tx;

	text2 = new sf::Text(ft, str, 24);
	text2->setFillColor(sf::Color::White);
	text2->setStyle(sf::Text::Bold);
	text2->setOutlineColor(sf::Color::Black);
	text2->setOutlineThickness(3);
	text2->setOrigin({ text2->getGlobalBounds().size.x / 2, text2->getGlobalBounds().size.y / 2 });
	text2->setPosition(getPosition() + sf::Vector2f{ 0,getSize().y });
}


bool Button::isMouseOver(sf::Vector2f pos)
{
	return getGlobalBounds().contains(pos);
}

void Button::ResetTx()
{
	this->setTexture(&button_tx);
	this->setFillColor(sf::Color::White);
	active = false;
}

void Button::changeTx()
{
	if (active)
	{
		this->setTexture(&button_tx);
		this->setFillColor(sf::Color::White);
		active = false;
	}
	else
	{
		this->setTexture(&cancel_tx);
		this->setFillColor(sf::Color::Red);
		active = true;
	}
}

void Button::SetPosAll(sf::Vector2f pos)
{
	this->setPosition(pos);
	if (text2 != nullptr)
	{
		text2->setPosition(getPosition() + getSize() * 0.5f);
		text2->setOrigin(text2->getGlobalBounds().size / 2.0f);
	}
}

void Button::TextUpdate(std::string str, bool resize, bool bigger, int max)
{
	if (text2 != nullptr)
	{
		sf::Vector2f prev_or = text2->getOrigin();
		text2->setString(str);
		text2->setOrigin(text2->getGlobalBounds().size / 2.0f);
		text2->setPosition(text2->getPosition() + text2->getOrigin() - prev_or);
	}
	if (resize && text2->getGlobalBounds().size.x * 1.1 > getSize().x && !bigger)
	{
		while (text2->getGlobalBounds().size.x * 1.1 > getSize().x)
		{
			text2->setCharacterSize(text2->getCharacterSize() - 1);
		}
	}
	else if (resize && text2->getGlobalBounds().size.x * 1.1 < getSize().x && bigger)
	{
		while (text2->getGlobalBounds().size.x * 1.1 < getSize().x)
		{
			text2->setCharacterSize(text2->getCharacterSize() + 1);
			if (text2->getCharacterSize() >= 24)
			{
				text2->setCharacterSize(max);
				break;
			}
		}
	}
}

//sf::Text& Button::getText()
//{
//	return text;
//}
void Button::draw(sf::RenderWindow& w, sf::View& v)
{
	w.setView(v);
	w.draw(*this);
	if (text2 != nullptr)
		w.draw(*text2);
}

TextBox::TextBox(sf::Vector2f pos, sf::Vector2f size, sf::Font& ft, std::string str)
	: TextView(pos, size, ft, str), Input(pos + sf::Vector2f(0, size.y), size, ft, "")
{
	TextView.setFillColor(sf::Color::Yellow);
	TextView.setOutlineColor(sf::Color::Black);
	TextView.setOutlineThickness(3);
	//TextView.setSize({ TextView.getSize().x, TextView.text2->getGlobalBounds().size.y });
	TextView.text2->setOrigin(TextView.text2->getGlobalBounds().size * 0.5f);
	TextView.text2->setPosition(sf::Vector2f(10, 0) + TextView.text2->getGlobalBounds().size / 2.0f);
	Input.setFillColor(sf::Color::Yellow);
	Input.setOutlineColor(sf::Color::Black);
	Input.setOutlineThickness(3);
	renderTexture = sf::RenderTexture({ unsigned(TextView.getSize().x + 4), unsigned(TextView.getSize().y < 500) ? unsigned(TextView.getSize().y) : 500 });
	Input.setSize(sf::Vector2f(renderTexture.getSize().x, 50));
}

void TextBox::draw(sf::RenderWindow& w, sf::View& v, sf::Vector2f pos)
{
	TextView.setOrigin({ 0,0 });
	TextView.setPosition({ 2,2 });
	renderTexture.clear(sf::Color::Yellow);
	renderTexture.draw(TextView);
	renderTexture.draw(*TextView.text2);
	renderTexture.display();
	Input.setOrigin({ Input.getSize().x, 0 });
	Input.setPosition(pos - sf::Vector2f(2, 0));
	Input.text2->setPosition({ Input.getPosition().x - Input.getGlobalBounds().size.x / 2.0f,Input.getPosition().y + Input.getGlobalBounds().size.y / 2.0f });
	Input.draw(w, v);
	sf::Sprite sprite(renderTexture.getTexture());
	sprite.setOrigin(sprite.getGlobalBounds().size);
	sprite.setPosition(pos - sf::Vector2f(2, 3));
	w.draw(sprite);
}

void TextBox::move(float offset)
{

	/*if (TextView.text2->getPosition().y - TextView.text2->getGlobalBounds().size.y / 2 >= 0 &&
		TextView.text2->getPosition().y + TextView.text2->getGlobalBounds().size.y / 2 <= 500)*/
	if (offset < 0 && TextView.text2->getPosition().y + TextView.text2->getGlobalBounds().size.y / 2 + offset < 400)
		return;
	if (offset > 0 && TextView.text2->getPosition().y - TextView.text2->getGlobalBounds().size.y / 2 + offset > 0)
		return;
	TextView.text2->setPosition(TextView.text2->getPosition() + sf::Vector2f(0, offset));
}

UpgradeBox::UpgradeBox(sf::Vector2f pos, sf::Vector2f size, sf::Font& ft, std::string str, std::vector<Node>* nodes, size_t id1, size_t id2)
{
	upgradeButton = Button(pos, size, ft, str);
	upgradeButton.setOutlineColor(sf::Color(0, 166, 5));
	upgradeButton.setOutlineThickness(3);
	node1 = new Button((*nodes)[id1].getPosition() - (*nodes)[id1].getSize() / 2.0f, (*nodes)[id1].getSize(), ft, "1");
	node1->setFillColor(sf::Color::Transparent);
	node1->setOutlineColor(sf::Color::Blue);
	node1->setOutlineThickness(2);
	node2 = new Button((*nodes)[id2].getPosition() - (*nodes)[id2].getSize() / 2.0f, (*nodes)[id2].getSize(), ft, "2");
	node2->setFillColor(sf::Color::Transparent);
	node2->setOutlineColor(sf::Color::Blue);
	node2->setOutlineThickness(2);

}
UpgradeBox::UpgradeBox(sf::Vector2f pos, sf::Vector2f size, sf::Font& ft, std::string str, sf::Vector2f posR, sf::Vector2f sizeR)
{
	upgradeButton = Button(pos, size, ft, str);
	upgradeButton.setOutlineColor(sf::Color(0, 166, 5));
	upgradeButton.setOutlineThickness(3);
	node1 = new Button(posR - sizeR / 2.0f, sizeR, ft, "");
	node1->setFillColor(sf::Color::Transparent);
	node1->setOutlineColor(sf::Color::Blue);
	node1->setOutlineThickness(2);
}

UpgradeBox::~UpgradeBox()
{
	if (node1 != nullptr)
		delete node1;
	if (node2 != nullptr)
		delete node2;
	std::cout << "UpgradeBox destroyed" << std::endl;
}

void UpgradeBox::draw(sf::RenderWindow& w, sf::View& v)
{
	upgradeButton.setFillColor(sf::Color(120, 255, 101));
	upgradeButton.draw(w, v);
	if (node1 != nullptr)
	{
		node1->draw(w, v);
	}
	if (node2 != nullptr)
	{
		node2->draw(w, v);
	}
}
