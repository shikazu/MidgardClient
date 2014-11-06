#include "Manager.h"

namespace UI {

    Manager::Manager(sf::RenderTarget& target):Widget(0, 0, 0, target.getSize().x, target.getSize().y)
    {
        iFocused = lstFocusable.end();
        pWidgetHovered = NULL;
        pWidgetPressed = NULL;
    }

    Manager::~Manager()
    {
    }

    void Manager::SwitchFocus(Widget* pWidget)
    {
        for (WidgetList::iterator iter = lstFocusable.begin(); iter != lstFocusable.end(); iter++)
        {
            if (*iter == pWidget)
            {
                if (iFocused != lstFocusable.end()) { (*iFocused)->SetFocus(false); }
                iFocused = iter;
                (*iFocused)->SetFocus(true);
                break;
            }
        }
    }

    void Manager::SwitchFocus(bool bForward)
    {
        if (iFocused == lstFocusable.end())
        {
            iFocused = lstFocusable.begin();
            return;
        }
        (*iFocused)->SetFocus(false);
        if (bForward)
        {
            iFocused = (++iFocused == lstFocusable.end()) ? lstFocusable.begin() : iFocused;
        }
        else
        {
            iFocused = (iFocused == lstFocusable.begin()) ? lstFocusable.end() : iFocused;
            iFocused--;
        }
        (*iFocused)->SetFocus(true);
    }

    void Manager::AddFocus(Widget* pWidget, Widget* pBefore, Widget* pAfter)
    {
        if (!pWidget->IsFocusable()) return;
        if (pBefore == NULL && pAfter == NULL)
        {
            lstFocusable.push_back(pWidget);
            return;
        }
        for (WidgetList::iterator iter = lstFocusable.begin(); iter != lstFocusable.end(); iter++)
        {
            if (*iter == pBefore)
            {
                lstFocusable.insert(++iter, pWidget);
                break;
            }
            if (*iter == pAfter)
            {
                lstFocusable.insert(iter, pWidget);
                break;
            }
        }
    }

    bool Manager::ParseEvent(sf::Event event, Manager *pManager)
    {
        if (event.type == event.KeyPressed || event.type == event.KeyReleased || event.type == event.TextEntered)
        {
            if (iFocused != lstFocusable.end())
            {
                return ((*iFocused)->ParseEvent(event, this));
            }
            else
            {
                return false;
            }
        }
        else
        {
            bool bReturn = SpreadEvent(event, this);
            if (!bReturn && event.type == event.MouseButtonPressed)
            {
                (*iFocused)->SetFocus(false);
                iFocused = lstFocusable.end();
            }
            return bReturn;
        }
    }

    bool Manager::IsPressed(Widget *pWidget)
    {
        return (pWidget == pWidgetPressed);
    }
    bool Manager::IsPressed(Widget *pWidget, sf::Mouse::Button mbutton)
    {
        return ((pWidget == pWidgetPressed) && (mbutton == btnPressed));
    }
    void Manager::SetPressed(Widget* pWidget, sf::Mouse::Button mbutton)
    {
        if (pWidgetPressed != NULL && pWidget != NULL) return;//If button is already set and not trying to unset it
        pWidgetPressed = pWidget;
        btnPressed = mbutton;
    }
    bool Manager::IsHovered(Widget *pWidget)
    {
        return (pWidget == pWidgetHovered);
    }
    void Manager::SetHovered(Widget* pWidget)
    {
        pWidgetHovered = pWidget;
    }
}
