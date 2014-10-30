#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

#include <libconfig.h>

#include <thread>
#include <iostream>
#include <sstream>

#include "views/LoginView.h"

#include "formats/CGrf.h"
#include "formats/CAltitude.h"
#include "formats/CGround.h"
#include "formats/CActor.h"
#include "formats/CSprite.h"
#include "ContentPipeline.h"

int main(int argc, char **argv)
{
    CSprite *spr;
    ContentPipeline *pipeline = new ContentPipeline("data.ini");


    std::stringstream os;
    if(pipeline->getFileStream("data\\prontera.gat", os))
    {
        //std::cout << os.str() << std::endl;
        CAltitude *alt = new CAltitude(os);
        std::cout << "Width: " << alt->GetWidth() << std::endl;
        std::cout << "Height: " << alt->GetHeight() << std::endl;
    }
    os.clear();

    if(pipeline->getFileStream("data\\prontera.gnd", os))
    {
        //std::cout << os.str() << std::endl;
        CGround *gnd = new CGround(os);
        std::cout << "Width: " << gnd->GetWidth() << std::endl;
        std::cout << "Height: " << gnd->GetHeight() << std::endl;
    }
    os.clear();

    if(pipeline->getFileStream("data\\sprite\\몬스터\\toucan.act", os))
    {
        //std::cout << os.str() << std::endl;
        CActor *act = new CActor(os);

        std::cout << "Action Count: " << act->GetActionCount() << std::endl;
    }
    os.clear();

    if(pipeline->getFileStream("data\\sprite\\몬스터\\toucan.spr", os))
    {
        spr = new CSprite(os);
        std::cout << "Tex Count: " << spr->GetTextureCount() << std::endl;
    }
    os.clear();

    if (spr == nullptr ) exit(0);

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
        sprite.setTexture(*spr->GetTexture((counter / 3) %spr->GetTextureCount()), true);
        sprite.scale(2.0f,2.0f);
        sprite.setPosition(50,50);
        window.draw(sprite);
        window.display();
        counter++;
    }

    return 0;
}
