#ifndef _UIBUTTON_H
#define _UIBUTTON_H
#include "UIWidget.h"
#include "../common/FileStream.h"

class UIButton : public UIWidget
{
    public:
        typedef void (*CallBack)(UIWidget *pParent, UIButton *pButton);//pButton will be this Button itself
        enum
        {
            INACTIVE = 0,
            ACTIVE,
            PRESSED,
            INVALID
        };

        UIButton(uint32_t x=0, uint32_t y=0, UIWidget* parent = NULL);
        virtual ~UIButton();
        void SetTexture(FileStream &strm, uint8_t uState);
        void SetCallback(UIButton::CallBack pFunc);
        void Paint(sf::RenderWindow &window);

    private:
        uint8_t uCurState;
        sf::Texture pTextures[3];
        void MousePressed(sf::Event::MouseButtonEvent btnEvent);
        void MouseReleased(sf::Event::MouseButtonEvent btnEvent);
        void MouseClicked(sf::Event::MouseButtonEvent btnEvent);
        void MouseEntered(sf::Event::MouseMoveEvent movEvent);
        void MouseLeft(sf::Event::MouseMoveEvent movEvent);
        UIButton::CallBack pCbkFunc;
        sf::Mouse::Button btnPressed;
};

#endif//_UIBUTTON_H
