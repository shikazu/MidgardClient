#ifndef _BUTTON_H
#define _BUTTON_H

#include "../common/FileStream.h"
#include "Widget.h"
#include "Manager.h"

namespace UI
{
    class Button : public Widget
    {
        public:
            enum State
            {
                INACTIVE = 0,
                ACTIVE,
                PRESSED,
                INVALID
            };
            typedef void (*Callback)(Button *pButton, Manager *pManager);

            Button(uint32_t dwIdent, sf::Vector2f vPos = sf::Vector2f(0,0));
            Button(uint32_t dwIdent, float x = 0, float y = 0);//Size is set by the textures
            virtual ~Button();

            void SetTexture(FileStream &stream, State uState);
            void SetCallback(Callback pFunc);//only for Left Button or seperately invoked from outside
            void InvokeCallback(Manager *pManager);

        protected:
            virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
            virtual void MousePressed(sf::Event::MouseButtonEvent btnEvent, Manager* pManager);
            virtual void MouseReleased(sf::Event::MouseButtonEvent btnEvent, Manager* pManager);
            virtual void MouseClicked(sf::Event::MouseButtonEvent btnEvent, Manager* pManager);
            virtual void MouseEntered(sf::Event::MouseMoveEvent movEvent, Manager* pManager);
            virtual void MouseLeft(sf::Event::MouseMoveEvent movEvent, Manager* pManager);

            State uCurrent;
            sf::Texture pTextures[INVALID];//because INVALID is last index + 1
            Callback pCallback;

    };
}


#endif // _BUTTON_H
