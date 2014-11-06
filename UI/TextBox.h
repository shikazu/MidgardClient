#ifndef _TEXTBOX_H
#define _TEXTBOX_H

#include "Widget.h"
#include "Manager.h"

namespace UI
{

    class TextBox : public Widget
    {
        public:
            enum Style
            {
                NORMAL = 0x0,
                BOLD = 0x1,
                ITALIC = 0x2
            };
            TextBox(sf::Vector2f vPos = sf::Vector2f(0,0), sf::Vector2f vSize = sf::Vector2f(50,20));
            TextBox(float x = 0, float y = 0, float w = 50, float h = 20);
            virtual ~TextBox();

            void SetFont(sf::Font& f);
            void SetPassChar(char c);
            void SetText(sf::String t);
            const sf::String GetText() const;
            void SetCharSize(uint32_t dwSize);
            const uint32_t GetCharSize() const;

        protected:
            virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
            virtual void MousePressed(sf::Event::MouseButtonEvent btnEvent, Manager* pManager);
            virtual void KeyPressed(sf::Event::KeyEvent keyEvent, Manager* pManager);
            virtual void TextEntered(sf::Event::TextEvent textEvent, Manager* pManager);

        private:
            mutable sf::String sText;
            char cPass;
            sf::Font* pFont;
            mutable uint32_t dwCursorC, dwCursorX;//C is wrt character and X is wrt pixels
            uint32_t dwCharSize;
            Style uStyle;

            mutable sf::VertexArray vaChars;
            mutable bool bUpdateNeeded;

            void updateCursorLocation(int32_t xSnap = -1);
            void updateVertices() const;

    };
}
#endif // _TEXTBOX_H
