#include "LoginView.h"
#include "../common/FileStream.h"
#include "../UI/Button.h"
#include "../UI/CheckButton.h"
#include "../UI/Frame.h"
#include "../UI/TextBox.h"

#define LOGIN   100
#define OKBTN   101
#define EXITBTN 102
#define USERIN  103
#define PASSIN  104
#define SAVECHK 105

LoginView::LoginView(UI::Manager &mgr, ContentPipeline *pipeline, sf::Font &font, sf::String sUser):View(mgr.GetBBox())
{
    mgr.ClearWidgets();

    //Texture Base Path
    std::string sPrefix = "data\\texture\\유저인터페이스\\";
    FileStream stream;

    //Window Frame
    pipeline->getFileStream(sPrefix + "login_interface\\win_login.bmp", stream, true);
    UI::Frame* pFrame = new UI::Frame(LOGIN, 0, 0);
    pFrame->SetTexture(stream);
    pFrame->SetAlignment(UI::MIDDLE, UI::CENTER);
    pFrame->SetDragArea(sf::FloatRect(0, 0, pFrame->GetWidth(), pFrame->GetHeight()));
    mgr.AddChild(pFrame);

    //Input Boxes and Buttons
    UI::TextBox* pUser = new UI::TextBox(USERIN,  90, 29, 127, 18);
    UI::TextBox* pPass = new UI::TextBox(PASSIN,  90, 61, 127, 18);
    UI::Button*  pOK   = new UI::Button(OKBTN,   120, 90);
    UI::Button*  pExit = new UI::Button(EXITBTN, 165, 90);
    UI::CheckButton* pSave = new UI::CheckButton(SAVECHK, 220, 29);

    //Add them to the frame
    pFrame->AddChild(pUser);
    pFrame->AddChild(pPass);
    pFrame->AddChild(pSave);
    pFrame->AddChild(pOK);
    pFrame->AddChild(pExit);

    //User Input
    pUser->SetCharSize(14);
    pUser->SetFont(font);
    pUser->SetCornerRadius(1);
    pUser->SetText(sUser);

    //Pass Input
    pPass->SetCharSize(14);
    pPass->SetFont(font);
    pPass->SetCornerRadius(1);
    pPass->SetPassChar('*');

    //Save User Button
    pipeline->getFileStream(sPrefix + "login_interface\\chk_saveoff.bmp", stream);
    pSave->SetTexture(stream, pSave->INACTIVE);
    pipeline->getFileStream(sPrefix + "login_interface\\chk_saveon.bmp", stream);
    pSave->SetTexture(stream, pSave->ACTIVE);

    //OK Button
    pipeline->getFileStream(sPrefix + "btn_ok.bmp", stream);
    pOK->SetTexture(stream, pOK->INACTIVE);
    pipeline->getFileStream(sPrefix + "btn_ok_a.bmp", stream);
    pOK->SetTexture(stream, pOK->ACTIVE);
    pipeline->getFileStream(sPrefix + "btn_ok_b.bmp", stream);
    pOK->SetTexture(stream, pOK->PRESSED);

    //Exit Button
    pipeline->getFileStream(sPrefix + "login_interface\\btn_exit.bmp", stream);
    pExit->SetTexture(stream, pExit->INACTIVE);
    pipeline->getFileStream(sPrefix + "login_interface\\btn_exit_a.bmp", stream);
    pExit->SetTexture(stream, pExit->ACTIVE);
    pipeline->getFileStream(sPrefix + "login_interface\\btn_exit_b.bmp", stream);
    pExit->SetTexture(stream, pExit->PRESSED);
}
/*
bool LoginView::prepare()
{
    sf::Sprite sprite;
    sf::Texture texture;
    if (texture.loadFromFile("data/login.jpg", sf::IntRect(0, 0, this->window->getSize().x, this->window->getSize().y)))
    {

        this->textures.push_back(texture);
        sprite.setTexture(this->textures.at(0));
        this->sprites.push_back(sprite);
        return true;
    }
    return false;
}
*/
