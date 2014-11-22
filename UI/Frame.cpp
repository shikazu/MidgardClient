#include "Frame.h"

namespace UI
{
	Frame::Frame(uint32_t dwIdent, int32_t x, int32_t y, uint32_t w, uint32_t h):Widget(dwIdent, ENABLED|VISIBLE|DRAGGABLE|CLICKABLE, x, y, w, h)
	{
		pTexture = NULL;
	}
	Frame::Frame(uint32_t dwIdent, sf::Vector2i vPos, sf::Vector2u vSize):Widget(dwIdent, ENABLED|VISIBLE|DRAGGABLE|CLICKABLE, vPos, vSize)
	{
		pTexture = NULL;
	}
	Frame::~Frame()
	{
		if (pTexture != NULL)
		{
			delete pTexture;
		}
		DelChildren();
	}
	void Frame::SetTexture(std::string sFile)
	{
		if (pTexture != NULL)
		{
			delete pTexture;//Delete old data if any
		}
		pTexture = new sf::Texture();
		if (!GetPipe().getTexture(sFile, pTexture))
		{
			delete pTexture;
			SetTextured(false);
			return;
		}

		SetTextured(true);
		//Adjust the Width and Height
		sf::Vector2u vImageSize = pTexture->getSize();
		sf::Vector2u vCurSize = GetSize();
		if (vCurSize.x < vImageSize.x)
		{
			vCurSize.x = vImageSize.x;
		}
		if (vCurSize.y < vImageSize.y)
		{
			vCurSize.y = vImageSize.y;
		}
		if (vCurSize != GetSize())
		{
			Resize(vCurSize, false);
		}
	}

	void Frame::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		//sf::Transform t;
		//t.translate(GetPosition());
		//states.transform = t;
		states.transform = GetTransform();

		//Setup Texture and blendmode if defined
		if (pTexture != NULL)
		{
			states.texture = pTexture;
			states.blendMode = sf::BlendMode::BlendAlpha;
		}

		//draw the pre-calculated vertices
		target.draw(GetBackGroundVA(), states);

		//draw the children - default behavior
		Widget::draw(target, states);
	}
}
