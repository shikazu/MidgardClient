#ifndef _LOGINVIEW_H
#define _LOGINVIEW_H

#include "../UI/Manager.h"
#include "../UI/TextBox.h"
#include "../UI/Button.h"
#include "../common/ContentPipeline.h"

class LoginView : public sf::View
{
public:
    LoginView(UI::Manager& mgr, ContentPipeline *pipeline, sf::Font &font, sf::String sUser = "");
    virtual ~LoginView(){}
    void PrepareWidgets(UI::Manager& mgr, ContentPipeline *pipeline, sf::Font &font, sf::String sUser = "");
    static void HandleLogin(UI::Button* pButton, UI::Manager* pManager);
    static void HandleExit(UI::Button* pButton, UI::Manager* pManager);

    static bool bOkPressed;
    static sf::String sUserName;
    static sf::String sPassWord;
    static bool bUserSave;
};

#endif//_LOGINVIEW_H

