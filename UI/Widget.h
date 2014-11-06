#ifndef _WIDGET_H
#define _WIDGET_H
#include <SFML/Graphics.hpp>
#include <list>
//#include <iostream>//cout

namespace UI
{
    class Widget;
    class Manager; //Top Most Widget
    typedef std::list<UI::Widget*> WidgetList;

    ///Alignment Enumerations
    enum VAlign
    {
        TOP = 0,
        CENTER,
        BOTTOM
    };
    enum HAlign
    {
        LEFT = 0,
        MIDDLE,
        RIGHT,
    };

    ///ZIndex Enumeration for Zindex operations
    enum ZIndex
    {
        TOP_MOST = 0,
        UP,
        DOWN,
        BOTTOM_MOST,
    };

    ///Color Identifier
    enum ColorID
    {
        FOREGROUND = 0,
        BACKGROUND,
        OUTLINE, //Border
        MAXID
    };

    ///One variable for 3 bit field
    enum WidgetMask
    {
        ENABLED   = 0x01,
        VISIBLE   = 0x02,
        CLICKABLE = 0x04,
        FOCUSABLE = 0x08,//Forget the spelling
        INFOCUS   = 0x10,
        EDITABLE  = 0x20//Only needed for Text Widgets but adding it anyways
    };

    ///Widget Class
    class Widget : public sf::Drawable
    {
        public:
            Widget(uint8_t uFlagMod = ENABLED|VISIBLE, sf::Vector2f vPos = sf::Vector2f(0,0), sf::Vector2f vSize = sf::Vector2f(20,20));
            Widget(uint8_t uFlagMod = ENABLED|VISIBLE, float x = 0, float y = 0, float w = 20,  float h = 20);
            virtual ~Widget();

            bool AttachTo(Widget* pWidget);
            void Detach();
            void AddChild(Widget* pWidget, Widget* pBefore = NULL, Widget* pAfter = NULL);
            void DelChild(Widget* pWidget, bool bCleanup = false);
            const Widget* GetParent() const;
            //WidgetList& GetChildren();

            const sf::Vector2f& GetPosition(bool bReal = false) const;//The real coordinates or relative coordinates
            void MoveTo(sf::Vector2f vPos);
            void MoveTo(float x, float y);
            void UpdateLocation();//Called to update the Physical Coordinates

            const sf::Vector2f& GetSize() const;
            void Resize(sf::Vector2f vSize);
            void Resize(float w, float h);
            void SetWidth(float w);
            const float GetWidth() const;
            void SetHeight(float h);
            const float GetHeight() const;

            const HAlign GetHAlignment() const;
            void SetHAlignment(HAlign h);

            const VAlign GetVAlignment() const;
            void SetVAlignment(VAlign v);
            void SetAlignment(HAlign h, VAlign v);

            const sf::Color& GetColor(ColorID id) const;
            void SetColor(sf::Color& color, ColorID id);
            const uint32_t GetBorderWidth() const;
            void SetBorderWidth(uint32_t dwBorder);
            const float GetCornerRadius() const;
            void SetCornerRadius(float fCorner);

            //Widget* GetSibling(ZIndex z);
            //void ChangeZIndex(ZIndex z);//change the order of appearance

            const bool IsEnabled() const;
            const bool IsVisible() const;
            const bool IsEditable() const;
            const bool HasFocus() const;
            const bool IsClickable() const;
            const bool IsFocusable() const;

            void SetEnabled(bool bStatus = true);
            void SetVisible(bool bStatus = true);
            void SetEditable(bool bStatus = true);
            void SetFocus(bool bStatus = true);

            bool ParseEvent(sf::Event event, Manager* pManager);//Checks the event and spreads it to children.
            sf::FloatRect GetBBox(bool bReal = true) const;//get the relative bbox or actual bbox
            bool IsPointInside(uint32_t x, uint32_t y);

        protected:
            virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
            virtual void MousePressed(sf::Event::MouseButtonEvent btnEvent, Manager* pManager) {}
            virtual void MouseReleased(sf::Event::MouseButtonEvent btnEvent, Manager* pManager) {}
            virtual void MouseClicked(sf::Event::MouseButtonEvent btnEvent, Manager* pManager) {}
            virtual void MouseEntered(sf::Event::MouseMoveEvent movEvent, Manager* pManager) {}
            virtual void MouseLeft(sf::Event::MouseMoveEvent movEvent, Manager* pManager) {}
            virtual void MouseMoved(sf::Event::MouseMoveEvent movEvent, Manager* pManager) {}

            virtual void KeyPressed(sf::Event::KeyEvent keyEvent, Manager* pManager) {}
            virtual void KeyReleased(sf::Event::KeyEvent keyEvent, Manager* pManager) {}
            virtual void TextEntered(sf::Event::TextEvent textEvent, Manager* pManager) {}

            bool SpreadEvent(sf::Event event, Manager* pManager);
            void DrawBorder(sf::RenderTarget& target, sf::RenderStates states) const;
            WidgetList lstChildren;

        private:

            uint8_t uFlag;
            sf::Vector2f vSize, vPos, vPosReal;//vPos = Relative Position & vPosReal = Actual Location in Window
            sf::Color pColors[MAXID];
            uint32_t dwBorderWidth;
            float fCornerRadius;
            HAlign uAlignH;
            VAlign uAlignV;
            Widget* pParent;

            bool isFlagSet(WidgetMask mask) const;
            void setFlag(WidgetMask mask, bool bStatus);
    };
}
#endif // _WIDGET_H
