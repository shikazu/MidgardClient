#include "Button.h"

namespace UI
{
    Button::Button(sf::Vector2f vPos):Widget(ENABLED|VISIBLE|CLICKABLE, vPos)
    {
        uCurrent = INACTIVE;
        pCallback = nullptr;
    }

    Button::Button(float x, float y):Widget(ENABLED|VISIBLE|CLICKABLE, x, y)
    {
        uCurrent = INACTIVE;
        pCallback = nullptr;
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
        if (pCallback != nullptr)
        {
            pCallback(this, pManager);
        }
    }

    void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        const sf::FloatRect& bBox = GetBBox();
        sf::VertexArray boundary(sf::TrianglesStrip, 4);
        boundary[0] = sf::Vertex(sf::Vector2f(bBox.left             , bBox.top              ), sf::Color::White, sf::Vector2f(0, 0));
        boundary[1] = sf::Vertex(sf::Vector2f(bBox.left             , bBox.top + bBox.height), sf::Color::White, sf::Vector2f(0, bBox.height));
        boundary[2] = sf::Vertex(sf::Vector2f(bBox.left + bBox.width, bBox.top              ), sf::Color::White, sf::Vector2f(bBox.width, 0));
        boundary[3] = sf::Vertex(sf::Vector2f(bBox.left + bBox.width, bBox.top + bBox.height), sf::Color::White, sf::Vector2f(bBox.width, bBox.height));
        states.texture = &pTextures[uCurrent];
        states.blendMode = sf::BlendMode::BlendAlpha;
        target.draw(boundary, states);
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
        if (pManager->IsPressed(this)) return;
        uCurrent = ACTIVE;
    }

    void Button::MouseLeft(sf::Event::MouseMoveEvent movEvent, Manager* pManager)
    {
        if (pManager->IsPressed(this)) return;
        uCurrent = INACTIVE;
    }
}
