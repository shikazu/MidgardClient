#include "LoginView.h"
#include "../common/FileStream.h"
#include "../UI/CheckButton.h"
#include "../UI/Frame.h"
#include "../UI/TextBox.h"

#define LOGIN   100
#define OKBTN   101
#define EXITBTN 102
#define USERIN  103
#define PASSIN  104
#define SAVECHK 105

#include <iostream>//Debug

bool LoginView::bOkPressed = false;
sf::String LoginView::sUserName = "";
sf::String LoginView::sPassWord = "";
bool LoginView::bUserSave = false;
LoginView::LoginView(UI::Manager &mgr, ContentPipeline *pipeline, sf::Font &font, sf::String sUser):View(mgr.GetBBox())
{
    PrepareWidgets(mgr, pipeline, font, sUser);
    LoginView::bUserSave = false;
}
void LoginView::PrepareWidgets(UI::Manager &mgr, ContentPipeline *pipeline, sf::Font &font, sf::String sUser)
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
    UI::Button*  pOK   = new UI::Button(OKBTN,   120, 95);
    UI::Button*  pExit = new UI::Button(EXITBTN, 170, 95);
    UI::CheckButton* pSave = new UI::CheckButton(SAVECHK, 220, 33);

    //Add them to the frame
    pFrame->AddChild(pUser);
    pFrame->AddChild(pPass);
    pFrame->AddChild(pSave);
    pFrame->AddChild(pOK);
    pFrame->AddChild(pExit);
    mgr.AddFocus(pUser);
    mgr.AddFocus(pPass);

    //User Input
    pUser->SetFont(font);
    pUser->SetCharSize(14);
    pUser->SetCornerRadius(1);
    pUser->SetText(sUser);
    pUser->SetColor(sf::Color::Transparent, UI::OUTLINE);
    pUser->SetColor(sf::Color::Blue, UI::FOREGROUND);

    //Pass Input
    pPass->SetFont(font);
    pPass->SetCharSize(14);
    pPass->SetCornerRadius(1);
    pPass->SetPassChar('*');
    pPass->SetColor(sf::Color::Transparent, UI::OUTLINE);
    pPass->SetColor(sf::Color::Blue, UI::FOREGROUND);

    //Save User Button
    pipeline->getFileStream(sPrefix + "login_interface\\chk_saveoff.bmp", stream);
    pSave->SetTexture(stream, pSave->INACTIVE);
    pipeline->getFileStream(sPrefix + "login_interface\\chk_saveon.bmp", stream);
    pSave->SetTexture(stream, pSave->ACTIVE);
    pSave->SetCheckStatus(LoginView::bUserSave);

    //OK Button
    pipeline->getFileStream(sPrefix + "btn_ok.bmp", stream);
    pOK->SetTexture(stream, pOK->INACTIVE);
    pipeline->getFileStream(sPrefix + "btn_ok_a.bmp", stream);
    pOK->SetTexture(stream, pOK->ACTIVE);
    pipeline->getFileStream(sPrefix + "btn_ok_b.bmp", stream);
    pOK->SetTexture(stream, pOK->PRESSED);
    pOK->SetCallback(LoginView::HandleLogin);

    //Exit Button
    pipeline->getFileStream(sPrefix + "login_interface\\btn_exit.bmp", stream);
    pExit->SetTexture(stream, pExit->INACTIVE);
    pipeline->getFileStream(sPrefix + "login_interface\\btn_exit_a.bmp", stream);
    pExit->SetTexture(stream, pExit->ACTIVE);
    pipeline->getFileStream(sPrefix + "login_interface\\btn_exit_b.bmp", stream);
    pExit->SetTexture(stream, pExit->PRESSED);
    pExit->SetCallback(LoginView::HandleExit);

    LoginView::bOkPressed = false;
}

void LoginView::HandleLogin(UI::Button* pButton, UI::Manager* pManager)
{
    LoginView::bOkPressed = true;
    const UI::Widget* pParent = pButton->GetParent();

    UI::TextBox* pTextInput;
    pTextInput = (UI::TextBox*)(pParent->GetChild(USERIN));
    LoginView::sUserName = pTextInput->GetText();
    pTextInput = (UI::TextBox*)(pParent->GetChild(PASSIN));
    LoginView::sPassWord = pTextInput->GetText();

    UI::CheckButton* pCheck = (UI::CheckButton*)(pParent->GetChild(SAVECHK));
    LoginView::bUserSave = pCheck->GetCheckStatus();
}

void LoginView::HandleExit(UI::Button* pButton, UI::Manager* pManager)
{
    pManager->ClearWidgets();
    exit(0);
}
