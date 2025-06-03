#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include "Node.hpp"
#include "Line.hpp"
#include "Functions.hpp"
class Button : public sf::RectangleShape
{
	sf::Font font;
	sf::Texture button_tx, cancel_tx;
	std::string textString, textInput; //textInput -> tylko dla statystyk
	bool active = false;
public:
	sf::Text* text2 = nullptr;
	Button();
	Button(sf::Vector2f pos, sf::Vector2f size, sf::Font& ft, std::string str);
	Button(sf::Vector2f pos, sf::Vector2f size, sf::Font& ft, std::string str, sf::Texture& tx, sf::Texture& c_tx);
	/*void setText(std::string str);
	void setFont(sf::Font font);
	void setSize(sf::Vector2f size);
	void setPosition(sf::Vector2f pos);
	void setColor(sf::Color color);
	void setOutlineColor(sf::Color color);
	void setOutlineThickness(float thickness);
	sf::RectangleShape getButton();*/
	bool isMouseOver(sf::Vector2f pos);
	void ResetTx();
	void changeTx();
	void SetPosAll(sf::Vector2f pos);
	void TextUpdate(std::string str, bool resize=false, bool bigger=false, int max=24);
	//sf::Text& getText();
	void draw(sf::RenderWindow& w, sf::View& v);

};

struct TextBox {
	Button TextView;
	Button Input;
	sf::RenderTexture renderTexture;
	TextBox(sf::Vector2f pos, sf::Vector2f size, sf::Font& ft, std::string str);
	void draw(sf::RenderWindow& w, sf::View& v, sf::Vector2f pos);
	void move(float offset);
};

struct UpgradeBox
{
	Button upgradeButton;
	Button *node1 = nullptr, *node2=nullptr;
	UpgradeBox(sf::Vector2f pos, sf::Vector2f size, sf::Font& ft, std::string str, std::vector<Node>* nodes, size_t id1, size_t id2);
	UpgradeBox(sf::Vector2f pos, sf::Vector2f size, sf::Font& ft, std::string str, sf::Vector2f posR, sf::Vector2f sizeR);
	~UpgradeBox();
	void draw(sf::RenderWindow& w, sf::View& v);
	//void SetPosAll(sf::Vector2f pos);
};
#endif// BUTTON_HPP