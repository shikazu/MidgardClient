#ifndef _TEXTBOX_H
#define _TEXTBOX_H
#include "Widget.h"
#include "Manager.h"

namespace UI
{

	class TextBox : public Widget
	{
		public:
			//Constructors and Destructor
			TextBox(uint32_t dwIdent, uint8_t uStyle = REGULAR, sf::Vector2i vPos = sf::Vector2i(0,0), sf::Vector2u vSize = sf::Vector2u(50,20));
			TextBox(uint32_t dwIdent, uint8_t uStyle = REGULAR, int32_t x = 0, int32_t y = 0, uint32_t w = 50, uint32_t h = 20);
			~TextBox() {}

			//Methods - Setters
			void SetFontID(uint32_t dwFontID);
			void SetPassChar(char c);
			void SetText(sf::String t);
			void SetCharSize(uint32_t dwSize);
			void SetStyle(uint8_t uStyle);

			//Methods - Getters
			const uint32_t GetFontID() const;
			const sf::String GetText() const;
			const uint32_t GetCharSize() const;
			const uint8_t GetStyle() const;

		protected:
			//Overrides
			virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
			virtual void MousePressed(sf::Event::MouseButtonEvent btnEvent, Manager* pManager);
			virtual void KeyPressed(sf::Event::KeyEvent keyEvent, Manager* pManager);
			virtual void TextEntered(sf::Event::TextEvent textEvent, Manager* pManager);
			virtual void WinResized();
			virtual void Dragged();

		private:
			//Attributes
			sf::String sText;
			uint8_t uStyle;
			char cPass;
			uint32_t dwFontID, dwStart;//dwStart Represents the Starting Character Index - default is 0
			uint32_t dwCursorC, dwCursorX;//C is wrt character and X is wrt pixels
			uint32_t dwCharSize;
			mutable bool bUpdateNeeded;

			//Methods
			void updateCursor(int32_t xSnap = -1);
	};
}
#endif//_TEXTBOX_H
