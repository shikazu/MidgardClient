#ifndef _CHECKBUTTON_H
#define _CHECKBUTTON_H

#include "Button.h"
#include "../common/FileStream.h"

namespace UI
{
    class CheckButton : public Button
    {
        ///Basically it is the same as Button but with a few changes in mechanism.
        ///We will be using states ACTIVE and INACTIVE to indicate checked and unchecked state
        ///PRESSED will not be used
        public:
            CheckButton(uint32_t dwIdent, sf::Vector2f vPos = sf::Vector2f(0,0));
            CheckButton(uint32_t dwIdent, float x = 0, float y = 0);//Size is set by the textures like in Button

            const bool GetCheckStatus() const;
            void SetCheckStatus(bool bStatus = true);
            virtual ~CheckButton();

        protected:
            virtual void MousePressed(sf::Event::MouseButtonEvent btnEvent, Manager* pManager);
            virtual void MouseReleased(sf::Event::MouseButtonEvent btnEvent, Manager* pManager);
            virtual void MouseClicked(sf::Event::MouseButtonEvent btnEvent, Manager* pManager);
            virtual void MouseEntered(sf::Event::MouseMoveEvent movEvent, Manager* pManager);
            virtual void MouseLeft(sf::Event::MouseMoveEvent movEvent, Manager* pManager);
    };
}

#endif//_CHECKBUTTON_H
