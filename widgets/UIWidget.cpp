#include "UIWidget.h"
#include <iostream>

UIWidget::UIWidget(uint32_t x, uint32_t y, uint32_t w, uint32_t h, UIWidget *parent)
{
    pParent = parent;
    vPos.x  = x;
    vPos.y  = y;
    vSize.x = w;
    vSize.y = h;
    pColors[ForeColor] = sf::Color::White;
    pColors[BackColor] = sf::Color::Transparent;
    pColors[BorderColor] = sf::Color::Black;
    fBorder = 1.0;
    bMousePressed = false;
    bCursorInside = false;
}

UIWidget::~UIWidget()
{
}

//Getters and Setters
void UIWidget::Position(sf::Vector2u p) {vPos = p;}
void UIWidget::Position(uint32_t x, uint32_t y) {vPos.x = x; vPos.y = y;}
sf::Vector2u UIWidget::Position() {return vPos;}

void UIWidget::Size(sf::Vector2u s) {vSize = s;}
void UIWidget::Size(uint32_t w, uint32_t h) {vSize.x = w; vSize.y = h;}
sf::Vector2u UIWidget::Size() {return vSize;}

void UIWidget::Color(sf::Color c, uint8_t uType)
{
    if (uType >= InvalidColor) {return;}
    pColors[uType] = c;
}
sf::Color UIWidget::Color(uint8_t uType)
{
    if (uType >= InvalidColor) {return sf::Color::Black;}
    return pColors[uType];
}

void UIWidget::BDWidth(float bd) {fBorder = bd;}
float UIWidget::BDWidth() {return fBorder;}

void UIWidget::Paint(sf::RenderWindow &window)
{
    sf::RectangleShape shape(sf::Vector2f(vSize.x, vSize.y));
    shape.setFillColor(pColors[BackColor]);
    shape.setPosition(vPos.x, vPos.y);
    shape.setOutlineColor(pColors[BorderColor]);
    shape.setOutlineThickness(fBorder);
    window.draw(shape);
}

void UIWidget::HandleEvent(sf::Event event)
{
    //Bounding Rectangle
    sf::IntRect rect(vPos.x, vPos.y, vSize.x, vSize.y);

    //Mouse Entry/Exit - both are obtained from Mouse Move event
    if (event.type == event.MouseMoved)
    {
        uint32_t dwMX = event.mouseMove.x;
        uint32_t dwMY = event.mouseMove.y;
        //Entry
        if (!bCursorInside && rect.contains(dwMX, dwMY))
        {
            bCursorInside = true;
            this->MouseEntered();
            return;
        }
        //Exit
        if (bCursorInside && !rect.contains(dwMX, dwMY))
        {
            bCursorInside = false;
            this->MouseLeft();
            return;
        }
    }

    //Mouse Press
    if (event.type == event.MouseButtonPressed)
    {
        if (bCursorInside)
        {
            if (!bMousePressed)//Catch only the first pressed one.
            {
                caughtButton = event.mouseButton.button;
            }
            bMousePressed = true;
            this->MousePressed(event.mouseButton.button);
            return;
        }
    }

    //Mouse Release - also the same as a click
    if (event.type == event.MouseButtonReleased)
    {
        if (bMousePressed && caughtButton == event.mouseButton.button)
        {
            this->MouseReleased(caughtButton);
            if (bCursorInside)//Only consider click if the release was inside the widget
            {
                this->MouseClicked(caughtButton);
            }
            bMousePressed = false;
            return;
        }

        if (bCursorInside)
        {
            this->MouseReleased(event.mouseButton.button);
            return;
        }
    }

    //Text Entered
    if (event.type == event.TextEntered)
    {
        this->TextEntered(event.text);
        return;
    }

    //Key Press
    if (event.type == event.KeyPressed)
    {
        this->KeyPressed(event.key);
        return;
    }

    //Key Release
    if (event.type == event.KeyReleased)
    {
        this->KeyReleased(event.key);
        return;
    }
}

void UIWidget::MousePressed(sf::Mouse::Button button){}
void UIWidget::MouseReleased(sf::Mouse::Button button){}
void UIWidget::MouseClicked(sf::Mouse::Button button){}
void UIWidget::MouseEntered(){}
void UIWidget::MouseLeft(){}

void UIWidget::KeyPressed(sf::Event::KeyEvent keyEvent){}
void UIWidget::KeyReleased(sf::Event::KeyEvent keyEvent){}
void UIWidget::TextEntered(sf::Event::TextEvent textEvent){}
