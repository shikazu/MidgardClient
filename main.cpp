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
#include "widgets/UIManager.h"
#include "widgets/UIButton.h"

CSprite *spr;
ContentPipeline *pipeline;
void Displayer(UIManager *pManager, sf::RenderWindow& window);
void BtnCallback(UIWidget* pParent, UIButton* pButton);

int main(int argc, char **argv)
{
    pipeline = new ContentPipeline("data.ini");
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

    UIManager *manager = new UIManager();
    UIButton *pButton = new UIButton(50, 100);
    FileStream flstream2;
    pipeline->getFileStream("data\\texture\\유저인터페이스\\btn_ok.bmp", flstream2);
    pButton->SetTexture(flstream2, pButton->INACTIVE);

    pipeline->getFileStream("data\\texture\\유저인터페이스\\btn_ok_a.bmp", flstream2);
    pButton->SetTexture(flstream2, pButton->ACTIVE);

    pipeline->getFileStream("data\\texture\\유저인터페이스\\btn_ok_b.bmp", flstream2);
    pButton->SetTexture(flstream2, pButton->PRESSED);
    pButton->SetCallback(BtnCallback);
    manager->AddWidget(pButton);
    //the event/logic/whatever loop
    //LoginView *loginView = new LoginView(&window);
    manager->SetManagerAddon(Displayer, manager->BEFORE_DRAW);
    manager->EventLoop(window);
    return 0;
}

void Displayer(UIManager *pManager, sf::RenderWindow& window)
{
    static int counter = 0;
    if (spr != nullptr && spr->IsValid())
    {
        sf::Sprite sprite;
        sprite.setTexture(*spr->GetTexture((counter / 3) %spr->GetTextureCount()), true);
        sprite.scale(2.0f,2.0f);
        sprite.setPosition(50,50);
        window.draw(sprite);
    }
    counter++;
}

void BtnCallback(UIWidget* pParent, UIButton* pButton)
{
    std::cout << "Callback received.. Now Ending sequence" << std::endl;
    exit(0);
}
