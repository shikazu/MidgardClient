#ifndef _MANAGER_H
#define _MANAGER_H
#include "Widget.h"

namespace UI
{
    class Manager : public Widget
    {
        public:
            Manager(sf::RenderTarget& target);
            virtual ~Manager();

            void SwitchFocus(Widget* pWidget);
            void SwitchFocus(bool bForward = true);
            void AddFocus(Widget* pWidget, Widget* pBefore = NULL, Widget* pAfter = NULL);
            bool ParseEvent(sf::Event event, Manager* pManager = NULL);//Manager argument is dummy

            bool IsPressed(Widget* pWidget);
            bool IsPressed(Widget* pWidget, sf::Mouse::Button mbutton);
            void SetPressed(Widget* pWidget, sf::Mouse::Button mbutton);
            bool IsHovered(Widget* pWidget);
            void SetHovered(Widget* pWidget);

        private:
            WidgetList lstFocusable;
            WidgetList::iterator iFocused;
            Widget* pWidgetPressed;
            Widget* pWidgetHovered;
            sf::Mouse::Button btnPressed;
    };
}
#endif // _MANAGER_H
