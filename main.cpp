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
#include "UI/CheckButton.h"
#include "UI/TextBox.h"

#include "views/MapView.h"

//sf::Font font;

//void BtnCallback(UI::Button* pButton, UI::Manager* pManager);

int main(int argc, char **argv)
{
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 4;
    settings.majorVersion = 3;
    settings.minorVersion = 0;

    sf::RenderWindow window(sf::VideoMode(800, 600), "Midgard Client - Made by Shikazu", sf::Style::Default, settings);
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(30);

    ContentPipeline pipeline("data.ini");

    MapView mapView(window, pipeline, "prontera");

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == event.Closed)
            {
                window.close();
            }
        }
        window.clear();
        mapView.draw();
        window.display();
    }




    //FileStream flstream;


    /*CSprite *spr;
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


    UI::Manager manager(window);
    UI::Button *pButton = new UI::Button(1, 100, 200);

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
    UI::TextBox *pBox = new UI::TextBox(2, 200, 200, 100, 20);
    UI::TextBox *pBox2 = new UI::TextBox(3, 200, 250, 100, 20);
    sf::Color color = sf::Color::White;

    pBox->SetText("DDDD");
    pBox->SetFont(font);
    pBox->SetCharSize(10);
    pBox->SetPassChar('*');
    pBox->SetColor(color, UI::FOREGROUND);
    pBox->SetEditable(false);
    sf::Color bdcolor = sf::Color::Red;
    pBox2->SetColor(bdcolor, UI::OUTLINE);
    pBox2->SetBorderWidth(2);
    pBox2->SetCornerRadius(6);

    pBox2->SetText("DDDD");
    pBox2->SetFont(font);
    pBox2->SetCharSize(10);
    pBox->SetColor(color, UI::FOREGROUND);

    manager.AddChild(pBox);
    //manager.AddFocus(pBox);
    manager.AddChild(pBox2);
    manager.AddFocus(pBox2);

    UI::CheckButton *pCButton = new UI::CheckButton(4, 100, 400);
    pipeline->getFileStream("data\\texture\\유저인터페이스\\login_interface\\chk_saveon.bmp", flstream);
    pCButton->SetTexture(flstream, pCButton->ACTIVE);
    pipeline->getFileStream("data\\texture\\유저인터페이스\\login_interface\\chk_saveoff.bmp", flstream);
    pCButton->SetTexture(flstream, pCButton->INACTIVE);

    manager.AddChild(pCButton);

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
    }*/

    return 0;
}

/*
void BtnCallback(UI::Button* pButton, UI::Manager* pManager)
{
    std::cout << "Callback received.. Now Ending sequence" << std::endl;
    exit(0);
}
*/
