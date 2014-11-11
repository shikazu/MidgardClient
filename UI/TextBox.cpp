#include "TextBox.h"
namespace UI
{
    TextBox::TextBox(uint32_t dwIdent, sf::Vector2f vPos, sf::Vector2f vSize):Widget(dwIdent, ENABLED|VISIBLE|FOCUSABLE|EDITABLE, vPos, vSize)
    {
        sText = "";
        cPass = 0;
        dwCursorC = 0;
        dwCursorX = GetPosition().x + GetCornerRadius();
        vaChars = sf::VertexArray(sf::Triangles);
        bUpdateNeeded = true;
        pFont = NULL;
    }

    TextBox::TextBox(uint32_t dwIdent, float x, float y, float w, float h):Widget(dwIdent, ENABLED|VISIBLE|FOCUSABLE|EDITABLE, x, y, w, h)
    {
        sText = "";
        cPass = 0;
        dwCursorC = 0;
        dwCursorX = x + GetCornerRadius();
        vaChars = sf::VertexArray(sf::Triangles);
        bUpdateNeeded = true;
        pFont = NULL;
    }

    TextBox::~TextBox()
    {
    }

    void TextBox::SetFont(sf::Font& f)
    {
        pFont = &f;
        bUpdateNeeded = true;
        updateCursorLocation();//can change since its a different font
    }
    void TextBox::SetPassChar(char c)
    {
        cPass = c;
        bUpdateNeeded = true;
        updateCursorLocation();//can change due to character size changes
    }
    void TextBox::SetText(sf::String t)
    {
        sText = t;
        bUpdateNeeded = true;
    }
    const sf::String TextBox::GetText() const
    {
        return sText;
    }
    void TextBox::SetCharSize(uint32_t dwSize)
    {
        dwCharSize = dwSize;
        bUpdateNeeded = true;
    }
    const uint32_t TextBox::GetCharSize() const
    {
        return dwCharSize;
    }
    void TextBox::SetFocus(bool bStatus)
    {
        Widget::SetFocus(bStatus);
        updateCursorLocation();
    }

    void TextBox::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        DrawBackground(target,states);
        if (pFont != NULL)
        {
            if (bUpdateNeeded) {updateVertices();}
            states.texture = &(pFont->getTexture(dwCharSize));
            target.draw(vaChars, states);
        }
        if (HasFocus() && IsEditable())
        {
            sf::Vertex vaCursor[] =
            {
                sf::Vertex(sf::Vector2f(dwCursorX, GetPosition().y + GetBorderWidth()), GetColor(FOREGROUND)),
                sf::Vertex(sf::Vector2f(dwCursorX, GetPosition().y + GetHeight() - GetBorderWidth()), GetColor(FOREGROUND))
            };
            target.draw(vaCursor, 2, sf::Lines);
        }
        if (GetBorderWidth() > 0)
        {
            DrawBorder(target, states);
        }

    }

    void TextBox::MousePressed(sf::Event::MouseButtonEvent btnEvent, Manager* pManager)
    {
        updateCursorLocation(btnEvent.x);
    }
    void TextBox::KeyPressed(sf::Event::KeyEvent keyEvent, Manager* pManager)
    {
        if (!IsEditable()) return;
        switch(keyEvent.code)
        {
            case sf::Keyboard::BackSpace:
            {
                if (dwCursorC > 0) dwCursorC--;
                updateCursorLocation();
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
                updateCursorLocation();
                break;
            }
            case sf::Keyboard::Right:
            {
                if (dwCursorC < sText.getSize()) dwCursorC++;
                updateCursorLocation();
                break;
            }
            case sf::Keyboard::Home:
            {
                dwCursorC = 0;
                updateCursorLocation();
                break;
            }
            case sf::Keyboard::End:
            {
                dwCursorC = sText.getSize();
                updateCursorLocation();
                break;
            }
            default:{}

        }
    }
    void TextBox::TextEntered(sf::Event::TextEvent textEvent, Manager* pManager)
    {
        if (IsEditable() && textEvent.unicode >= 0x20)
        {
            sText.insert(dwCursorC, textEvent.unicode);
            bUpdateNeeded = true;
            dwCursorC++;
            updateCursorLocation();
        }
    }
    void TextBox::WinResized(Manager* pManager)
    {
        bUpdateNeeded = true;
        updateCursorLocation();
    }
    void TextBox::Dragged()
    {
        UpdateLocation();
        bUpdateNeeded = true;
        updateCursorLocation();
    }

    void TextBox::updateCursorLocation(int32_t xSnap)
    {
        if (pFont == NULL) return;
        bool bIsBold = ((uStyle & BOLD) != 0); //Flag needed in glyph function

        uint32_t dwAlign = GetCornerRadius() + GetBorderWidth();
        if (dwAlign < 3) {dwAlign = 3;}

        dwCursorX = GetPosition().x + dwAlign;//an offset
        uint32_t dwRight = dwCursorX + GetWidth() - 2*dwAlign;
        if (xSnap >= 0)//meaning its called from the mouse click
        {
            dwCursorC = sText.getSize();
        }
        uint32_t i = 0;
        sf::Uint32 cPrev = ' ';
        uint32_t dwCursorActual = 0;
        for ( ;i <= dwCursorC; i++)
        {
            sf::Uint32 cNow;
            if (cPass == '\0')
            {
                cNow = sText[i];
            }
            else
            {
                cNow = cPass;
            }
            uint32_t dwKerning = pFont->getKerning(cPrev, cNow, dwCharSize);
            dwCursorX += dwKerning;
            if (dwCursorX <= dwRight) { dwCursorActual = dwCursorX;}
            if (xSnap >= 0 && dwCursorX >= xSnap) break;
            if (i == dwCursorC) break;
            cPrev = cNow;

            uint32_t dwAdvance =  pFont->getGlyph(cNow, dwCharSize, bIsBold).advance;
            if (xSnap >= 0 && (dwCursorX + (dwAdvance-dwKerning)/2) >= xSnap) break;
            dwCursorX += dwAdvance;
        }
        dwCursorC = i;
        dwCursorX = dwCursorActual;
    }

    void TextBox::updateVertices() const
    {
        if (!bUpdateNeeded) return;//Only call this when absolutely needed
        bUpdateNeeded = false;
        vaChars.clear();//Clear the vertices
        if (pFont == NULL || sText.isEmpty()) return;//Check for valid font and non-empty string

        bool bIsBold = (uStyle & BOLD) != 0;
        float fItalic = (uStyle & ITALIC) ? 0.208f : 0.f;//12 degree tilt
        float fHspace = static_cast<float>(pFont->getGlyph(L' ', dwCharSize, bIsBold).advance);//For skipping blankspaces

        sf::FloatRect rect = GetBBox();//Get the Bounding Box;//Faster method
        float x = GetCornerRadius() + GetBorderWidth();
        if (x < 3) {x = 3;}
        float y = (rect.height + dwCharSize)/2.0;//centre aligned text
        float xMax = rect.width - x;

        //Create One quad per character
        sf::Uint32 cPrev = 0;
        sf::Color color = GetColor(FOREGROUND);
        std::size_t i = 0;
        for (; i < sText.getSize(); i++)
        {
            sf::Uint32 cNow;
            if (cPass == '\0')
            {
                cNow = sText[i];
            }
            else
            {
                cNow = cPass;
            }

            //Apply Kerning Offset
            x += static_cast<float>(pFont->getKerning(cPrev, cNow, dwCharSize));
            cPrev = cNow;

            if (cNow == ' ') { x += fHspace; continue;}//Skip BlankSpace

            //Now get the Glyph details
            const sf::Glyph& glyph = pFont->getGlyph(cNow, dwCharSize, bIsBold);
            if (x+glyph.advance >= xMax)
            {
                x -= static_cast<float>(pFont->getKerning(cPrev, cNow, dwCharSize));
                break;// Precaution for overflowing chars
            }

            float left   = glyph.bounds.left;
            float top    = glyph.bounds.top;
            float right  = glyph.bounds.left + glyph.bounds.width;
            float bottom = glyph.bounds.top  + glyph.bounds.height;

            float u1 = static_cast<float>(glyph.textureRect.left);
            float v1 = static_cast<float>(glyph.textureRect.top);
            float u2 = static_cast<float>(glyph.textureRect.left + glyph.textureRect.width);
            float v2 = static_cast<float>(glyph.textureRect.top  + glyph.textureRect.height);

            // Add a quad for the current character
            vaChars.append(sf::Vertex(sf::Vector2f(rect.left + x + left  - fItalic * top   , rect.top + y + top)   , color, sf::Vector2f(u1, v1)));
            vaChars.append(sf::Vertex(sf::Vector2f(rect.left + x + right - fItalic * top   , rect.top + y + top)   , color, sf::Vector2f(u2, v1)));
            vaChars.append(sf::Vertex(sf::Vector2f(rect.left + x + left  - fItalic * bottom, rect.top + y + bottom), color, sf::Vector2f(u1, v2)));
            vaChars.append(sf::Vertex(sf::Vector2f(rect.left + x + left  - fItalic * bottom, rect.top + y + bottom), color, sf::Vector2f(u1, v2)));
            vaChars.append(sf::Vertex(sf::Vector2f(rect.left + x + right - fItalic * top   , rect.top + y + top)   , color, sf::Vector2f(u2, v1)));
            vaChars.append(sf::Vertex(sf::Vector2f(rect.left + x + right - fItalic * bottom, rect.top + y + bottom), color, sf::Vector2f(u2, v2)));

            x+= glyph.advance;
        }
    }
}
