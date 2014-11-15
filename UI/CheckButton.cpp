#include "CheckButton.h"

namespace UI
{
	CheckButton::CheckButton(uint32_t dwIdent, sf::Vector2f vPos):Button(dwIdent, vPos)
	{
	}

	CheckButton::CheckButton(uint32_t dwIdent, float x, float y):Button(dwIdent, x, y)
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
}
