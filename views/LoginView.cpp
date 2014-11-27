#include "LoginView.h"
#include "../common/FileStream.h"
#include "../UI/CheckButton.h"
#include "../UI/TextBox.h"
#include <iostream>

namespace LoginView
{
	///Variables
	sf::String userName;
	sf::String passWord;
	uint32_t dwFontID, dwCharSize;
	uint8_t uStyle;

	bool bLoggedIn, bExitGame;
	bool bSaveUser;
	UI::Frame* pFrame;

	void Init(sf::String user, sf::String pass, uint32_t dwFont, uint8_t uTextStyle, uint32_t dwSize, bool bSave)
	{
		userName   = user;
		passWord   = pass;
		dwFontID   = dwFont;
		dwCharSize = dwSize;
		bSaveUser  = bSave;
		uStyle	 = uTextStyle;
		bLoggedIn = false;
		bExitGame = false;
	}

	void Create(UI::Manager &mgr)
	{
		if (bLoggedIn && !bSaveUser)
		{
			userName = "";
		}
		//Window Frame
		pFrame = new UI::Frame(LV_FRAME, 0, 0);
		pFrame->SetTexture("login_interface\\win_login.bmp");
		pFrame->SetAlign(UI::CENTER, UI::MIDDLE);
		pFrame->SetDragArea(0, 0, pFrame->GetWidth(), pFrame->GetHeight());
		mgr.AddChild(pFrame);

		//Username Input
		UI::TextBox* pUser = new UI::TextBox(LV_USERIN, uStyle, 90, 29, 127, 18);
		pFrame->AddChild(pUser);
		pUser->SetColor(sf::Color::Blue, UI::FOREGROUND);
		pUser->SetFontID(dwFontID);
		pUser->SetCharSize(dwCharSize);
		pUser->SetCornerRadius(1);
		pUser->SetText(userName);
		mgr.AddFocus(pUser);

		//Password Input
		UI::TextBox* pPass = new UI::TextBox(LV_PASSIN, uStyle, 90, 61, 127, 18);
		pFrame->AddChild(pPass);
		pPass->SetColor(sf::Color::Blue, UI::FOREGROUND);
		pPass->SetFontID(dwFontID);
		pPass->SetCharSize(dwCharSize);
		pPass->SetCornerRadius(1);
		pPass->SetPassChar('*');
		mgr.AddFocus(pPass);

		//Save User CheckButton
		UI::CheckButton* pSave = new UI::CheckButton(LV_SAVECHK, pUser->GetPosition(false).x + pUser->GetWidth() + 15 , 33);
		pSave->SetTexture("login_interface\\chk_saveoff.bmp", pSave->INACTIVE);
		pSave->SetTexture("login_interface\\chk_saveon.bmp",  pSave->ACTIVE);
		pSave->SetChecked(bSaveUser);
		pFrame->AddChild(pSave);

		//OK Button
		UI::Button* pLogin   = new UI::Button(LV_LOGINBTN,   -49, -4);
		pLogin->SetAlign(UI::RIGHT, UI::BOTTOM);
		pLogin->SetTexture("login_interface\\btn_connect.bmp", pLogin->INACTIVE);
		pLogin->SetTexture("login_interface\\btn_connect_a.bmp", pLogin->ACTIVE);
		pLogin->SetTexture("login_interface\\btn_connect_b.bmp", pLogin->PRESSED);
		pLogin->SetCallback(LoginView::HandleLogin);
		pFrame->AddChild(pLogin);

		//Exit Button
		UI::Button* pExit = new UI::Button(LV_EXITBTN,  -4, -4);
		pExit->SetAlign(UI::RIGHT, UI::BOTTOM);
		pExit->SetTexture("login_interface\\btn_exit.bmp", pExit->INACTIVE);
		pExit->SetTexture("login_interface\\btn_exit_a.bmp", pExit->ACTIVE);
		pExit->SetTexture("login_interface\\btn_exit_b.bmp", pExit->PRESSED);
		pExit->SetCallback(LoginView::HandleExit);
		pFrame->AddChild(pExit);

		mgr.SwitchFocus(pUser);
		bLoggedIn = false;
		bExitGame = false;
	}

	void HandleLogin(UI::Widget* pButton, UI::Manager* pManager)
	{
		bLoggedIn = true;
		UI::TextBox* pTextInput;

		pTextInput = (UI::TextBox*)(pFrame->GetChild(LV_USERIN));
		userName = pTextInput->GetText();

		pTextInput = (UI::TextBox*)(pFrame->GetChild(LV_PASSIN));
		passWord = pTextInput->GetText();

		UI::CheckButton* pCheck = (UI::CheckButton*)(pFrame->GetChild(LV_SAVECHK));
		bSaveUser = pCheck->IsChecked();

		pManager->DelChild(pFrame, pManager);
		GetMouseCursor().SetState(CRS_DEFAULT);
	}

	void HandleExit(UI::Widget* pButton, UI::Manager* pManager)
	{
		UI::Button* pExit = (UI::Button*)pButton;
		pExit->SetCurrentState(pExit->INACTIVE);
		char *pMsg = GetMsg(17);
		new UI::MessageBox(pManager, dwFontID, uStyle, dwCharSize, LoginView::HandleExitBox, pMsg, UI::MSG_OK|UI::MSG_CANCEL, pButton->GetParent()->GetX(), pButton->GetParent()->GetY() - 120);//"Do you want to Exit the game?"
		delete[] pMsg;
		GetMouseCursor().SetState(CRS_DEFAULT);
	}

	void HandleExitBox(UI::Widget* pWidget, UI::Manager* pManager)
	{
		UI::MessageBox* pBox = (UI::MessageBox*)pWidget;
		if (pBox->GetSelected() == UI::MSG_OK)
		{
			bExitGame = true;
		}
	}

	sf::String& GetUserName()
	{
		return userName;
	}

	sf::String& GetPassWord()
	{
		return passWord;
	}

	bool IsLoggedIn()
	{
		return bLoggedIn;
	}

	bool IsExitPressed()
	{
		return bExitGame;
	}
}
