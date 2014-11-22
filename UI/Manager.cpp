#include "Manager.h"

namespace UI {

	Manager::Manager(uint32_t dwID, sf::RenderTarget& target):Widget(dwID, ENABLED|VISIBLE, 0, 0, target.getSize().x, target.getSize().y)
	{
		iFocused = lstFocusable.end();
		pWidgetHovered = NULL;
		pWidgetPressed = NULL;
	}

	void Manager::SwitchFocus(Widget* pWidget)
	{
		if (pWidget == NULL)
		{
			if (iFocused != lstFocusable.end())
			{
				(*iFocused)->SetFocused(false);
			}
			iFocused = lstFocusable.end();
			return;
		}

		for (WidgetList::iterator iter = lstFocusable.begin(); iter != lstFocusable.end(); iter++)
		{
			if (*iter == pWidget)
			{
				if (iFocused != lstFocusable.end())
				{
					(*iFocused)->SetFocused(false);
				}
				iFocused = iter;
				(*iFocused)->SetFocused(true);
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
		(*iFocused)->SetFocused(false);
		if (bForward)
		{
			iFocused = (++iFocused == lstFocusable.end()) ? lstFocusable.begin() : iFocused;
		}
		else
		{
			iFocused = (iFocused == lstFocusable.begin()) ? lstFocusable.end() : iFocused;
			iFocused--;
		}
		(*iFocused)->SetFocused(true);
	}

	void Manager::AddFocus(Widget* pWidget, Widget* pBefore, Widget* pAfter)
	{
		if (pWidget == NULL || !pWidget->IsFocusable()) return;
		if (pBefore == NULL && pAfter == NULL)
		{
			lstFocusable.push_back(std::move(pWidget));
			return;
		}
		for (WidgetList::iterator iter = lstFocusable.begin(); iter != lstFocusable.end(); iter++)
		{
			if (*iter == pBefore)
			{
				lstFocusable.insert(++iter, std::move(pWidget));
				break;
			}
			if (*iter == pAfter)
			{
				lstFocusable.insert(iter, std::move(pWidget));
				break;
			}
		}
	}

	void Manager::RemoveFocus(Widget* pWidget)
	{
		if (pWidget == NULL) return;
		lstFocusable.remove(pWidget);
		if (*iFocused == pWidget) {iFocused = lstFocusable.end();}
	}

	Widget* Manager::GetFocused()
	{
		if (iFocused == lstFocusable.end())
		{
			return NULL;
		}
		else
		{
			return *iFocused;
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
	void Manager::SetPressed(Widget* pWidget)
	{
		pWidgetPressed = pWidget;
	}
	void Manager::SetPressed(Widget* pWidget, sf::Mouse::Button mbutton)
	{
		if (pWidgetPressed != NULL && pWidget != NULL) return;//If button is already set and not trying to unset it
		pWidgetPressed = pWidget;
		btnPressed = mbutton;
	}
	Widget* Manager::GetPressed()
	{
		return pWidgetPressed;
	}

	bool Manager::IsHovered(Widget *pWidget)
	{
		return (pWidget == pWidgetHovered);
	}
	void Manager::SetHovered(Widget* pWidget)
	{
		pWidgetHovered = pWidget;
	}
	Widget* Manager::GetHovered()
	{
		return pWidgetHovered;
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
			if (event.type == event.Resized)
			{
				SetWidth(event.size.width);
				SetHeight(event.size.height);
			}

			bool bReturn = SpreadEvent(event, this);
			if (!bReturn && event.type == event.MouseButtonPressed && iFocused != lstFocusable.end())
			{
				(*iFocused)->SetFocused(false);
				iFocused = lstFocusable.end();
			}
			if (event.type == event.MouseButtonReleased && pWidgetPressed != NULL)//Extra precaution in case it got skipped
			{
				pWidgetPressed->ParseEvent(event, this);
			}

			if (!bReturn && event.type == event.MouseMoved && pWidgetHovered != NULL)//Extra precaution like before one
			{
				pWidgetHovered->ParseEvent(event, this);
				pWidgetHovered = NULL;
				GetMouseCursor().SetState(CRS_DEFAULT);
			}
			return bReturn;
		}
	}
}
