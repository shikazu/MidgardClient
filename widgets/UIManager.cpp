#include "UIManager.h"

UIManager::UIManager()
{
    iKBFocus = lstFocusWidgets.end();
    pMFocusWidget = NULL;
    pMPressWidget = NULL;
    pHandler = NULL;
    for (int i = 0; i < TOTAL_CALLS; i++)
    {
        pAddons[i] = NULL;
    }
}

UIManager::~UIManager()
{

}
void UIManager::EventLoop(sf::RenderWindow &window)
{
    while (window.isOpen())
    {
        if (pAddons[BEFORE_EVENTS] != NULL) (*pAddons[BEFORE_EVENTS])(this, window);

        sf::Event event;
        while(window.pollEvent(event))
        {
            if (event.type == event.Closed)
            {
                window.close();
            }
            if (iKBFocus != lstFocusWidgets.end())
            {
                if (event.type == event.KeyPressed)
                {
                    (*iKBFocus)->KeyPressed(event.key);
                    if (event.key.code ==  sf::Keyboard::Tab && !lstFocusWidgets.empty())
                    {
                        (*iKBFocus)->KillFocus();
                        if (event.key.shift)
                        {
                            iKBFocus = (iKBFocus == lstFocusWidgets.begin()) ? lstFocusWidgets.rbegin().base() : --iKBFocus;
                        }
                        else
                        {
                            iKBFocus = (++iKBFocus == lstFocusWidgets.end()) ? lstFocusWidgets.begin() : iKBFocus;
                        }
                        (*iKBFocus)->SetFocus();
                    }
                }
                if (event.type == event.KeyReleased)
                {
                    (*iKBFocus)->KeyReleased(event.key);
                }
                if (event.type == event.TextEntered)
                {
                    (*iKBFocus)->TextEntered(event.text);
                }
            }
            UIWidget* pMFocusWidgetOld = pMFocusWidget;
            pMFocusWidget = NULL;

            for ( WList::reverse_iterator iter = lstWidgets.rbegin(); iter != lstWidgets.rend() ; iter++ )
            {
                UIWidget* pWidget = *iter;
                if (event.type == event.MouseButtonPressed)
                {
                    if (!pWidget->IsClickable()) continue;//Check if Mouse Click need to be detected or not;
                    if (pWidget->IsPointInside(event.mouseButton.x, event.mouseButton.y))
                    {
                        pWidget->MousePressed(event.mouseButton);
                        pMPressWidget = *iter;
                        break;
                    }
                }
                if (event.type == event.MouseButtonReleased)
                {
                    if (!pWidget->IsClickable()) continue;//Check if Mouse Click need to be detected or not;
                    if (pWidget->IsPointInside(event.mouseButton.x, event.mouseButton.y))
                    {
                        pWidget->MouseReleased(event.mouseButton);
                        if (*iter == pMPressWidget)
                        {
                            pWidget->MouseClicked(event.mouseButton);
                        }
                        break;
                    }
                }
                if (event.type == event.MouseMoved)//Detect Entry and Exit
                {
                    if (pWidget->IsPointInside(event.mouseMove.x, event.mouseMove.y))
                    {
                        if (*iter == pMFocusWidgetOld)
                        {
                            pWidget->MouseMoved(event.mouseMove);
                        }
                        else
                        {
                            pWidget->MouseEntered(event.mouseMove);
                        }
                        pMFocusWidget = *iter;
                        break;
                    }
                }
            }
            if (event.type == event.MouseLeft && pMPressWidget != NULL)
            {
                sf::Event::MouseButtonEvent dummy = {sf::Mouse::Left, -1, -1};
                pMPressWidget->MouseReleased(dummy);
                pMPressWidget = NULL;
            }

            if (event.type == event.MouseButtonReleased)
            {
                pMPressWidget = NULL;
            }

            if ((pMFocusWidget == NULL || pMFocusWidget != pMFocusWidgetOld) && pMFocusWidgetOld != NULL)//mousefocus == NULL, means it went outside into the window, focus != focusOld means it entered another widget
            {
                pMFocusWidgetOld->MouseLeft(event.mouseMove);
            }
            if (pHandler != NULL)
            {
                (*pHandler)(this, window, event);
            }
        }
        window.clear();
        if (pAddons[BEFORE_DRAW] != NULL) (*pAddons[BEFORE_DRAW])(this, window);

        for (WList::iterator iter = lstWidgets.begin(); iter != lstWidgets.end(); iter++)
        {
            (*iter)->Paint(window);
        }

        if (pAddons[AFTER_DRAW] != NULL) (*pAddons[AFTER_DRAW])(this, window);

        window.display();

        if (pAddons[AFTER_DISPLAY] != NULL) (*pAddons[AFTER_DISPLAY])(this, window);
    }
}

void UIManager::AddWidget(UIWidget* pWidget, UIWidget* pBefore, UIWidget* pAfter)
{
    if (pBefore == NULL && pAfter == NULL)
    {
        lstWidgets.push_back(pWidget);
        return;
    }
    for (WList::iterator iter = lstWidgets.begin(); iter != lstWidgets.end(); iter++)
    {
        if (pBefore == *iter)
        {
            lstWidgets.insert(++iter, pWidget);
            break;
        }
        if (pAfter == *iter)
        {
            lstWidgets.insert(iter, pWidget);
            break;
        }
    }
}

void UIManager::AddFocus(UIWidget* pWidget, UIWidget* pBefore, UIWidget* pAfter)
{
    if (pBefore == NULL && pAfter == NULL)
    {
        lstFocusWidgets.push_back(pWidget);
        return;
    }
    for (WList::iterator iter = lstFocusWidgets.begin(); iter != lstFocusWidgets.end(); iter++)
    {
        if (pBefore == *iter)
        {
            lstFocusWidgets.insert(++iter, pWidget);
            break;
        }
        if (pAfter == *iter)
        {
            lstFocusWidgets.insert(iter, pWidget);
            break;
        }
    }
}

void UIManager::RemoveWidget(UIWidget* pWidget)
{
    for (WList::iterator iter = lstWidgets.begin(); iter != lstWidgets.end(); iter++)
    {
        if (*iter == pWidget) {
            lstWidgets.erase(iter);
            break;
        }
    }
    for (WList::iterator iter = lstFocusWidgets.begin(); iter != lstFocusWidgets.end(); iter++)
    {
        if (*iter == pWidget) {
            lstFocusWidgets.erase(iter);
            break;
        }
    }
}

void UIManager::SetEventHandler(UIManager::EventHandler pFn)
{
    pHandler = pFn;
}

void UIManager::SetManagerAddon(UIManager::ManagerAddon pFn, UIManager::CALL_ZONE position)
{
    if (position >= TOTAL_CALLS) return;
    pAddons[position] = pFn;
}
