#include "Manager.h"

namespace UI {

    Manager::Manager(sf::RenderTarget& target):Widget(0, 0, target.getSize().x, target.getSize().y)
    {
        iFocused = lstChildren.begin();
        pWidgetHovered = NULL;
        pWidgetPressed = NULL;
    }

    Manager::~Manager()
    {
    }

    void Manager::SwitchFocus(Widget* pWidget)
    {
        for (WidgetList::iterator iter = lstChildren.begin(); iter != lstChildren.end(); iter++)
        {
            if (*iter == pWidget)
            {
                (*iFocused)->SetFocus(false);
                iFocused = iter;
                (*iFocused)->SetFocus(true);
                break;
            }
        }
    }

    void Manager::SwitchFocus(bool bForward)
    {
        (*iFocused)->SetFocus(false);
        if (bForward)
        {
            iFocused = (++iFocused == lstChildren.end()) ? lstChildren.begin() : iFocused;
        }
        else
        {
            iFocused = (iFocused == lstChildren.begin()) ? lstChildren.end() : iFocused;
            iFocused--;
        }
        (*iFocused)->SetFocus(true);
    }

    void Manager::AddFocus(Widget* pWidget, Widget* pBefore, Widget* pAfter)
    {
        if (!pWidget->IsFocusable()) return;
        if (pBefore == NULL && pAfter == NULL)
        {
            lstChildren.push_back(pBefore);
            return;
        }
        for (WidgetList::iterator iter = lstChildren.begin(); iter != lstChildren.end(); iter++)
        {
            if (*iter == pBefore)
            {
                lstChildren.insert(++iter, pWidget);
                break;
            }
            if (*iter == pAfter)
            {
                lstChildren.insert(iter, pWidget);
                break;
            }
        }
    }

    bool Manager::ParseEvent(sf::Event event, Manager *pManager)
    {
        if (event.type == event.KeyPressed || event.type == event.KeyReleased || event.type == event.TextEntered)
        {
            if (*iFocused != NULL)
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
            return SpreadEvent(event, this);
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
