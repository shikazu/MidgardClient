#include "UIButton.h"
#include <SFML/Graphics/Rect.hpp>
#include <iostream>

UIButton::UIButton(uint32_t x, uint32_t y, UIWidget *parent):UIWidget(x, y, 0, 0, parent)
{
}

UIButton::~UIButton()
{
}

void UIButton::SetTexture(const char* sFile, uint8_t state)
{
    if (state >= INVALID) {return;}//invalid state
    std::string fname(sFile);
    sf::Image image;
    image.loadFromFile(fname);
    image.createMaskFromColor(sf::Color::Magenta);
    if (vSize.x == 0 || vSize.y == 0)
    {
        vSize = image.getSize();
    }
    sf::IntRect rect(0, 0, vSize.x, vSize.y);
    pTextures[state].loadFromImage(image, rect);
}

void UIButton::SetTexture(std::istream &stream, uint8_t state)
{
    if (state >= INVALID) {return;}//invalid state
    stream.seekg(0, stream.end);
    uint32_t dwLength = stream.tellg();
    stream.seekg(0, stream.beg);

    char* sBuffer = new char[dwLength];
    stream.read(sBuffer, dwLength);
    sf::IntRect rect(0, 0, vSize.x, vSize.y);
    pTextures[state].loadFromMemory(sBuffer, dwLength, rect);
    delete sBuffer;
}

void UIButton::Paint(sf::RenderWindow &window)
{
    sf::RectangleShape shape(sf::Vector2f(vSize.x, vSize.y));
    shape.setFillColor(pColors[BackColor]);

    sf::Sprite sprite;
    sprite.setTexture(pTextures[uCurState]);
    sprite.setPosition(vPos.x, vPos.y);

    window.draw(shape);
    window.draw(sprite);
}

void UIButton::SetCallback(UIButton::CallBack pFunc)
{
    pCbkFunc = pFunc;
}

void UIButton::MouseClicked(sf::Mouse::Button button)
{
    if (button == sf::Mouse::Left)
    {
        if (pCbkFunc != nullptr)
        {
            pCbkFunc(pParent, this);
        }
    }
}

void UIButton::MouseEntered()
{
    if (bMousePressed) return;
    uCurState = ACTIVE;
}

void UIButton::MouseLeft()
{
    if (bMousePressed) return;
    uCurState = INACTIVE;
}

void UIButton::MousePressed(sf::Mouse::Button button)
{
    uCurState = PRESSED;
}

void UIButton::MouseReleased(sf::Mouse::Button button)
{
    if (button != caughtButton) return;
    if (bCursorInside)
    {
        uCurState = ACTIVE;
    }
    else
    {
        uCurState = INACTIVE;
    }
}
