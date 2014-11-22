#include "TextBox.h"
namespace UI
{
	TextBox::TextBox(uint32_t dwIdent, uint8_t uStyle, sf::Vector2i vPos, sf::Vector2u vSize):Widget(dwIdent, ENABLED|VISIBLE|FOCUSABLE|EDITABLE, vPos, vSize)
	{
		sText = "";
		cPass = 0;
		dwCursorC = 0;
		bUpdateNeeded = true;
		dwFontID = 0;
		dwStart = 0;
		this->uStyle = uStyle;
		updateCursor();
	}

	TextBox::TextBox(uint32_t dwIdent, uint8_t uStyle, int32_t x, int32_t y, uint32_t w, uint32_t h):Widget(dwIdent, ENABLED|VISIBLE|FOCUSABLE|EDITABLE, x, y, w, h)
	{
		sText = "";
		cPass = 0;
		dwCursorC = 0;
		bUpdateNeeded = true;
		dwFontID = 0;
		dwStart = 0;
		this->uStyle = uStyle;
		updateCursor();
	}

	void TextBox::SetFontID(uint32_t dwFontID)
	{
		this->dwFontID = dwFontID;
		bUpdateNeeded = true;
		updateCursor();//can change since its a different font
	}

	void TextBox::SetPassChar(char c)
	{
		cPass = c;
		bUpdateNeeded = true;
		updateCursor();//can change due to character size changes
	}

	void TextBox::SetText(sf::String t)
	{
		sText = t;
		bUpdateNeeded = true;
		updateCursor();
	}

	void TextBox::SetCharSize(uint32_t dwSize)
	{
		dwCharSize = dwSize;
		bUpdateNeeded = true;
		updateCursor();
	}
	void TextBox::SetStyle(uint8_t uStyle)
	{
		this->uStyle = uStyle;
		bUpdateNeeded = true;
		updateCursor();
	}

	const uint32_t TextBox::GetFontID() const
	{
		return dwFontID;
	}

	const sf::String TextBox::GetText() const
	{
		return sText;
	}

	const uint32_t TextBox::GetCharSize() const
	{
		return dwCharSize;
	}

	const uint8_t TextBox::GetStyle() const
	{
		return uStyle;
	}

	void TextBox::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		//sf::Transform t;
		//t.translate(GetPosition());
		//states.transform = t;
		states.transform = GetTransform();

		//Draw the Background - unless its transparent
		if (GetColor(BACKGROUND).a != 0)
		{
			target.draw(GetBackGroundVA(), states);
		}

		//Update vertices if needed and Draw the text
		if (bUpdateNeeded)
		{
			sf::Vector2i vTextPos = sf::Vector2i(0, (GetHeight() + dwCharSize)/2);
			uint32_t dwXMax = GetWidth() - XLEFT;
			uint32_t dwNum = 1;
			UpdateTextVA(dwFontID, sText, uStyle, dwCharSize, dwXMax, dwNum, dwStart, vTextPos, cPass);
			bUpdateNeeded = false;
		}
		states.texture = &(GetFont(dwFontID).getTexture(dwCharSize));
		target.draw(GetTextVA(), states);

		//Draw the Cursor
		if (IsFocused() && IsEditable())
		{
			sf::VertexArray vaCursor(sf::Lines, 2);
			vaCursor[0] = sf::Vertex(sf::Vector2f(dwCursorX, GetBorderWidth()), GetColor(FOREGROUND));
			vaCursor[1] = sf::Vertex(sf::Vector2f(dwCursorX, GetHeight() - GetBorderWidth()), GetColor(FOREGROUND));
			target.draw(vaCursor, states);
		}

		//Finally Draw the Border
		if (GetBorderWidth() > 0)
		{
			target.draw(GetBorderVA(), states);
		}
	}

	void TextBox::MousePressed(sf::Event::MouseButtonEvent btnEvent, Manager* pManager)
	{
		updateCursor(btnEvent.x - GetX());
	}
	void TextBox::KeyPressed(sf::Event::KeyEvent keyEvent, Manager* pManager)
	{
		if (!IsEditable()) return;
		switch(keyEvent.code)
		{
			case sf::Keyboard::BackSpace:
			{
				if (dwCursorC > 0) dwCursorC--;
				updateCursor();
			}//no need to break it since it should delete next
			case sf::Keyboard::Delete:
			{
				sText.erase(dwCursorC);
				bUpdateNeeded = true;
				break;
			}
			case sf::Keyboard::Left:
			{
				if (dwCursorC > 0) dwCursorC--;
				updateCursor();
				break;
			}
			case sf::Keyboard::Right:
			{
				if (dwCursorC < sText.getSize()) dwCursorC++;
				updateCursor();
				break;
			}
			case sf::Keyboard::Home:
			{
				dwCursorC = 0;
				updateCursor();
				break;
			}
			case sf::Keyboard::End:
			{
				dwCursorC = sText.getSize();
				updateCursor();
				break;
			}
			default:
			{
			}
		}
	}

	void TextBox::TextEntered(sf::Event::TextEvent textEvent, Manager* pManager)
	{
		if (IsEditable() && textEvent.unicode >= 0x20)//Printable Characters
		{
			sText.insert(dwCursorC, textEvent.unicode);
			bUpdateNeeded = true;
			dwCursorC++;
			updateCursor();
		}
	}

	void TextBox::WinResized()
	{
		bUpdateNeeded = true;
		updateCursor();
	}

	void TextBox::Dragged()
	{
		UpdateLocation();
		bUpdateNeeded = true;
		updateCursor();
	}

	void TextBox::updateCursor(int32_t xSnap)
	{
		//Prepare Pre-requisites
		bool bIsBold = ((uStyle & BOLD) != 0);

		//Initial Position & Bounding value
		float x = GetCornerRadius() + GetBorderWidth();
		if (x < XLEFT) { x = XLEFT; }
		float xMax = static_cast<float>(GetWidth()) - x;

		//Change Cursor to maximum value in case mouse click was used
		if (xSnap >= 0)
		{
			dwCursorC = sText.getSize();
		}

		//Move Character By Character
		uint32_t i = dwStart;
		sf::Uint32 cPrev = 0;

		for ( ;i <= dwCursorC; i++)
		{
			sf::Uint32 cNow = sText[i];
			if (cPass != 0)
			{
				cNow = cPass;
			}

			float fKerning = static_cast<float>(GetFont(dwFontID).getKerning(cPrev, cNow, dwCharSize));
			float fAdvance = static_cast<float>(GetFont(dwFontID).getGlyph(cNow, dwCharSize, bIsBold).advance);

			if (x+fKerning+fAdvance >= xMax)
			{
				dwCursorC = i;
				break;
			}
			x += fKerning;
			if (x >= xMax)//An Additional check
			{
				dwCursorC = i;
				break;
			}
			if (xSnap >= 0 && x >= xSnap)//When mouse is clicked snap to that
			{
				dwCursorC = i;
				break;
			}
			if (i == dwCursorC) break;//For when the cursor is after the last character
			cPrev = cNow;

			if (xSnap >= 0 && (x + (fAdvance - fKerning)/2) >= xSnap)
			{
				dwCursorC = i;
				break;
			}
			x += fAdvance;
		}
		dwCursorX = x;
	}
}
