#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

#include <libconfig.h>

#include <thread>
#include <iostream>
#include <sstream>
//#include "grf/libgrf.h"

#include "views/LoginView.h"

#include "formats/GGrf.h"
#include "formats/GMapAlt.h"

int main(int argc, char **argv)
{
    GGrf *grf = new GGrf();
    if(grf->open("data.grf"))
    {
        if(grf->fileExists("data\\prontera.gat"))
        {
            std::stringstream os;
            if(grf->write("data\\prontera.gat", os))
            {
                //std::cout << os.str() << std::endl;
                GMapAlt *alt = new GMapAlt(os);
                std::cout << "Width: " << alt->GetWidth() << std::endl;
                std::cout << "Height: " << alt->GetHeight() << std::endl;
            }
        }
        else
        {
            std::cout << "File doesnt exist!" << std::endl;
        }
    }

    /*sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 4;
    settings.majorVersion = 3;
    settings.minorVersion = 0;

    sf::RenderWindow window(sf::VideoMode(800, 600), "Midgard Client - Made by Shikazu", sf::Style::Default, settings);
    window.setVerticalSyncEnabled(true);
*/
    // the event/logic/whatever loop
    //LoginView *loginView = new LoginView(&window);


    return 0;
}
