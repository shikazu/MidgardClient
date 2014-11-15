#ifndef _LOGINVIEW_H
#define _LOGINVIEW_H

#include "../UI/Manager.h"
#include "../UI/TextBox.h"
#include "../UI/Button.h"
#include "../UI/Frame.h"
#include "../UI/MessageBox.h"
#include "../common/Globals.h"

namespace LoginView
{
	///Initialization
	void Init(sf::String user = "", sf::String pass = "", uint32_t dwFont = 0, uint8_t uTextStyle = UI::REGULAR, uint32_t dwSize = 12, bool bSave = false);

	///Create the Login Window
	void Create(UI::Manager &mgr);//Create  the widgets for Login

	///Button Callbacks
	void HandleLogin(UI::Widget* pButton, UI::Manager* pManager);
	void HandleExit(UI::Widget* pButton, UI::Manager* pManager);
	void HandleMsgBox(UI::Widget* pWidget, UI::Manager* pManager);//pWidget is the messagebox

	///Member Access Functions
	sf::String& GetUserName();
	sf::String& GetPassWord();
	bool IsLoggedIn();
	bool IsExitPressed();
};

#endif//_LOGINVIEW_H

