#include "UIWidget.h"
#include <list>

class UIManager
{
    public:
        enum CALL_ZONE
        {
            BEFORE_EVENTS = 0,
            BEFORE_DRAW,
            AFTER_DRAW,
            AFTER_DISPLAY,
            TOTAL_CALLS //Should always be last entry - only used as an endpoint dont use it elsewhere
        };

        typedef std::list<UIWidget*> WList;
        typedef void (*EventHandler)(UIManager*, sf::RenderWindow&, sf::Event);
        typedef void (*ManagerAddon)(UIManager*, sf::RenderWindow&);
        UIManager();
        virtual ~UIManager();
        void EventLoop(sf::RenderWindow &window);
        void AddWidget(UIWidget* pWidget, UIWidget* pBefore = NULL, UIWidget* pAfter = NULL);//add widget to the internal widget list
        void AddFocus(UIWidget* pWidget, UIWidget* pBefore = NULL, UIWidget* pAfter = NULL);//add to the focus list after pBefore or before pAfter
        void RemoveWidget(UIWidget* pWidget);
        void SetEventHandler(UIManager::EventHandler pFn);
        void SetManagerAddon(UIManager::ManagerAddon pFn, UIManager::CALL_ZONE position);

    private:
        WList lstWidgets;
        WList lstFocusWidgets;
        WList::iterator iKBFocus;  //index in lstFocusWidgets of currently focused widget for keyboard
        UIWidget* pMFocusWidget;//pWidget where Mouse Currently is - used for triggering entry and leave functions
        UIWidget* pMPressWidget;//pWidget index in lstWidgets where Mouse was Pressed - used for implementing mouse grab
        UIManager::EventHandler pHandler;
        UIManager::ManagerAddon pAddons[TOTAL_CALLS];
};
