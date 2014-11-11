#ifndef _LOGINVIEW_H
#define _LOGINVIEW_H

#include "../UI/Manager.h"
#include "../common/ContentPipeline.h"

class LoginView : public sf::View
{
public:
    LoginView(UI::Manager& mgr, ContentPipeline *pipeline, sf::Font &font, sf::String sUser = "");
    ~LoginView();

private:
    //UI::Manager* pManager;
};

#endif//_LOGINVIEW_H

