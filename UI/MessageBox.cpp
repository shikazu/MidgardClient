#include "MessageBox.h"

namespace UI
{
	MessageBox::MessageBox(Manager *pManager, uint32_t dwFontID, uint8_t uStyle, uint32_t dwCharSize, CALLBACK callback, sf::String sText, uint8_t uButtons, sf::Vector2f vPos):Frame(MSGBOX_FRAM, vPos)
	{
		pManager->AddChild(this);
		this->pCallback = callback;
		this->dwFontID = dwFontID;
		this->dwCharSize = dwCharSize;
		this->construct(pManager, uStyle, sText, uButtons);
	}

	MessageBox::MessageBox(Manager *pManager, uint32_t dwFontID, uint8_t uStyle, uint32_t dwCharSize, CALLBACK callback, sf::String sText, uint8_t uButtons, float x, float y):Frame(MSGBOX_FRAM, x, y)
	{
		pManager->AddChild(this);
		this->pCallback = callback;
		this->dwFontID = dwFontID;
		this->dwCharSize = dwCharSize;
		this->construct(pManager, uStyle, sText, uButtons);
	}

	MessageBox::~MessageBox()
	{
		UI::Manager *pManager = (UI::Manager*)(GetParent());
		pManager->SwitchFocus(pFocusPrevious);
	}

	void MessageBox::construct(Manager *pManager, uint8_t uStyle, sf::String sText, uint8_t uButtons)
	{
		//Save Currently Focused Widget - We need to grab focus to the messagebox so this is needed
		pFocusPrevious = pManager->GetFocused();

		//Setup Focus
		SetFocusable(true);
		pManager->AddFocus(this);
		pManager->SwitchFocus(this);

		//Assign Texture and Drag Area
		SetTexture("win_msgbox.bmp");
		SetDragArea(0, 0, GetWidth(), 20);

		//Now for the Buttons
		float x = -4;
		if ((uButtons & MSG_CANCEL) != 0)
		{
			UI::Button *pButton = new UI::Button(MSGBOX_NBTN, x, -4);
			pButton->SetTexture("btn_cancel.bmp", pButton->INACTIVE );
			pButton->SetTexture("btn_cancel_a.bmp", pButton->ACTIVE );
			pButton->SetTexture("btn_cancel_b.bmp", pButton->PRESSED);
			pButton->SetCallback(MessageBox::SelectButton);
			pButton->SetAlign(UI::RIGHT, UI::BOTTOM);
			x -= (4 + pButton->GetWidth());
			AddChild(pButton);
		}

		if ((uButtons & MSG_OK) != 0)
		{
			UI::Button *pButton = new UI::Button(MSGBOX_PBTN, x, -4);
			pButton->SetTexture("btn_ok.bmp", pButton->INACTIVE );
			pButton->SetTexture("btn_ok_a.bmp", pButton->ACTIVE );
			pButton->SetTexture("btn_ok_b.bmp", pButton->PRESSED);
			pButton->SetCallback(MessageBox::SelectButton);
			pButton->SetAlign(UI::RIGHT, UI::BOTTOM);
			AddChild(pButton);
		}

		//Set selected as NONE - to avoid mischecks
		selected = MSG_NONE;

		//Add the Text
		sf::Vector2f vTextPos = sf::Vector2f(20, GetHeight()/2);
		SetColor(sf::Color::Blue, FOREGROUND);
		UpdateTextVA(dwFontID, sText, uStyle, dwCharSize, GetWidth(), 5, 0, vTextPos);
	}

	void MessageBox::draw(sf::RenderTarget &target, sf::RenderStates states) const
	{
		//Draw everything else
		Frame::draw(target, states);

		//Draw Text
		states.texture = &(GetFont(dwFontID).getTexture(dwCharSize));
		states.blendMode = sf::BlendMode::BlendAlpha;
		target.draw(GetTextVA(), states);
	}

	bool MessageBox::ParseEvent(sf::Event event, Manager* pManager)
	{
		if (event.type == event.KeyPressed && event.key.code == sf::Keyboard::Return)//Catch "Carriage Return" presses
		{
			SelectButton((UI::Widget*)GetChild(MSGBOX_PBTN), pManager);
		}
		else
		{
			bool bStatus = Frame::ParseEvent(event, pManager);
			if (!bStatus && event.type == event.MouseMoved && !pManager->IsHovered(NULL))//Extra precaution like in manager
			{
				pManager->GetHovered()->ParseEvent(event, pManager);
				pManager->SetHovered(NULL);
			}
		}
		return true;
	}

	const MsgButton MessageBox::GetSelected() const
	{
		return selected;
	}

	void MessageBox::SelectButton(Widget* pButton, Manager* pManager)
	{
		//Get the Pointer to this MessageBox. since the function is static, there is no "this" pointer
		UI::MessageBox *pBox = (UI::MessageBox*)(pButton->GetParent());

		//Check and update selected button
		if (pButton->GetID() == MSGBOX_PBTN)
		{
			pBox->selected = MSG_OK;
		}
		else
		{
			pBox->selected = MSG_CANCEL;
		}

		//Switch the Focus first
		pManager->SwitchFocus(pBox->pFocusPrevious);

		//Switch out the Hovered Widget
		pManager->SetHovered(NULL);

		//If callback is defined call it
		if (pBox->pCallback != NULL)
		{
			pBox->pCallback(pBox, pManager);
		}

		//Close the MessageBox - delete it essentially
		pManager->DelChild(pBox);
	}
}
