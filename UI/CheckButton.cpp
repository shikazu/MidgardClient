#include "CheckButton.h"

namespace UI
{
    CheckButton::CheckButton(sf::Vector2f vPos):Button(vPos)
    {
    }

    CheckButton::CheckButton(float x, float y):Button(x, y)
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
}
