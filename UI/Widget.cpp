#include "Widget.h"
#include "Manager.h"

namespace UI
{
	///Constructors + Destructor
	Widget::Widget(uint32_t dwID, uint16_t wFlag, sf::Vector2f vPos, sf::Vector2f vSize)
	{
		this->dwID = dwID;
		this->wFlag = wFlag;
		this->vPos = vPos;
		this->vSize = vSize;
		this->dwBorderWidth = 0;
		this->dwCornerRadius = 0;
		this->halign = LEFT;
		this->valign = TOP;
		this->pWinParent = NULL;
		this->vaBorder = sf::VertexArray(sf::TrianglesStrip);
		this->vaBack = sf::VertexArray(sf::TrianglesFan);
		this->vaText = sf::VertexArray(sf::Triangles);
		this->pColor[FOREGROUND] = this->pColor[BACKGROUND] = this->pColor[OUTLINE] = sf::Color::Transparent;
		UpdateLocation();
	}

	Widget::Widget(uint32_t dwID, uint16_t wFlag, float x, float y, float w,  float h)
	{
		this->dwID = dwID;
		this->wFlag = wFlag;
		this->vPos.x = x;
		this->vPos.y = y;
		this->vSize.x = w;
		this->vSize.y = h;
		this->dwBorderWidth = 0;
		this->dwCornerRadius = 0;
		this->halign = LEFT;
		this->valign = TOP;
		this->pWinParent = NULL;
		this->vaBorder = sf::VertexArray(sf::TrianglesStrip);
		this->vaBack = sf::VertexArray(sf::TrianglesFan);
		this->vaText = sf::VertexArray(sf::Triangles);
		this->pColor[FOREGROUND] = this->pColor[BACKGROUND] = this->pColor[OUTLINE] = sf::Color::Transparent;
		UpdateLocation();
	}

	Widget::~Widget()
	{
		DelChildren();
	}

	///Public Methods
	bool Widget::Attach(Widget* pWinParent)
	{
		if (this->pWinParent != NULL) { return false; }
		this->pWinParent = pWinParent;
		UpdateLocation();
		return true;
	}

	void Widget::Detach()
	{
		this->pWinParent = NULL;
		UpdateLocation();
	}

	void Widget::AddChild(Widget* pChild, Widget* pBefore, Widget* pAfter)
	{
		if (pBefore == NULL && pAfter == NULL)
		{
			lstChildren.push_back(std::move(pChild));
			pChild->Attach(this);
			return;
		}
		for (WidgetList::iterator iter = lstChildren.begin(); iter != lstChildren.end(); iter++)
		{
			if (*iter == pBefore)
			{
				lstChildren.insert(++iter, std::move(pChild));
				pChild->Attach(this);
				break;
			}
			if (*iter == pAfter)
			{
				lstChildren.insert(iter, std::move(pChild));
				pChild->Attach(this);
				break;
			}
		}
	}

	void Widget::DelChild(Widget* pChild, Manager* pManager, bool bCleanup)
	{
		if (pChild == NULL) { return; }
		if (bCleanup)
		{
			if (pManager == NULL) {pManager = getManager();}
			if (pManager->IsHovered(pChild)) {pManager->SetHovered(NULL);}
			if (pManager->IsPressed(pChild)) {pManager->SetPressed(NULL);}
			pManager->RemoveFocus(pChild);
			delete pChild;
		}
		else
		{
			pChild->Detach();
		}
		lstChildren.remove(pChild);
	}

	void Widget::DelChildren(Manager* pManager, bool bCleanup)
	{
		if (pManager == NULL) {pManager = getManager();}
		for (WidgetList::iterator iter = lstChildren.begin(); iter != lstChildren.end(); iter++)
		{
			if (bCleanup)
			{
				if (pManager->IsHovered(*iter)) {pManager->SetHovered(NULL);}
				if (pManager->IsPressed(*iter)) {pManager->SetPressed(NULL);}
				pManager->RemoveFocus(*iter);
				delete *iter;
			}
			else
			{
				(*iter)->Detach();
			}
		}
		lstChildren.clear();
	}

	Widget* Widget::GetParent() const
	{
		return pWinParent;
	}

	Widget* Widget::GetChild(uint32_t dwChild) const
	{
		Widget* pChild = NULL;
		for (WidgetList::const_iterator iter = lstChildren.cbegin(); iter != lstChildren.cend(); iter++)
		{
			pChild = *iter;
			if (pChild->GetID() == dwChild) { break; }
		}
		return pChild;
	}

	const WidgetList& Widget::GetChildren() const
	{
		return lstChildren;
	}

	const uint32_t Widget::GetID() const
	{
		return dwID;
	}

	void Widget::MoveTo(sf::Vector2f vPos, bool bIsOffset)
	{
		if (bIsOffset)
		{
			this->vPos = this->vPos + vPos;
		}
		else
		{
			this->vPos = vPos;
		}
		UpdateLocation();
	}

	void Widget::MoveTo(float x, float y, bool bIsOffset)
	{
		if (bIsOffset)
		{
			vPos.x += x;
			vPos.y += y;
		}
		else
		{
			vPos.x  = x;
			vPos.y  = y;
		}
		UpdateLocation();
	}

	void Widget::Resize(sf::Vector2f vSize, bool bIsOffset)
	{
		if (bIsOffset)
		{
			this->vSize = this->vSize + vSize;
		}
		else
		{
			this->vSize = vSize;
		}
		UpdateLocation();
	}

	void Widget::Resize(float w, float h, bool bIsOffset)
	{
		if (bIsOffset)
		{
			vSize.x += w;
			vSize.y += h;
		}
		else
		{
			vSize.x  = w;
			vSize.y  = h;
		}
		UpdateLocation();
	}

	void Widget::SetWidth(float w)
	{
		vSize.x = w;
		UpdateLocation();
	}

	void Widget::SetHeight(float h)
	{
		vSize.y = h;
		UpdateLocation();
	}

	void Widget::SetDragArea(sf::FloatRect rArea)
	{
		rDrag = rArea;
	}

	void Widget::SetDragArea(float x, float y, float w, float h)
	{
		rDrag.left   = x;
		rDrag.top	   = y;
		rDrag.width  = w;
		rDrag.height = h;
	}

	const sf::Vector2f& Widget::GetPosition(bool bReal) const
	{
		if (bReal)
		{
			return vPosReal;
		}
		else
		{
			return vPos;
		}
	}

	const float Widget::GetX(bool bReal) const
	{
		if (bReal)
		{
			return vPosReal.x;
		}
		else
		{
			return vPos.x;
		}
	}

	const float Widget::GetY(bool bReal) const
	{
		if (bReal)
		{
			return vPosReal.y;
		}
		else
		{
			return vPos.y;
		}
	}

	const sf::Vector2f& Widget::GetSize() const
	{
		return vSize;
	}

	const float Widget::GetWidth() const
	{
		return vSize.x;
	}

	const float Widget::GetHeight() const
	{
		return vSize.y;
	}

	const sf::FloatRect Widget::GetBBox(bool bReal) const
	{
		if (bReal)
		{
			return sf::FloatRect(vPosReal.x, vPosReal.y, vSize.x, vSize.y);
		}
		else
		{
			return sf::FloatRect(vPos.x, vPos.y, vSize.x, vSize.y);
		}
	}

	const sf::FloatRect& Widget::GetDragArea() const
	{
		return rDrag;
	}

	void Widget::SetHAlign(HAlign h)
	{
		halign = h;
		UpdateLocation();
	}

	void Widget::SetVAlign(VAlign v)
	{
		valign = v;
		UpdateLocation();
	}

	void Widget::SetAlign(HAlign h, VAlign v)
	{
		halign = h;
		valign = v;
		UpdateLocation();
	}

	const HAlign Widget::GetHAlign() const
	{
		return halign;
	}

	const VAlign Widget::GetVAlign() const
	{
		return valign;
	}

	void Widget::SetEnabled(  bool bStatus) { setFlag(ENABLED  ,  bStatus); }
	void Widget::SetVisible(  bool bStatus) { setFlag(VISIBLE  ,  bStatus); }
	void Widget::SetClickable(bool bStatus) { setFlag(CLICKABLE,  bStatus); }
	void Widget::SetEditable( bool bStatus) { setFlag(EDITABLE ,  bStatus); }
	void Widget::SetFocusable(bool bStatus) { setFlag(FOCUSABLE,  bStatus); }
	void Widget::SetDraggable(bool bStatus) { setFlag(DRAGGABLE,  bStatus); }
	void Widget::SetFocused(  bool bStatus) { setFlag(FOCUSED  ,  bStatus); }
	void Widget::SetDragged(  bool bStatus) { setFlag(DRAGGED  ,  bStatus); }
	void Widget::SetTextured( bool bStatus) { setFlag(TEXTURED ,  bStatus); UpdateLocation();}

	const bool Widget::IsEnabled()   const 	{ return isFlagSet(ENABLED  ); }
	const bool Widget::IsVisible()   const 	{ return isFlagSet(VISIBLE  ); }
	const bool Widget::IsClickable() const 	{ return isFlagSet(CLICKABLE); }
	const bool Widget::IsEditable()  const 	{ return isFlagSet(EDITABLE ); }
	const bool Widget::IsFocusable() const 	{ return isFlagSet(FOCUSABLE); }
	const bool Widget::IsDraggable() const 	{ return isFlagSet(DRAGGABLE); }
	const bool Widget::IsFocused()   const 	{ return isFlagSet(FOCUSED  ); }
	const bool Widget::IsDragged()   const 	{ return isFlagSet(DRAGGED  ); }
	const bool Widget::IsTextured()  const 	{ return isFlagSet(TEXTURED ); }

	void Widget::SetColor(sf::Color color, ColorID cid)
	{
		if (cid >= MAX_ID) { return; }
		pColor[cid] = color;
		UpdateLocation();
	}

	void Widget::SetBorderWidth(uint32_t dwBorder)
	{
		dwBorderWidth = dwBorder;
		UpdateLocation();
	}

	void Widget::SetCornerRadius(uint32_t dwCorner)
	{
		dwCornerRadius = dwCorner;
		UpdateLocation();
	}

	const sf::Color& Widget::GetColor(ColorID cid) const
	{
		if (cid >= MAX_ID) { cid = MAX_ID; }
		return pColor[cid];
	}

	const uint32_t Widget::GetBorderWidth() const
	{
		return dwBorderWidth;
	}

	const uint32_t Widget::GetCornerRadius() const
	{
		return dwCornerRadius;
	}

	bool Widget::IsPointInside(uint32_t x, uint32_t y)
	{
		return GetBBox().contains(x, y);
	}

	void Widget::UpdateLocation()
	{
		sf::Vector2f vRef;
		sf::Vector2f vSizeDiff;

		//First update the Physical Position
		if (pWinParent == NULL)
		{
			vRef = vPos;
			vSizeDiff = sf::Vector2f(0, 0);
		}
		else
		{
			vRef = vPos + pWinParent->GetPosition();
			vSizeDiff = pWinParent->GetSize() - vSize;
		}

		switch (halign)
		{
			case LEFT:   {vPosReal.x = vRef.x                           ; break;}
			case CENTER: {vPosReal.x = vRef.x + (int32_t)(vSizeDiff.x/2); break;}
			case RIGHT:  {vPosReal.x = vRef.x + (int32_t)(vSizeDiff.x  ); break;}
		}

		switch (valign)
		{
			case TOP:	   {vPosReal.y = vRef.y                         ; break;}
			case MIDDLE: {vPosReal.y = vRef.y + (int32_t)(vSizeDiff.y/2); break;}
			case BOTTOM: {vPosReal.y = vRef.y + (int32_t)(vSizeDiff.y  ); break;}
		}

		//Now update the vertex arrays
		updateVertices();

		//Repeat for all children
		for (WidgetList::iterator iter = lstChildren.begin(); iter != lstChildren.end(); iter++)
		{
			Widget* pChild = *iter;
			pChild->UpdateLocation();
		}
	}

	bool Widget::ParseEvent(sf::Event event, Manager* pManager)
	{
		bool bFlag = false;
		//Sanity Check
		if (!IsVisible() || !IsEnabled()) { return bFlag; }

		//Keyboard Events
		if (event.type == event.KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Tab)
			{
				if (event.key.shift)
				{
					pManager->SwitchFocus(false);
				}
				else
				{
					pManager->SwitchFocus(true);
				}
			}
			KeyPressed(event.key, pManager);
		}
		if (event.type == event.KeyReleased)
		{
			KeyReleased(event.key, pManager);
		}
		if (event.type == event.TextEntered)
		{
			TextEntered(event.text, pManager);
		}

		//Window Resize Event
		if (event.type == event.Resized)
		{
			UpdateLocation();
			SpreadEvent(event, pManager);
			WinResized();
			return bFlag;
		}

		//Mouse Events
		if (event.type == event.MouseButtonPressed && IsPointInside(event.mouseButton.x, event.mouseButton.y))
		{
			bFlag = SpreadEvent(event, pManager);
			if (bFlag) {return bFlag;}
			MousePressed(event.mouseButton, pManager);

			//Drag Detection Start
			if (IsDraggable() && rDrag.contains(event.mouseButton.x - vPosReal.x, event.mouseButton.y - vPosReal.y))
			{
				SetDragged();
				vDrag.x = event.mouseButton.x;
				vDrag.y = event.mouseButton.y;
			}

			//Click Detection Start
			if (IsClickable() && pManager->IsPressed(NULL))
			{
				pManager->SetPressed(this, event.mouseButton.button);
			}

			//Focus Detection - Mouse Press switches Focus
			if (IsFocusable())
			{
				pManager->SwitchFocus(this);
			}
			bFlag = true;
		}

		if (event.type == event.MouseButtonReleased && IsPointInside(event.mouseButton.x, event.mouseButton.y))
		{
			bFlag = SpreadEvent(event, pManager);
			if (bFlag) {return bFlag;}
			MouseReleased(event.mouseButton, pManager);

			//Switch off Drag Detection
			SetDragged(false);

			//Detect Click
			if (pManager->IsPressed(this, event.mouseButton.button))
			{
				MouseClicked(event.mouseButton, pManager);
				pManager->SetPressed(NULL, event.mouseButton.button);
			}
			bFlag = true;
		}

		if (event.type == event.MouseButtonReleased && pManager->IsPressed(this))
		{
			MouseReleased(event.mouseButton, pManager);
			pManager->SetPressed(NULL, event.mouseButton.button);
		}

		if (event.type == event.MouseMoved)
		{
			if (IsPointInside(event.mouseMove.x, event.mouseMove.y))
			{
				bFlag = SpreadEvent(event, pManager);
				if (bFlag) return bFlag;

				if (pManager->IsHovered(this))
				{
					MouseMoved(event.mouseMove, pManager);
				}
				else //Mouse Entry/Leave Detection
				{
					MouseEntered(event.mouseMove, pManager);
					if (!pManager->IsHovered(NULL))
					{
						pManager->GetHovered()->MouseLeft(event.mouseMove, pManager);
					}
					pManager->SetHovered(this);
				}
				bFlag = true;
			}
			else if (pManager->IsHovered(this))
			{
				MouseLeft(event.mouseMove, pManager);
				pManager->SetHovered(NULL);
			}

			//Drag Detection - movement
			if (IsDragged())
			{
				vPos.x += event.mouseMove.x - vDrag.x;
				vPos.y += event.mouseMove.y - vDrag.y;
				Dragged();
				vDrag.x = event.mouseMove.x;
				vDrag.y = event.mouseMove.y;
			}
		}

		return bFlag;
	}

	///Protected Methods
	void Widget::draw(sf::RenderTarget &target, sf::RenderStates states) const
	{
		for (WidgetList::const_iterator iter = lstChildren.cbegin(); iter != lstChildren.cend(); iter++)
		{
			Widget* pChild = *iter;
			if (pChild->IsVisible())
			{
				target.draw(*pChild, states);
			}
		}
	}

	void Widget::Dragged()
	{
		UpdateLocation();
	}

	bool Widget::SpreadEvent(sf::Event event, Manager* pManager)
	{
		bool bFlag = false;
		for (WidgetList::reverse_iterator iter = lstChildren.rbegin(); iter != lstChildren.rend(); iter++)//Reverse Order because last one has highest preference since its drawn last
		{
			Widget* pChild = *iter;
			bFlag = pChild->ParseEvent(event, pManager);
			if (bFlag) { break; }
		}
		return bFlag;
	}

	const sf::VertexArray& Widget::GetBorderVA() const
	{
		return vaBorder;
	}

	const sf::VertexArray& Widget::GetBackGroundVA() const
	{
		return vaBack;
	}

	const sf::VertexArray& Widget::GetTextVA() const
	{
		return vaText;
	}

	void Widget::UpdateTextVA(uint32_t dwFontIndex, const sf::String &text, uint8_t uStyle, uint32_t dwCharSize, uint32_t dwTextWidth, uint32_t dwNumLines, uint32_t dwStartIndex, const sf::Vector2f& vTextPos, char cPass) const
	{
		//Sanity Check
		vaText.clear();
		if (text.isEmpty() || pColor[FOREGROUND].a == 0) { return; }

		//Prepare Requisites - Font & other things
		bool bIsBold = (uStyle & BOLD) != 0;
		float fItalic = (uStyle & ITALIC) ? 0.208f : 0.f;//12 degree tilt
		float fSpace = static_cast<float>(GetFont(dwFontIndex).getGlyph(L' ', dwCharSize, bIsBold).advance);//For skipping blankspaces

		//Boundaries for x. For testing y, we will use the dwNumLines itself
		float xMin = vTextPos.x;
		if (xMin < XLEFT)
		{
			xMin = XLEFT;
		}
		if (xMin < dwCornerRadius + dwBorderWidth)
		{
			xMin = dwCornerRadius + dwBorderWidth;
		}

		float xMax = vTextPos.x + dwTextWidth;
		if (xMax > vSize.x - XLEFT)
		{
			xMax = vSize.x - XLEFT;
		}
		if (xMax > vSize.x - dwCornerRadius - dwBorderWidth)
		{
			xMax = vSize.x - dwCornerRadius - dwBorderWidth;
		}
		dwNumLines--;

		//Setup Initial Position
		float x = xMin;
		float y = vTextPos.y;//(vSize.y + dwCharSize)/2.0;//Center Alignment
		float yDelta = GetFont(dwFontIndex).getLineSpacing(dwCharSize);

		//Create One quad per character
		sf::Uint32 cPrev = 0;
		sf::Color color = pColor[FOREGROUND];
		for (std::size_t i = dwStartIndex; i < text.getSize(); i++)
		{
			sf::Uint32 cNow = text[i];
			if (cNow == '\r')
			{
				if (dwNumLines == 0) { break; }
				dwNumLines--;
				x = xMin;
				y += yDelta;
				continue;
			}
			else if (cNow == '\n')
			{
				if (cPrev != '\r')
				{
					if (dwNumLines == 0) { break; }
					dwNumLines--;
					x = xMin;
					y += yDelta;
				}
				cPrev = 0;
				continue;
			}
			else if (cPass != '\0')
			{
				cNow = cPass;
			}

			//Kerning Offset & Char Pixel Width
			float fKerning = static_cast<float>(GetFont(dwFontIndex).getKerning(cPrev, cNow, dwCharSize));
			cPrev = cNow;

			//Skip BlankSpace (stop if its appearing at end of the whole thing)
			if (cNow == ' ')
			{
				//Check if overflowing - if yes skip to next line (wrap)
				if (x+fKerning+fSpace >= xMax)
				{
					if (dwNumLines == 0) { break; }
					dwNumLines--;
					cPrev = 0;
					i--;
					x = xMin;
					y += yDelta;
				}
				else
				{
					x += fKerning+fSpace;
					continue;
				}
			}

			//For other characters - get the Glyph details
			const sf::Glyph& glyph = GetFont(dwFontIndex).getGlyph(cNow, dwCharSize, bIsBold);

			//Check if overflowing - if yes skip to next line (wrap)
			if (x+fKerning+glyph.advance >= xMax)
			{
				if (dwNumLines == 0) { break; }
				dwNumLines--;
				cPrev = 0;
				i--;
				x = xMin;
				y += yDelta;
				break;
			}

			//Get the Glyph Boundary
			float x1	= glyph.bounds.left;
			float y1	= glyph.bounds.top;
			float x2	= glyph.bounds.left + glyph.bounds.width;
			float y2	= glyph.bounds.top  + glyph.bounds.height;

			//Get the Glyph Texture Boundary
			float u1 = static_cast<float>(glyph.textureRect.left);
			float v1 = static_cast<float>(glyph.textureRect.top);
			float u2 = static_cast<float>(glyph.textureRect.left + glyph.textureRect.width);
			float v2 = static_cast<float>(glyph.textureRect.top  + glyph.textureRect.height);

			// Add a quad for the current character
			x += fKerning;

			vaText.append(sf::Vertex(sf::Vector2f(vPosReal.x + x + x1 - fItalic * y1, vPosReal.y + y + y1), color, sf::Vector2f(u1, v1)));
			vaText.append(sf::Vertex(sf::Vector2f(vPosReal.x + x + x2 - fItalic * y1, vPosReal.y + y + y1), color, sf::Vector2f(u2, v1)));
			vaText.append(sf::Vertex(sf::Vector2f(vPosReal.x + x + x1 - fItalic * y2, vPosReal.y + y + y2), color, sf::Vector2f(u1, v2)));
			vaText.append(sf::Vertex(sf::Vector2f(vPosReal.x + x + x1 - fItalic * y2, vPosReal.y + y + y2), color, sf::Vector2f(u1, v2)));
			vaText.append(sf::Vertex(sf::Vector2f(vPosReal.x + x + x2 - fItalic * y1, vPosReal.y + y + y1), color, sf::Vector2f(u2, v1)));
			vaText.append(sf::Vertex(sf::Vector2f(vPosReal.x + x + x2 - fItalic * y2, vPosReal.y + y + y2), color, sf::Vector2f(u2, v2)));

			x += glyph.advance;
		}
	}

	///Private Methods
	const bool Widget::isFlagSet(WFlag mask) const
	{
		return ((wFlag & mask) != 0);
	}

	void Widget::setFlag(WFlag mask, bool bStatus)
	{
		if (bStatus)
		{
			wFlag = wFlag | mask;
		}
		else
		{
			wFlag = wFlag & ~mask;
		}
	}

	void Widget::updateVertices()
	{
		//Border
		vaBorder.clear();
		if (dwBorderWidth != 0)
		{
			if (dwCornerRadius == 0)
			{
				genUnRounded(OUTLINE, vaBorder);
			}
			else
			{
				genRounded(OUTLINE, vaBorder);
			}
		}

		//Background
		vaBack.clear();
		if ((!IsTextured() && pColor[BACKGROUND].a == 0) || (vSize.x == 0 && vSize.y == 0))
		{
			return;
		}

		if (dwCornerRadius == 0)
		{
			genUnRounded(BACKGROUND, vaBack);
		}
		else
		{
			genRounded(BACKGROUND, vaBack);
		}
	}

	void Widget::genRounded(ColorID cid, sf::VertexArray &vertArray)
	{
		//Sanity Check
		if (cid >= MAX_ID) { return; }

		//Setup Multiplier - 1 for Background, 2 for Border/Outline
		uint32_t dwMult = 1;
		if (cid == OUTLINE) { dwMult++; }

		//Some constants
		static const uint32_t dwSmoothness = 20;
		static const float fDeltaAngle = 90.0f/(dwSmoothness-1);
		static const float fDeg2Rad = 3.141592654f / 180.0f;

		//setup the variables for marking center of corner circle
		float fCenterX, fCenterY;

		//Setup the vertex Array
		uint32_t dwMax = 4 * dwMult * dwSmoothness;//Max number of vertices - additional number is to make a complete polygon
		vertArray.resize(dwMax + dwMult);

		//Color to fill with - Use white for texture
		sf::Color color;
		if (IsTextured())
		{
			color = sf::Color::White;
		}
		else
		{
			color = GetColor(cid);
		}

		//Fill the Array with the points & color
		for (uint32_t i = 0; i < dwMax; i++)
		{
			uint32_t dwIndex = i/dwMult;
			uint32_t dwCenterIndex = dwIndex/dwSmoothness;
			switch (dwCenterIndex)
			{
				case 0: {fCenterX = vSize.x - dwCornerRadius; fCenterY = dwCornerRadius - vSize.y; break;}
				case 1: {fCenterX = dwCornerRadius;		   fCenterY = dwCornerRadius - vSize.y; break;}
				case 2: {fCenterX = dwCornerRadius;		   fCenterY = 0 - dwCornerRadius;	   break;}
				case 3: {fCenterX = vSize.x - dwCornerRadius; fCenterY = 0 - dwCornerRadius;	   break;}
			}

			if (cid == OUTLINE)
			{
				vertArray[i] = 	sf::Vertex(
													sf::Vector2f(
														vPosReal.x + dwCornerRadius * cos(fDeltaAngle * (dwIndex-dwCenterIndex) * fDeg2Rad) + fCenterX,
														vPosReal.y + dwCornerRadius * sin(fDeltaAngle * (dwIndex-dwCenterIndex) * fDeg2Rad) - fCenterY
													),
													color
												);
				i++;
				vertArray[i] =	sf::Vertex(
													sf::Vector2f(
														vPosReal.x + (dwCornerRadius-dwBorderWidth) * cos(fDeltaAngle * (dwIndex-dwCenterIndex) * fDeg2Rad) + fCenterX,
														vPosReal.y + (dwCornerRadius-dwBorderWidth) * sin(fDeltaAngle * (dwIndex-dwCenterIndex) * fDeg2Rad) - fCenterY
													),
													color
												);
			}
			else
			{
				vertArray[i] =	sf::Vertex(
													sf::Vector2f(
														vPosReal.x + (dwCornerRadius-dwBorderWidth) * cos(fDeltaAngle * (dwIndex-dwCenterIndex) * fDeg2Rad) + fCenterX,
														vPosReal.y + (dwCornerRadius-dwBorderWidth) * sin(fDeltaAngle * (dwIndex-dwCenterIndex) * fDeg2Rad) - fCenterY
													),
													color
												);
			}
		}

		//Complete the Polygon
		vertArray[dwMax] = vertArray[0];
		if (cid == OUTLINE)
		{
			vertArray[dwMax+1] = vertArray[1];
		}

		//If Textured Widget - Setup the Texture coordinates
		if (IsTextured())
		{
			for (uint32_t i = 0; i < (dwMax + dwMult); i++)
			{
				vertArray[i].texCoords = vertArray[i].position - vPosReal;
			}
		}
	}

	void Widget::genUnRounded(ColorID cid, sf::VertexArray &vertArray)
	{
		//Sanity Check
		if (cid >= MAX_ID) { return; }

		//Setup Multiplier - 1 for Background, 2 for Border/Outline
		uint32_t dwMult = 1;
		if (cid == OUTLINE) { dwMult++; }

		//Setup the Vertex Array
		vertArray.resize(5 * dwMult);

		//Color to fill with - Use white for texture
		sf::Color color;
		if (IsTextured())
		{
			color = sf::Color::White;
		}
		else
		{
			color = GetColor(cid);
		}

		//Inner Boundary - Boundary excluding Border
		sf::FloatRect inner = sf::FloatRect(dwBorderWidth, dwBorderWidth, vSize.x - 2*dwBorderWidth, vSize.y - 2*dwBorderWidth);

		//Fill the Array with points and color
		if (cid == BACKGROUND)
		{
			vertArray[0] = sf::Vertex(sf::Vector2f(vPosReal.x + inner.left			  , vPosReal.y + inner.top)			   , color);
			vertArray[1] = sf::Vertex(sf::Vector2f(vPosReal.x + inner.left + inner.width, vPosReal.y + inner.top)			   , color);
			vertArray[2] = sf::Vertex(sf::Vector2f(vPosReal.x + inner.left + inner.width, vPosReal.y + inner.top + inner.height), color);
			vertArray[3] = sf::Vertex(sf::Vector2f(vPosReal.x + inner.left			  , vPosReal.y + inner.top + inner.height), color);
			vertArray[4] = vertArray[0];
		}
		else
		{
			sf::FloatRect outer = sf::FloatRect(0, 0, vSize.x, vSize.y);
			vertArray[0] = sf::Vertex(sf::Vector2f(vPosReal.x + outer.left			  , vPosReal.y + outer.top)			   , color);
			vertArray[1] = sf::Vertex(sf::Vector2f(vPosReal.x + inner.left			  , vPosReal.y + inner.top)			   , color);
			vertArray[2] = sf::Vertex(sf::Vector2f(vPosReal.x + outer.left + outer.width, vPosReal.y + outer.top)			   , color);
			vertArray[3] = sf::Vertex(sf::Vector2f(vPosReal.x + inner.left + inner.width, vPosReal.y + inner.top)			   , color);
			vertArray[4] = sf::Vertex(sf::Vector2f(vPosReal.x + outer.left + outer.width, vPosReal.y + outer.top + outer.height), color);
			vertArray[5] = sf::Vertex(sf::Vector2f(vPosReal.x + inner.left + inner.width, vPosReal.y + inner.top + inner.height), color);
			vertArray[6] = sf::Vertex(sf::Vector2f(vPosReal.x + outer.left			  , vPosReal.y + outer.top + outer.height), color);
			vertArray[7] = sf::Vertex(sf::Vector2f(vPosReal.x + inner.left			  , vPosReal.y + inner.top + inner.height), color);
			vertArray[8] = vertArray[0];
			vertArray[9] = vertArray[1];
		}

		//If Textured Widget - Setup the Texture coordinates
		if (IsTextured())
		{
			for (uint32_t i = 0; i < (5 * dwMult); i++)
			{
				vertArray[i].texCoords = vertArray[i].position - vPosReal;
			}
		}
	}

	Manager* Widget::getManager() const
	{
		if (pWinParent == NULL)
		{
			return (UI::Manager*)(this);
		}
		else
		{
			return pWinParent->getManager();
		}
	}
}
