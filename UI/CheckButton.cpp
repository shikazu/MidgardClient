#include "CheckButton.h"

namespace UI
{
	CheckButton::CheckButton(uint32_t dwIdent, sf::Vector2i vPos):Button(dwIdent, vPos)
	{
	}

	CheckButton::CheckButton(uint32_t dwIdent, int32_t x, int32_t y):Button(dwIdent, x, y)
	{
	}

	void CheckButton::MouseClicked(sf::Event::MouseButtonEvent btnEvent, Manager* pManager)
	{
		if (btnEvent.button == sf::Mouse::Left)
		{
			if (uCurrent == ACTIVE)
			{
				uCurrent = INACTIVE;
			}
			else
			{
				uCurrent = ACTIVE;
			}
		}
	}

	const bool CheckButton::IsChecked() const
	{
		return (uCurrent == ACTIVE);
	}
	void CheckButton::SetChecked(bool bStatus)
	{
		if (bStatus)
		{
			uCurrent = ACTIVE;
		}
		else
		{
			uCurrent = INACTIVE;
		}
	}

	void CheckButton::MouseReleased(sf::Event::MouseButtonEvent btnEvent, Manager* pManager)
	{
		if (btnEvent.button == sf::Mouse::Left && !IsPointInside(btnEvent.x, btnEvent.y))
		{
			GetMouseCursor().SetState(CRS_DEFAULT);
		}
	}

	void CheckButton::MouseEntered(sf::Event::MouseMoveEvent movEvent, Manager* pManager)
	{
		if (!pManager->IsPressed(NULL)) return; //If its pressing anything else ignore
		GetMouseCursor().SetState(CRS_BUTTON);
	}

	void CheckButton::MouseLeft(sf::Event::MouseMoveEvent movEvent, Manager* pManager)
	{
		if (!pManager->IsPressed(NULL)) return; //If its pressing anything else ignore
		GetMouseCursor().SetState(CRS_DEFAULT);
	}
}
