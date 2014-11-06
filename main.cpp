#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

#include <libconfig.h>
#include <thread>
#include <iostream>
#include <sstream>
#include <exception>

#include "views/LoginView.h"

#include "formats/CAltitude.h"
#include "formats/CGround.h"
#include "formats/CActor.h"
#include "formats/CSprite.h"
#include "common/ContentPipeline.h"
#include "common/FileStream.h"
#include "UI/Manager.h"
#include "UI/Button.h"
#include "UI/TextBox.h"

//sf::Font font;

void BtnCallback(UI::Button* pButton, UI::Manager* pManager);

int main(int argc, char **argv)
{
    CSprite *spr;
    ContentPipeline *pipeline = new ContentPipeline("data.ini");
    FileStream flstream;
    if(pipeline->getFileStream("data\\prontera.gat", flstream, true))
    {
        CAltitude *alt = new CAltitude(flstream);
        std::cout << "Alt Width: " << alt->GetWidth() << std::endl;
        std::cout << "Alt Height: " << alt->GetHeight() << std::endl;
    }

    if(pipeline->getFileStream("data\\prontera.gnd", flstream, true))
    {
        CGround *gnd = new CGround(flstream);
        std::cout << "Gnd Width: " << gnd->GetWidth() << std::endl;
        std::cout << "Gnd Height: " << gnd->GetHeight() << std::endl;
    }

    if(pipeline->getFileStream("data\\sprite\\몬스터\\toucan.act", flstream, true))
    {
        CActor *act = new CActor(flstream);
        std::cout << "Action Count: " << act->GetActionCount() << std::endl;
    }

    if(pipeline->getFileStream("data\\sprite\\몬스터\\toucan.spr", flstream, true))
    {
        spr = new CSprite(flstream);
    }

    if (spr == nullptr) exit(0);

    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 4;
    settings.majorVersion = 3;
    settings.minorVersion = 0;

    sf::RenderWindow window(sf::VideoMode(800, 600), "Midgard Client - Made by Shikazu", sf::Style::Default, settings);
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(30);

    UI::Manager manager(window);
    UI::Button *pButton = new UI::Button(100, 200);

    pipeline->getFileStream("data\\texture\\유저인터페이스\\btn_close.bmp", flstream);
    pButton->SetTexture(flstream, pButton->INACTIVE);

    pipeline->getFileStream("data\\texture\\유저인터페이스\\btn_close_a.bmp", flstream);
    pButton->SetTexture(flstream, pButton->ACTIVE);

    pipeline->getFileStream("data\\texture\\유저인터페이스\\btn_close_b.bmp", flstream);
    pButton->SetTexture(flstream, pButton->PRESSED);
    pButton->SetCallback(BtnCallback);

    manager.AddChild(pButton);

    sf::Font font;
    font.loadFromFile("SFTechnodelightNS.TTF");
    UI::TextBox *pBox = new UI::TextBox(200, 200, 500, 30);
    UI::TextBox *pBox2 = new UI::TextBox(200, 250, 500, 30);
    sf::Color color = sf::Color::White;

    pBox->SetText("DDDD");
    pBox->SetFont(font);
    pBox->SetCharSize(14);
    pBox->SetPassChar('o');
    pBox->SetColor(color, UI::FOREGROUND);

    pBox2->SetText("DDDD");
    pBox2->SetFont(font);
    pBox2->SetCharSize(14);
    pBox->SetColor(color, UI::FOREGROUND);

    manager.AddChild(pBox);
    manager.AddFocus(pBox);
    manager.AddChild(pBox2);
    manager.AddFocus(pBox2);

    int counter = 0;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == event.Closed)
            {
                window.close();
            }
            manager.ParseEvent(event);
        }
        window.clear();
        if (spr != nullptr && spr->IsValid())
        {
            sf::Sprite sprite;
            sprite.setTexture(*spr->GetTexture((counter / 3) %spr->GetTextureCount()), true);
            sprite.scale(2.0f,2.0f);
            sprite.setPosition(50,50);
            window.draw(sprite);
            counter++;
        }
        window.draw(manager);
        window.display();
    }
    return 0;
}

void BtnCallback(UI::Button* pButton, UI::Manager* pManager)
{
    std::cout << "Callback received.. Now Ending sequence" << std::endl;
    exit(0);
}
