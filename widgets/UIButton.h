#ifndef _CBUTTON_H
#define _CBUTTON_H
#include "UIWidget.h"
#include <fstream>

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
        void SetTexture(const char* sFile, uint8_t uState);
        void SetTexture(std::istream &stream, uint8_t uState);
        void SetCallback(UIButton::CallBack pFunc);
        void Paint(sf::RenderWindow &window);

    private:
        uint8_t uCurState;
        sf::Texture pTextures[3];
        void MouseClicked(sf::Mouse::Button button);
        void MousePressed(sf::Mouse::Button button);
        void MouseReleased(sf::Mouse::Button button);
        void MouseEntered();
        void MouseLeft();
        UIButton::CallBack pCbkFunc;
};

#endif//_CBUTTON_H
