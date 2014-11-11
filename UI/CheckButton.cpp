#include "CheckButton.h"

namespace UI
{
    CheckButton::CheckButton(uint32_t dwIdent, sf::Vector2f vPos):Button(dwIdent, vPos)
    {
    }

    CheckButton::CheckButton(uint32_t dwIdent, float x, float y):Button(dwIdent, x, y)
    {
    }

    CheckButton::~CheckButton()
    {
    }

    void CheckButton::MousePressed(sf::Event::MouseButtonEvent btnEvent, Manager* pManager) {}
    void CheckButton::MouseReleased(sf::Event::MouseButtonEvent btnEvent, Manager* pManager) {}
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
    void CheckButton::MouseEntered(sf::Event::MouseMoveEvent movEvent, Manager* pManager) {}
    void CheckButton::MouseLeft(sf::Event::MouseMoveEvent movEvent, Manager* pManager) {}

    const bool CheckButton::GetCheckStatus() const
    {
        return (uCurrent == ACTIVE);
    }
    void CheckButton::SetCheckStatus(bool bStatus)
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
