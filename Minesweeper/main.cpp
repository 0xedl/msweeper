
#include "TextureManager.h"
#include "Board.h"
using namespace std;

int main()
{
    Board b;
    sf::RenderWindow window(sf::VideoMode(b.GetWidth(), b.GetHeight()+100), "Minesweeper");
    while (window.isOpen())
    {
        sf::Event event{};
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed)
            {
                bool leftClick = (event.mouseButton.button == sf::Mouse::Left);
                b.MousePress(event.mouseButton.x, event.mouseButton.y, leftClick);
            }
        }
        window.clear();
        b.Draw(window);
        window.display();
    }
}
