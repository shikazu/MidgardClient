#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <libconfig.h++>

#include <thread>
#include <iostream>
#include "grf/libgrf.h"

using namespace libconfig;

int main(int argc, char **argv)
{
    /*grf_handle myGRFHandle = grf_load("data.grf", false);

    std::cout << "GRF File Count: " << grf_filecount(myGRFHandle) << std::endl;

    grf_free(myGRFHandle);*/

    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 4;
    settings.majorVersion = 3;
    settings.minorVersion = 0;

    sf::Window window(sf::VideoMode(800, 600), "Midgard Client - Made by Shikazu", sf::Style::Default, settings);
    window.setVerticalSyncEnabled(true);

    // the event/logic/whatever loop
    while (window.isOpen())
    {
        // handle events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                // end the program
                window.close();
            }

            if (event.type == sf::Event::LostFocus)
            {

            }

            if (event.type == sf::Event::GainedFocus)
            {

            }

            if (event.type == sf::Event::TextEntered)
            {
                if (event.text.unicode < 128)
                    std::cout << "ASCII character typed: " << static_cast<char>(event.text.unicode) << std::endl;
            }
        }

        window.display();
    }

    return 0;
}
