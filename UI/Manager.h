#ifndef _MANAGER_H
#define _MANAGER_H
#include "Widget.h"

namespace UI
{
	class Manager : public Widget
	{
		public:
			//Constructors and Destructor
			Manager(uint32_t dwID, sf::RenderTarget& target);
			~Manager() {}

			//Focus Widget Methods
			void SwitchFocus(Widget* pWidget);
			void SwitchFocus(bool bForward = true);
			void AddFocus(Widget* pWidget, Widget* pBefore = NULL, Widget* pAfter = NULL);
			void RemoveFocus(Widget* pWidget);
			Widget* GetFocused();

			//Pressed Widget Methods
			bool IsPressed(Widget* pWidget);
			bool IsPressed(Widget* pWidget, sf::Mouse::Button mbutton);
			void SetPressed(Widget* pWidget, sf::Mouse::Button mbutton);
			Widget* GetPressed();

			//Hovered Widget Methods
			bool IsHovered(Widget* pWidget);
			void SetHovered(Widget* pWidget);
			Widget* GetHovered();

			//Miscellaneous Methods
			bool ParseEvent(sf::Event event, Manager* pManager = NULL);//Manager argument is dummy

		private:
			WidgetList lstFocusable;
			WidgetList::iterator iFocused;
			Widget* pWidgetPressed;
			Widget* pWidgetHovered;
			sf::Mouse::Button btnPressed;
	};
}
#endif//_MANAGER_H
