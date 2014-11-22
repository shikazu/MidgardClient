#ifndef _CHARVIEW_H
#define _CHARVIEW_H

#include <SFML/Graphics.hpp>
#include "../UI/Manager.h"

namespace CharView
{
	void Init();
	void Create(UI::Manager &mgr);
	void HandleOK(UI::Widget* pButton, UI::Manager* pManager);
	void HandleCancel(UI::Widget* pButton, UI::Manager* pManager);
	void HandleDelete(UI::Widget* pButton, UI::Manager* pManager);
	void HandleCreate(UI::Widget* pButton, UI::Manager* pManager);
	void HandleScroll(UI::Widget* pButton, UI::Manager* pManager);
	bool IsOkPressed();
	bool IsCancelled();
	bool IsDelPressed();
};

#endif//_CHARVIEW_H
