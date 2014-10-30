#ifndef _CWIDGET_H
#define _CWIDGET_H
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>

class UIWidget
{
    public:
        enum
        {
            ForeColor = 0,
            BackColor,
            BorderColor,
            InvalidColor
        };

        UIWidget(uint32_t x=0, uint32_t y=0, uint32_t w=0, uint32_t h=0, UIWidget *parent = NULL);
        virtual ~UIWidget();

        void Position(uint32_t x, uint32_t y);
        void Position(sf::Vector2u p);
        sf::Vector2u Position();

        void Size(uint32_t w, uint32_t h);
        void Size(sf::Vector2u s);
        sf::Vector2u Size();

        void Color(sf::Color c, uint8_t uType);
        sf::Color Color(uint8_t uType);

        void BDWidth(float bd);
        float BDWidth();

        UIWidget* Parent();//No setter you can't set parent afterwards thats absurd

        void HandleEvent(sf::Event event);

    protected:
        virtual void Paint(sf::RenderWindow &window);

        virtual void MousePressed(sf::Mouse::Button button);
        virtual void MouseReleased(sf::Mouse::Button button);
        virtual void MouseClicked(sf::Mouse::Button button);
        virtual void MouseEntered();
        virtual void MouseLeft();

        virtual void KeyPressed(sf::Event::KeyEvent keyEvent);
        virtual void KeyReleased(sf::Event::KeyEvent keyEvent);
        virtual void TextEntered(sf::Event::TextEvent textEvent);

        bool bMousePressed;//Enabled when mouse is pressed on top of widget, disabled when released anywhere.
        bool bCursorInside;//Enabled when cursor moves inside widget area, disabled when moving out.
        sf::Mouse::Button caughtButton;//First Mouse Button pressed.
        sf::Vector2u vPos;
        sf::Vector2u vSize;
        sf::Color pColors[3];
        float fBorder;
        UIWidget* pParent;
};

#endif//_CWIDGET_H
