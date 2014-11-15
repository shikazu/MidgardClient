#include "LoginView.h"
#include "../common/FileStream.h"
#include "../UI/CheckButton.h"
#include "../UI/TextBox.h"

#define LOGIN   100
#define OKBTN   101
#define EXITBTN 102
#define USERIN  103
#define PASSIN  104
#define SAVECHK 105

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
	}

	void Create(UI::Manager &mgr)
	{
		mgr.DelChildren();

		//Window Frame
		pFrame = new UI::Frame(LOGIN, 0, 0);
		pFrame->SetTexture("login_interface\\win_login.bmp");
		pFrame->SetAlign(UI::CENTER, UI::MIDDLE);
		pFrame->SetDragArea(0, 0, pFrame->GetWidth(), pFrame->GetHeight());
		mgr.AddChild(pFrame);

		//Username Input
		UI::TextBox* pUser = new UI::TextBox(USERIN, UI::BOLD, 90, 29, 127, 18);
		pUser->SetColor(sf::Color::Blue, UI::FOREGROUND);
		pUser->SetStyle(uStyle);
		pUser->SetFontID(dwFontID);
		pUser->SetCharSize(dwCharSize);
		pUser->SetCornerRadius(1);
		pUser->SetText(userName);
		pFrame->AddChild(pUser);
		mgr.AddFocus(pUser);
		mgr.SwitchFocus(pUser);

		//Password Input
		UI::TextBox* pPass = new UI::TextBox(PASSIN, UI::BOLD, 90, 61, 127, 18);
		pPass->SetColor(sf::Color::Blue, UI::FOREGROUND);
		pPass->SetStyle(uStyle);
		pPass->SetFontID(dwFontID);
		pPass->SetCharSize(dwCharSize);
		pPass->SetCornerRadius(1);
		pPass->SetPassChar('*');
		pFrame->AddChild(pPass);
		mgr.AddFocus(pPass);

		//Save User CheckButton
		UI::CheckButton* pSave = new UI::CheckButton(SAVECHK, pUser->GetPosition(false).x + pUser->GetWidth() + 15 , 33);
		pSave->SetTexture("login_interface\\chk_saveoff.bmp", pSave->INACTIVE);
		pSave->SetTexture("login_interface\\chk_saveon.bmp",  pSave->ACTIVE);
		pSave->SetChecked(bSaveUser);
		pFrame->AddChild(pSave);

		//OK Button
		UI::Button* pOK   = new UI::Button(OKBTN,   -49, -4);
		pOK->SetAlign(UI::RIGHT, UI::BOTTOM);
		pOK->SetTexture("btn_ok.bmp", pOK->INACTIVE);
		pOK->SetTexture("btn_ok_a.bmp", pOK->ACTIVE);
		pOK->SetTexture("btn_ok_b.bmp", pOK->PRESSED);
		pOK->SetCallback(LoginView::HandleLogin);
		pFrame->AddChild(pOK);

		//Exit Button
		UI::Button* pExit = new UI::Button(EXITBTN,  -4, -4);
		pExit->SetAlign(UI::RIGHT, UI::BOTTOM);
		pExit->SetTexture("login_interface\\btn_exit.bmp", pExit->INACTIVE);
		pExit->SetTexture("login_interface\\btn_exit_a.bmp", pExit->ACTIVE);
		pExit->SetTexture("login_interface\\btn_exit_b.bmp", pExit->PRESSED);
		pExit->SetCallback(LoginView::HandleExit);
		pFrame->AddChild(pExit);

		bLoggedIn = false;
		bExitGame = false;
	}

	void HandleLogin(UI::Widget* pButton, UI::Manager* pManager)
	{
		bLoggedIn = true;
		UI::TextBox* pTextInput;

		pTextInput = (UI::TextBox*)(pFrame->GetChild(USERIN));
		userName = pTextInput->GetText();

		pTextInput = (UI::TextBox*)(pFrame->GetChild(PASSIN));
		passWord = pTextInput->GetText();

		UI::CheckButton* pCheck = (UI::CheckButton*)(pFrame->GetChild(SAVECHK));
		bSaveUser = pCheck->IsChecked();

		pManager->DelChild(pFrame);
		pManager->SetHovered(NULL);//To avoid crashes
	}

	void HandleExit(UI::Widget* pButton, UI::Manager* pManager)
	{
		UI::Button* pExit = (UI::Button*)pButton;
		pExit->SetCurrentState(pExit->INACTIVE);
		new UI::MessageBox(pManager, dwFontID, uStyle, dwCharSize, LoginView::HandleMsgBox, "Do you want to Exit the game?", UI::MSG_OK|UI::MSG_CANCEL, pButton->GetParent()->GetX(), pButton->GetParent()->GetY() - 120);
	}

	void HandleMsgBox(UI::Widget* pWidget, UI::Manager* pManager)
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
