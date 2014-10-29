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
#include "formats/GMapGnd.h"
#include "formats/GActor.h"
#include "formats/GSprite.h"

int main(int argc, char **argv)
{
    GSprite *spr;

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

        std::cout << std::endl;

        if(grf->fileExists("data\\prontera.gnd"))
        {
            std::stringstream os;
            if(grf->write("data\\prontera.gnd", os))
            {
                //std::cout << os.str() << std::endl;
                GMapGnd *gnd = new GMapGnd(os);
                std::cout << "Width: " << gnd->GetWidth() << std::endl;
                std::cout << "Height: " << gnd->GetHeight() << std::endl;
            }
        }
        else
        {
            std::cout << "File doesnt exist!" << std::endl;
        }

        if(grf->fileExists("data\\sprite\\homun\\vanilmirth.act"))
        {
            std::stringstream os;
            if(grf->write("data\\sprite\\homun\\vanilmirth.act", os))
            {
                //std::cout << os.str() << std::endl;
                GActor *act = new GActor(os);

                /*std::cout << "Width: " << gnd->GetWidth() << std::endl;
                std::cout << "Height: " << gnd->GetHeight() << std::endl;*/
            }
        }
        else
        {
            std::cout << "File doesnt exist!" << std::endl;
        }

        if(grf->fileExists("data\\sprite\\homun\\lif.spr"))
        {
            std::stringstream os;
            if(grf->write("data\\sprite\\homun\\lif.spr", os))
            {
                //std::cout << os.str() << std::endl;
                spr = new GSprite(os);

                /*std::cout << "Width: " << gnd->GetWidth() << std::endl;
                std::cout << "Height: " << gnd->GetHeight() << std::endl;*/
            }
        }
        else
        {
            std::cout << "File doesnt exist!" << std::endl;
        }
    }

    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 4;
    settings.majorVersion = 3;
    settings.minorVersion = 0;

    sf::RenderWindow window(sf::VideoMode(800, 600), "Midgard Client - Made by Shikazu", sf::Style::Default, settings);
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(30);

    // the event/logic/whatever loop
    //LoginView *loginView = new LoginView(&window);


    uint32_t counter = 0;
    std::vector<sf::Texture*> textures;

    for(uint32_t i = 0; i < spr->GetImageCount(); i++)
    {
        sf::Texture *texture = new sf::Texture();
        texture->loadFromImage(*spr->GetImage(i));
        textures.push_back(texture);
    }

    while(window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                // end the program
                window.close();
            }
        }
        window.clear(sf::Color::Black);

        sf::Sprite sprite;
        sprite.setTexture(*textures.at((counter / 3) %spr->GetImageCount()), true);
        sprite.scale(2.0f,2.0f);
        sprite.setPosition(50,50);
        window.draw(sprite);
        window.display();
        counter++;
    }

    return 0;
}
