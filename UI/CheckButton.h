#ifndef _CHECKBUTTON_H
#define _CHECKBUTTON_H
#include "Button.h"

namespace UI
{
	class CheckButton : public Button
	{
		///Basically it is the same as Button but with a few changes in mechanism.
		///We will be using states ACTIVE and INACTIVE to indicate checked and unchecked state
		///PRESSED will not be used
		public:
			//Constructors and Destructor
			CheckButton(uint32_t dwIdent, sf::Vector2i vPos = sf::Vector2i(0,0));
			CheckButton(uint32_t dwIdent, int32_t x = 0, int32_t y = 0);//Size is set by the textures like in Button
			~CheckButton() {}

			//Extra Methods other than the one Button provides
			const bool IsChecked() const;
			void SetChecked(bool bStatus = true);

		protected:
			//Overrides
			virtual void MousePressed(sf::Event::MouseButtonEvent btnEvent, Manager* pManager) {}
			virtual void MouseReleased(sf::Event::MouseButtonEvent btnEvent, Manager* pManager);
			virtual void MouseClicked(sf::Event::MouseButtonEvent btnEvent, Manager* pManager);
			virtual void MouseEntered(sf::Event::MouseMoveEvent movEvent, Manager* pManager);
			virtual void MouseLeft(sf::Event::MouseMoveEvent movEvent, Manager* pManager);
	};
}

#endif//_CHECKBUTTON_H
