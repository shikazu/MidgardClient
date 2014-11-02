#ifndef _UIWIDGET_H
#define _UIWIDGET_H
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

        void BorderWidth(float bd);
        float BorderWidth();

        UIWidget* Parent();//No setter you can't set parent afterwards thats absurd

        virtual void Paint(sf::RenderWindow &window);

        virtual void MousePressed(sf::Event::MouseButtonEvent btnEvent);
        virtual void MouseReleased(sf::Event::MouseButtonEvent btnEvent);
        virtual void MouseClicked(sf::Event::MouseButtonEvent btnEvent);

        virtual void MouseEntered(sf::Event::MouseMoveEvent movEvent);
        virtual void MouseLeft(sf::Event::MouseMoveEvent movEvent);
        virtual void MouseMoved(sf::Event::MouseMoveEvent movEvent);

        virtual void KeyPressed(sf::Event::KeyEvent keyEvent);
        virtual void KeyReleased(sf::Event::KeyEvent keyEvent);
        virtual void TextEntered(sf::Event::TextEvent textEvent);

        void SetFocus();
        void KillFocus();
        bool IsClickable();
        bool IsPointInside(uint32_t x, uint32_t y);

    protected:
        sf::Vector2u vPos;
        sf::Vector2u vSize;
        sf::Color pColors[3];
        float fBorder;
        UIWidget* pParent;
        bool bHasFocus;
        bool bClickable;
};

#endif//_UIWIDGET_H
