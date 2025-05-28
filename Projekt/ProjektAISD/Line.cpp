#include "Line.hpp"
#include "Functions.hpp"
Line::Line() { capacity = 0; cost = 0; endNode = NULL; startNode = NULL; }
Line::Line(sf::Vector2f pos)
{
    capacity = 0;
	endNode = NULL;
	startNode = NULL;
	this->setOrigin({ 0, 10 });
    this->setTexture(texture.get());
    this->setPosition(pos);
    cost = (1 + rand() % 20);
}

Line::Line(sf::RectangleShape x)
{
	capacity = 0;
    cost = 0;
	endNode = NULL;
	startNode = NULL;
    this->setOrigin(x.getOrigin());
	this->setTexture(x.getTexture());
    this->setPosition(x.getPosition());
    this->setFillColor(sf::Color(rand() % 255, rand() % 255, rand()%255));
    this->setSize(x.getSize());
    this->setRotation(x.getRotation());
    this->setOrigin(x.getOrigin());
}

sf::Vector2f Line::getPosEnd() const { return endPos; }
void Line::setPosEnd(sf::Vector2f pos) { endPos = pos; }

void Line::ConnectToNode(std::vector<Node> nodes, size_t tmpEnd, size_t tmpStart)
{
    if (tmpEnd != -1)
        endNode = tmpEnd;
    if (tmpStart != -1)
        startNode = tmpStart;
    this->setFillColor(green);
    CanPlace = true;
	endPos = nodes[endNode].getPosition();
    this->setPosition(nodes[startNode].getPosition());
    this->setRotation(sf::radians(atan2(
        -(nodes[startNode].getPosition().y - nodes[endNode].getPosition().y),
        -(nodes[startNode].getPosition().x - nodes[endNode].getPosition().x)
    )));
    this->setSize({ dl(nodes[startNode].getPosition(), nodes[endNode].getPosition()), 20 });
	//text.setString(std::to_string(capacity) +"," std::to_string(cost));
}

void Line::FollowMouse(sf::Vector2f mousePos)
{
    this->setSize(
        { dl(this->getPosition(), mousePos)
            ,20 });

    this->setRotation(sf::radians(atan2(
        -(this->getPosition().y - mousePos.y),
        -(this->getPosition().x - mousePos.x)
    )));
    this->setPosEnd(mousePos);
}

void Line::Reset()
{
	freePlace = false;
    endNode = -1;
    CollideWith = -1;
    CanPlace = false;
	SetGreen();
}

std::shared_ptr<sf::Texture> Line::GetTxt() { return texture; }

bool Line::SnapToLine(std::vector<Line>& linie, std::vector<Node> farms, sf::Vector2f mousePos)
{
    for (int i = 0; i < linie.size(); i++)
    {
		if (i == startLine) continue;
        if (linie[i].startNode == startNode or linie[i].endNode == startNode) continue;
        if (!linie[i].getGlobalBounds().contains(mousePos)) continue;
        if (CursorOnLine(linie[i], *this, farms))
        {
            setFillColor(green);
            CollideWith = i;
            setPosEnd(closestPointOnLine(farms[linie[i].startNode].getPosition(), farms[linie[i].endNode].getPosition(), mousePos));
            setSize({ dl(getPosition(), getPosEnd()), 20 });
            setRotation(sf::radians(atan2(
                -(getPosition().y - getPosEnd().y),
                -(getPosition().x - getPosEnd().x)
            )));
            CanPlace = true;
            return true;
        }
    }
	return false;
}

void Line::SetGreen() { this->setFillColor(green); }
void Line::SetRed() { this->setFillColor(red); }
size_t Line::getCapacity() { return capacity; }
void Line::setCapacity(size_t cap) 
{ 
    if (cap != 0)
        capacity = cap;
    else
        capacity = 1 + rand() % 50;
}
void Line::setUsed(std::pair<size_t, size_t> flow)
{
    used = flow;
}
std::pair<size_t, size_t> Line::getUsed()
{
	return used;
}
size_t Line::GetCost() { return cost; };
void Line::SetCost(size_t n_cost) { cost = n_cost; }
