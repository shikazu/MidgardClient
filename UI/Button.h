#ifndef _BUTTON_H
#define _BUTTON_H
#include "Widget.h"
#include "Manager.h"

namespace UI
{
	class Button : public Widget
	{
		public:
			// Enumerations
			enum State
			{
				INACTIVE = 0,
				ACTIVE,
				PRESSED,
				INVALID
			};

			//Constructors and Destructor
			Button(uint32_t dwIdent, sf::Vector2f vPos = sf::Vector2f(0,0));
			Button(uint32_t dwIdent, float x = 0, float y = 0);//Size is set by texture
			~Button() {}

			//Other Methods
			void SetCurrentState(State uState);
			void SetTexture(std::string sFile, State uState);
			void SetCallback(UI::CALLBACK pFunc);//Meant for Left Button Clicks only
			void InvokeCallback(Manager *pManager);

		protected:
			//Overrides
			virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
			virtual void MousePressed(sf::Event::MouseButtonEvent btnEvent, Manager* pManager);
			virtual void MouseReleased(sf::Event::MouseButtonEvent btnEvent, Manager* pManager);
			virtual void MouseClicked(sf::Event::MouseButtonEvent btnEvent, Manager* pManager);
			virtual void MouseEntered(sf::Event::MouseMoveEvent movEvent, Manager* pManager);
			virtual void MouseLeft(sf::Event::MouseMoveEvent movEvent, Manager* pManager);

			//Attributes. Protected because we need them in CheckButton
			State uCurrent;
			sf::Texture *pTextures[INVALID];//because INVALID is last index + 1
			CALLBACK pCallback;
	};
}


#endif//_BUTTON_H
