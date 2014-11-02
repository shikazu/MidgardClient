#include "UIButton.h"
#include <SFML/Graphics/Rect.hpp>

UIButton::UIButton(uint32_t x, uint32_t y, UIWidget *parent):UIWidget(x, y, 0, 0, parent)
{
}

UIButton::~UIButton()
{
}

void UIButton::SetTexture(FileStream &strm, uint8_t state)
{
    if (state >= INVALID) {return;}//invalid state
    sf::Image image;
    image.loadFromStream(strm);
    image.createMaskFromColor(sf::Color::Magenta);
    if (vSize.x == 0 || vSize.y == 0)
    {
        vSize = image.getSize();
    }
    sf::IntRect rect(0, 0, vSize.x, vSize.y);
    pTextures[state].loadFromImage(image, rect);
}

void UIButton::Paint(sf::RenderWindow &window)
{
    sf::Sprite sprite;
    sprite.setTexture(pTextures[uCurState]);
    sprite.setPosition(vPos.x, vPos.y);
    window.draw(sprite);
}

void UIButton::SetCallback(UIButton::CallBack pFunc)
{
    pCbkFunc = pFunc;
}

void UIButton::MouseClicked(sf::Event::MouseButtonEvent btnEvent)
{
    if (btnEvent.button == sf::Mouse::Left && pCbkFunc != nullptr)
    {
        pCbkFunc(pParent, this);
    }
}

void UIButton::MouseEntered(sf::Event::MouseMoveEvent movEvent)
{
    if (uCurState == PRESSED) return;
    uCurState = ACTIVE;
}

void UIButton::MouseLeft(sf::Event::MouseMoveEvent movEvent)
{
    if (uCurState == PRESSED) return;
    uCurState = INACTIVE;
}

void UIButton::MousePressed(sf::Event::MouseButtonEvent btnEvent)
{
    uCurState = PRESSED;
    btnPressed = btnEvent.button;
}

void UIButton::MouseReleased(sf::Event::MouseButtonEvent btnEvent)
{
    if (btnEvent.button != btnPressed) return;
    if (IsPointInside(btnEvent.x, btnEvent.y))
    {
        uCurState = ACTIVE;
    }
    else
    {
        uCurState = INACTIVE;
    }
}
