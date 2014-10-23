#include <SFML/Graphics.hpp>
#include <libconfig.h++>

using namespace libconfig;

int main(int argc, char **argv)
{
    sf::RenderWindow window(sf::VideoMode(1280, 720), "SFML works!");

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.display();
    }

    return 0;
}
