#include "Button.h"

namespace UI
{
    Button::Button(uint32_t dwIdent, sf::Vector2f vPos):Widget(dwIdent, ENABLED|VISIBLE|CLICKABLE, vPos, sf::Vector2f(0,0))
    {
        uCurrent = INACTIVE;
        pCallback = NULL;
    }

    Button::Button(uint32_t dwIdent, float x, float y):Widget(dwIdent, ENABLED|VISIBLE|CLICKABLE, x, y, 0, 0)
    {
        uCurrent = INACTIVE;
        pCallback = NULL;
    }

    Button::~Button()
    {
    }

    void Button::SetTexture(FileStream &stream, State uState)
    {
        if (uState >= INVALID) return;
        sf::Image image;
        image.loadFromStream(stream);
        image.createMaskFromColor(sf::Color::Magenta);
        sf::Vector2u vImageSize = image.getSize();

        if (GetWidth()  < vImageSize.x) { SetWidth( vImageSize.x); }
        if (GetHeight() < vImageSize.y) { SetHeight(vImageSize.y); }
        sf::IntRect rect(0, 0, GetWidth(), GetHeight());
        pTextures[uState].loadFromImage(image, rect);
    }

    void Button::SetCallback(Callback pFunc)
    {
        pCallback = pFunc;
    }

    void Button::InvokeCallback(Manager *pManager)
    {
        if (pCallback != NULL)
        {
            pCallback(this, pManager);
        }
    }

    void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.texture = &pTextures[uCurrent];
        states.blendMode = sf::BlendMode::BlendAlpha;
        DrawBorder(target, states, true);
        DrawBackground(target, states, true);
    }

    void Button::MousePressed(sf::Event::MouseButtonEvent btnEvent, Manager* pManager)
    {
        if (btnEvent.button == sf::Mouse::Left) { uCurrent = PRESSED;}
    }

    void Button::MouseReleased(sf::Event::MouseButtonEvent btnEvent, Manager* pManager)
    {
        if (btnEvent.button != sf::Mouse::Left) { return; }
        if (IsPointInside(btnEvent.x, btnEvent.y))
        {
            uCurrent = ACTIVE;
        }
        else
        {
            uCurrent = INACTIVE;
        }
    }

    void Button::MouseClicked(sf::Event::MouseButtonEvent btnEvent, Manager* pManager)
    {
        if (btnEvent.button == sf::Mouse::Left) { InvokeCallback(pManager); }
    }

    void Button::MouseEntered(sf::Event::MouseMoveEvent movEvent, Manager* pManager)
    {
        if (!pManager->IsPressed(NULL)) return;
        uCurrent = ACTIVE;
    }

    void Button::MouseLeft(sf::Event::MouseMoveEvent movEvent, Manager* pManager)
    {
        if (!pManager->IsPressed(NULL)) return;
        uCurrent = INACTIVE;
    }
}
