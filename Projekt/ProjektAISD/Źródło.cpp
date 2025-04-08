#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <algorithm>

// Funkcja obliczajaca odleglosc miedzy dwoma punktami w przestrzeni 2D
float dl(float x, float y, float mx, float my)
{
    return sqrt(pow(x - mx, 2) + pow(y - my, 2));
}

// Klasa reprezentujaca wezel (Node) - moze byc farma lub punktem na mapie
class Node : public sf::RectangleShape
{
public:
    // Konstruktor tworzacy wezel w miejscu kursora myszy
    Node(sf::Window& window)
    {
        this->setPosition({ float(sf::Mouse::getPosition(window).x), float(sf::Mouse::getPosition(window).y) });
        this->setOrigin({ 25,25 });
        this->setSize({ 50,50 });
        this->setFillColor(sf::Color::Yellow);
    }

    // Konstruktor tworzacy wezel w okreslonej pozycji
    Node(sf::Vector2f pos)
    {
        this->setPosition(pos);
        this->setOrigin({ 10,10 });
        this->setSize({ 20,20 });
        this->setFillColor(sf::Color::White);
    }
};

// Funkcja obliczajaca najblizszy punkt na linii wzgledem danego punktu
sf::Vector2f closestPointOnLine(sf::Vector2f a, sf::Vector2f b, sf::Vector2f m)
{
    float t = ((m.x - a.x) * (b.x - a.x) + (m.y - a.y) * (b.y - a.y)) / ((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y));
    t = std::max(0.f, std::min(1.f, t)); // Ograniczenie t do zakresu [0, 1]
    sf::Vector2f d = { a.x + t * (b.x - a.x), a.y + t * (b.y - a.y) };
    return d;
}

// Klasa reprezentujaca linie (Line) - moze byc droga lub polaczeniem miedzy wezlami
class Line : public sf::RectangleShape
{
private:
    std::shared_ptr<sf::Texture> texture; // Tekstura linii
    sf::Vector2f endPos; // Pozycja koncowa linii
public:
    size_t startNode; // Indeks wezla poczatkowego
    size_t endNode;   // Indeks wezla koncowego
    int CollideWith = -1; // Indeks linii, z ktora nastapila kolizja
    bool CanPlace = false; // Czy linia moze zostac umieszczona

    // Operator porownania linii
    bool operator==(const Line& l) const
    {
        return this->getPosition() == l.getPosition() && this->getPosEnd() == l.getPosEnd();
    }

    // Konstruktor tworzacy linie w okreslonej pozycji z tekstura
    Line(sf::Vector2f pos, std::shared_ptr<sf::Texture> txt)
    {
        texture = txt;
        this->setTexture(texture.get());
        this->setPosition(pos);
        this->setFillColor(sf::Color(rand() % 255, rand() % 255, rand() % 255));
    }

    // Konstruktor kopiujacy wlasciwosci z innego obiektu sf::RectangleShape
    Line(sf::RectangleShape x)
    {
        this->setPosition(x.getPosition());
        this->setFillColor(x.getFillColor());
        this->setSize(x.getSize());
        this->setRotation(x.getRotation());
        this->setOrigin(x.getOrigin());
    }

    // Getter i setter dla pozycji koncowej linii
    sf::Vector2f getPosEnd() const { return endPos; }
    void setPosEnd(sf::Vector2f pos) { endPos = pos; }

    // Funkcja laczaca linie z wezlami
    void ConnectToNode(std::vector<Node> nodes, size_t tmpEnd = -1, size_t tmpStart = -1)
    {
        if (tmpEnd != -1)
            endNode = tmpEnd;
        if (tmpStart != -1)
            startNode = tmpStart;
        this->setFillColor(sf::Color::Green);
        CanPlace = true;
        this->setPosition(nodes[startNode].getPosition());
        this->setRotation(sf::radians(atan2(
            -(nodes[startNode].getPosition().y - nodes[endNode].getPosition().y),
            -(nodes[startNode].getPosition().x - nodes[endNode].getPosition().x)
        )));
        this->setSize({ dl(nodes[startNode].getPosition().x, nodes[startNode].getPosition().y, nodes[endNode].getPosition().x, nodes[endNode].getPosition().y), 20 });
    }

    // Funkcja pozwalajaca linii podazac za kursorem myszy
    void FollowMouse(sf::Window& window)
    {
        this->setSize(
            { dl(this->getPosition().x, this->getPosition().y, float(sf::Mouse::getPosition(window).x), float(sf::Mouse::getPosition(window).y))
                ,20 });

        this->setRotation(sf::radians(atan2(
            -(this->getPosition().y - float(sf::Mouse::getPosition(window).y)),
            -(this->getPosition().x - float(sf::Mouse::getPosition(window).x))
        )));
        this->setPosEnd({ float(sf::Mouse::getPosition(window).x), float(sf::Mouse::getPosition(window).y) });
    }

    // Resetowanie wlasciwosci linii
    void Reset()
    {
        endNode = -1;
        CollideWith = -1;
        CanPlace = false;
        this->setFillColor(sf::Color::Red);
    }

    // Getter dla tekstury linii
    std::shared_ptr<sf::Texture> GetTxt() { return texture; }

    // Funkcja dopasowujaca linie do istniejacych linii
    void SnapToLine(std::vector<Line>& linie, std::vector<Node> farms, sf::Window& window);
};

// Funkcja sprawdzajaca kolizje miedzy liniami
bool checkCollisionLine(const Line& line, const Line& newLine, std::vector<Node> nodes)
{
    float dlLineFrontNew = dl(newLine.getPosEnd().x, newLine.getPosEnd().y, line.getPosition().x, line.getPosition().y);
    float dlLineBackNew = dl(newLine.getPosEnd().x, newLine.getPosEnd().y, line.getPosEnd().x, line.getPosEnd().y);
    return (dlLineFrontNew + dlLineBackNew < line.getSize().x + 1);
}

// Implementacja funkcji SnapToLine
void Line::SnapToLine(std::vector<Line>& linie, std::vector<Node> farms, sf::Window& window)
{
    for (int i = 0; i < linie.size(); i++)
    {
        if (linie[i].endNode != -1 && (linie[i].startNode == startNode or linie[i].endNode == startNode)) continue;
        if (!linie[i].getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window)))) continue;
        if (checkCollisionLine(linie[i], *this, farms))
        {
            setFillColor(sf::Color::Green);
            CollideWith = i;
            setPosEnd(closestPointOnLine(linie[i].getPosition(), linie[i].getPosEnd(), sf::Vector2f(sf::Mouse::getPosition(window))));
            setSize({ dl(getPosition().x, getPosition().y, getPosEnd().x, getPosEnd().y), 20 });
            setRotation(sf::radians(atan2(
                -(getPosition().y - getPosEnd().y),
                -(getPosition().x - getPosEnd().x)
            )));
            CanPlace = true;
            break;
        }
    }
}

// Funkcja rysujaca elementy na ekranie
void draw(std::vector<sf::RectangleShape*>& ui, std::vector<sf::RectangleShape*>& all, sf::RenderWindow& w, sf::View view, sf::View uiView, sf::RectangleShape* obj = nullptr)
{
    w.clear(sf::Color::White);
    w.setView(view);
    for (size_t i = 0; i < all.size(); i++)
    {
        w.draw(*all[i]);
    }
    w.setView(uiView);
    for (size_t i = 0; i < ui.size(); i++)
    {
        w.draw(*ui[i]);
    }
    if (obj != nullptr)
        w.draw(*obj);
    w.display();
}

// Funkcja sprawdzajaca, czy kursor myszy znajduje sie nad wezlem
int HoverOverFarm(sf::RenderWindow& w, const std::vector<Node> farms, int j = -1)
{
    for (int i = 0; i < farms.size(); i++) {
        if (i == j) continue;
        if (farms[i].getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(w))))
        {
            return i;
        }
    }
    return -1;
}

// Funkcja glowna programu
int main()
{
    srand(time(NULL));
    bool change = false;
    sf::RenderWindow window(sf::VideoMode().getDesktopMode(), "SFML works!", sf::Style::Close);
    sf::Texture tx_farm;
    auto tx_road = std::make_shared<sf::Texture>();

    // Wczytywanie tekstur
    if (!tx_farm.loadFromFile("Textury/farm.png"))
    {
        std::cerr << "blad wczytywania grafiki farmy!" << std::endl;
        window.close();
    }
    if (!tx_road->loadFromFile("Textury/road.png"))
    {
        std::cerr << "blad wczytywania grafiki drogi!" << std::endl;
        window.close();
    }

    std::vector<Line> linie; // Lista linii
    std::vector<Node> farms; // Lista wezlow (farm)
    std::vector<sf::RectangleShape*> all; // Wszystkie obiekty do rysowania
    std::vector<sf::RectangleShape*> ui;  // Elementy interfejsu uzytkownika

    sf::View view = window.getView();
    sf::View uiView = window.getDefaultView();

    // Przycisk w interfejsie
    sf::RectangleShape button({ 50, 50 });
    button.setFillColor(sf::Color::Red);
    button.setPosition({ 0, 0 });
    ui.push_back(&button);

    // Glowna petla programu
    while (window.isOpen())
    {
        for (int i = 0; i < linie.size(); i++)
        {
            all.push_back(&linie[i]);
        }
        for (int i = 0; i < farms.size(); i++)
        {
            all.push_back(&farms[i]);
        }

        draw(ui, all, window, view, uiView);

        // Obsluga zdarzen
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        // Obsluga klikniecia lewym przyciskiem myszy
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
        {
            sf::sleep(sf::milliseconds(150));

            // Klikniecie w przycisk zmienia tryb
            if (sf::Mouse::getPosition(window).x < 50 && sf::Mouse::getPosition(window).y < 50)
            {
                change = !change;
                sf::sleep(sf::milliseconds(200));
                continue;
            }

            int tmp;
            if (change && (tmp = HoverOverFarm(window, farms)) != -1) {
                // Tworzenie nowej linii
                Line linia({ float(farms[tmp].getPosition().x), float(farms[tmp].getPosition().y) }, tx_road);
                linia.startNode = tmp;

                linia.setOrigin({ linia.getOrigin().x, linia.getOrigin().y + 10 });
                while (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                    sf::sleep(sf::milliseconds(100));
                    linia.Reset();
                    linia.FollowMouse(window);
                    tmp = HoverOverFarm(window, farms, linia.startNode);
                    if (tmp != -1)
                    {
                        linia.ConnectToNode(farms, tmp);
                    }
                    if (tmp == -1)
                    {
                        linia.SnapToLine(linie, farms, window);
                    }
                    draw(ui, all, window, view, uiView, &linia);
                    window.draw(linia);
                }
                if (linia.CanPlace)
                {
                    linia.setTexture(tx_road.get());
                    linia.setFillColor(sf::Color::White);
                    linia.setOutlineColor(sf::Color::Black);
                    linia.setOutlineThickness(2);
                    if (linia.endNode != -1)
                        linia.ConnectToNode(farms);

                    linie.push_back(linia);
                    sf::sleep(sf::milliseconds(100));
                }
            }
            else if (!change)
            {
                // Tworzenie nowej farmy
                Node farm(window);
                while (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
                {
                    farm.setPosition(sf::Vector2f(sf::Mouse::getPosition(window)));
                    draw(ui, all, window, view, uiView, &farm);
                }

                farm.setTexture(&tx_farm);
                farms.push_back(farm);
            }
        }

        // Obsluga klikniecia prawym przyciskiem myszy (przesuwanie widoku)
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
        {
            sf::Vector2i cr = sf::Mouse::getPosition(window);
            sf::sleep(sf::milliseconds(10));
            sf::Vector2i mv = sf::Mouse::getPosition(window) - cr;
            for (int i = 0; i < all.size(); i++)
            {
                all[i]->move(sf::Vector2f(mv));
            }
            sf::sleep(sf::milliseconds(100));
        }

        // Obsluga przyblizania widoku
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Add))
        {
            view.zoom(0.98f); // Przyblizenie
            window.setView(view);
            sf::sleep(sf::milliseconds(50));
        }

        // Obsluga oddalania widoku
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Subtract))
        {
            view.zoom(1.02f); // Oddalenie
            window.setView(view);
            sf::sleep(sf::milliseconds(50));
        }

        all.clear();
    }
}