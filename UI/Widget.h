#ifndef _WIDGET_H
#define _WIDGET_H
#define XLEFT 3 //Minimum Spacing from Left for Text

#include <SFML/Graphics.hpp>
#include <list>
#include "../common/Globals.h"
//#include <iostream>

namespace UI
{
	///--- Forward Declarations ---///

	//Base Widget Class
	class Widget;

	//Manager class which handles everything from window.
	//Derived from Widget
	class Manager;


	///--- Typedefs ---///

	//List of Widgets
	typedef std::list<Widget*> WidgetList;

	//Common Callback Function Template
	typedef void (*CALLBACK)(Widget *pWidget, Manager *pManager);

	///--- Enumerations ---///

	//Horizontal Alignment
	enum HAlign
	{
		LEFT = 0,
		CENTER,
		RIGHT
	};

	//Vertical Alignment
	enum VAlign
	{
		TOP = 0,
		MIDDLE,
		BOTTOM
	};

	//Color Identifier
	enum ColorID
	{
		FOREGROUND = 0,
		BACKGROUND,
		OUTLINE,
		MAX_ID
	};

	//Widget Flag
	enum WFlag
	{
		ENABLED   = 0x0001,
		VISIBLE   = 0x0002,
		CLICKABLE = 0x0004,
		EDITABLE	= 0x0008,
		FOCUSABLE = 0x0010,
		DRAGGABLE = 0x0020,
		FOCUSED   = 0x0040,
		DRAGGED   = 0x0080,
		TEXTURED  = 0x0100
	};

	//Text Style
	enum TStyle
	{
		REGULAR = 0x00,
		BOLD   = 0x01,
		ITALIC = 0x02
	};

	///--- Class Definition ---///
	class Widget : public sf::Drawable
	{
		public:
			//Constructors and Destructor
			Widget(uint32_t dwID, uint16_t wFlag = ENABLED|VISIBLE, sf::Vector2f vPos = sf::Vector2f(0,0), sf::Vector2f vSize = sf::Vector2f(0,0));
			Widget(uint32_t dwID, uint16_t wFlag = ENABLED|VISIBLE, float x = 0, float y = 0, float w = 0,  float h = 0);
			virtual ~Widget();

			//Hierarchy Methods - Setters
			bool Attach(Widget* pWinParent);//Assign Parent Widget
			void Detach();//Unset Parent Widget
			void AddChild(Widget* pChild, Widget* pBefore = NULL, Widget* pAfter = NULL);//Add Child Widget
			void DelChild(Widget* pChild, Manager* pManager = NULL, bool bCleanup = true);//Remove Child Widget - Optionally delete the Widget itself
			void DelChildren(Manager* pManager = NULL, bool bCleanup = true);

			//Hierarchy Methods - Getters
			Widget* GetParent() const;
			Widget* GetChild(uint32_t dwChild) const;
			const WidgetList& GetChildren() const;

			//ID Methods
			const uint32_t GetID() const;

			//Geometry Methods - Setters
			void MoveTo(sf::Vector2f vPos, bool bIsOffset = false);//Sets the position to value send (can be relative to current value). Only Widget position relative to parent can be set.
			void MoveTo(float x, float y, bool bIsOffset = false);

			void Resize(sf::Vector2f vSize, bool bIsOffset = false);//Sets the Width and Height to value send (can be relative to current value)
			void Resize(float w, float h, bool bIsOffset = false);
			void SetWidth(float w);
			void SetHeight(float h);

			void SetDragArea(sf::FloatRect rArea);//Area where mouse drag is detected. Relative to the current widget i.e. 0,0 represents top left
			void SetDragArea(float x, float y, float w, float h);

			//Geometry Methods - Getters (Self Explanatory)
			const sf::Vector2f& GetPosition(bool bReal = true) const;
			const float GetX(bool bReal = true) const;
			const float GetY(bool bReal = true) const;

			const sf::Vector2f& GetSize() const;
			const float GetWidth() const;
			const float GetHeight() const;

			const sf::FloatRect GetBBox(bool bReal = true) const;//Get the Bounding Box of the Widget
			const sf::FloatRect& GetDragArea() const;//Get the Draggable Area of the Widget

			//Alignment Methods - Setters
			void SetHAlign(HAlign h);
			void SetVAlign(VAlign v);
			void SetAlign(HAlign h, VAlign v);

			//Alignment Methods - Getters
			const HAlign GetHAlign() const;
			const VAlign GetVAlign() const;

			//Flag Methods - Setters
			void SetEnabled(  bool bStatus = true);
			void SetVisible(  bool bStatus = true);
			void SetClickable(bool bStatus = true);
			void SetEditable( bool bStatus = true);
			void SetFocusable(bool bStatus = true);
			void SetDraggable(bool bStatus = true);
			void SetFocused(  bool bStatus = true);
			void SetDragged(  bool bStatus = true);
			void SetTextured( bool bStatus = true);

			//Flag Methods - Getters
			const bool IsEnabled()   const;
			const bool IsVisible()   const;
			const bool IsClickable() const;
			const bool IsEditable()  const;
			const bool IsFocusable() const;
			const bool IsDraggable() const;
			const bool IsFocused()   const;
			const bool IsDragged()   const;
			const bool IsTextured()  const;

			//Miscellaneous Methods - Setters
			void SetColor(sf::Color color, ColorID cid);
			void SetBorderWidth(uint32_t dwBorder);
			void SetCornerRadius(uint32_t dwCorner);

			//Miscellaneous Methods - Getters
			const sf::Color& GetColor(ColorID cid) const;
			const uint32_t GetBorderWidth() const;
			const uint32_t GetCornerRadius() const;

			//Utility Methods
			bool IsPointInside(uint32_t x, uint32_t y);//Check if the point is inside the Bounding Box
			void UpdateLocation();//Updates the Physical Location value
			virtual bool ParseEvent(sf::Event event, Manager* pManager);//Checks the event and spreads to children

		protected:
			//Event callbacks
			virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

			virtual void MousePressed(sf::Event::MouseButtonEvent btnEvent, Manager* pManager) {}
			virtual void MouseReleased(sf::Event::MouseButtonEvent btnEvent, Manager* pManager) {}
			virtual void MouseClicked(sf::Event::MouseButtonEvent btnEvent, Manager* pManager) {}
			virtual void MouseEntered(sf::Event::MouseMoveEvent movEvent, Manager* pManager) {}
			virtual void MouseLeft(sf::Event::MouseMoveEvent movEvent, Manager* pManager) {}
			virtual void MouseMoved(sf::Event::MouseMoveEvent movEvent, Manager* pManager) {}

			virtual void KeyPressed(sf::Event::KeyEvent keyEvent, Manager* pManager) {}
			virtual void KeyReleased(sf::Event::KeyEvent keyEvent, Manager* pManager) {}
			virtual void TextEntered(sf::Event::TextEvent textEvent, Manager* pManager) {}

			virtual void WinResized() {}//Special Event - triggered when the RenderWindow is Resized
			virtual void Dragged();//Special Event - triggered when the Widget is Dragged

			//Utility Methods
			bool SpreadEvent(sf::Event event, Manager* pManager);//Does the Spreading part in Parse Event
			const sf::VertexArray& GetBorderVA() const;
			const sf::VertexArray& GetBackGroundVA() const;
			const sf::VertexArray& GetTextVA() const;
			void UpdateTextVA(uint32_t dwFontIndex, const sf::String &text, uint8_t uStyle, uint32_t dwCharSize, uint32_t dwTextWidth, uint32_t dwNumLines = 1, uint32_t dwStartIndex = 0, const sf::Vector2f& vTextPos = sf::Vector2f(0,0), char cPass = 0) const;//Font Index to get the Font from Global Font Array

		private:
			//Attributes
			uint32_t dwID;
			uint16_t wFlag;//Combination of WFlag values
			sf::Vector2f vSize, vDrag; //vSize = Size of Widget, vDrag = Reference Point for Drag implementation
			sf::Vector2f vPos, vPosReal;//vPos = Relative Position w.r.t. Parent, vPosReal = Actual Position in Window
			sf::FloatRect rDrag;
			sf::Color pColor[MAX_ID];
			uint32_t dwBorderWidth, dwCornerRadius;//Self Explanatory
			HAlign halign;
			VAlign valign;
			Widget* pWinParent;
			WidgetList lstChildren;
			mutable sf::VertexArray vaBorder, vaBack, vaText;

			//Methods
			const bool isFlagSet(WFlag mask) const;
			void setFlag(WFlag mask, bool bStatus = true);
			void updateVertices();
			void genRounded(ColorID cid, sf::VertexArray &vertArray);
			void genUnRounded(ColorID cid, sf::VertexArray &vertArray);
			Manager* getManager() const;//Only used in delete functions
	};
}
#endif//_WIDGET_H
