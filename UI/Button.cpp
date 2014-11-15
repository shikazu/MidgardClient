#include "Button.h"

namespace UI
{
	Button::Button(uint32_t dwIdent, sf::Vector2f vPos):Widget(dwIdent, ENABLED|VISIBLE|CLICKABLE|TEXTURED, vPos)
	{
		uCurrent = INACTIVE;
		pCallback = NULL;
		pTextures[INACTIVE] = pTextures[ACTIVE] = pTextures[PRESSED] = NULL;
	}

	Button::Button(uint32_t dwIdent, float x, float y):Widget(dwIdent, ENABLED|VISIBLE|CLICKABLE|TEXTURED, x, y)
	{
		uCurrent = INACTIVE;
		pCallback = NULL;
		pTextures[INACTIVE] = pTextures[ACTIVE] = pTextures[PRESSED] = NULL;
	}
	void Button::SetCurrentState(State uState)
	{
		if (uState >= INVALID) return;
		uCurrent = uState;
	}
	void Button::SetTexture(std::string sFile, State uState)
	{
		if (uState >= INVALID) return;
		if (pTextures[uState] != NULL)
		{
			delete pTextures[uState];//Delete old data if any
		}
		//Allocate the Texture and Get from Global pipeline (i.e. DataPipe)
		pTextures[uState] = new sf::Texture();
		if (!DataPipe->getTexture(sFile, pTextures[uState]))
		{
			delete pTextures[uState];
			return;
		}

		//Adjust the Width and Height
		sf::Vector2u vImageSize = pTextures[uState]->getSize();
		sf::Vector2f vCurSize = GetSize();
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

	void Button::SetCallback(CALLBACK pFunc)
	{
		pCallback = pFunc;
	}

	void Button::InvokeCallback(Manager *pManager)
	{
		if (pCallback != NULL)
		{
			pCallback(this, pManager);
		}
	}

	void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		//Sanity Check
		if (pTextures[uCurrent] == NULL) { return; }

		//Put the texture , set the blend mode
		states.texture = pTextures[uCurrent];
		states.blendMode = sf::BlendMode::BlendAlpha;

		//draw the pre-calculated vertices
		target.draw(GetBackGroundVA(), states);
	}

	void Button::MousePressed(sf::Event::MouseButtonEvent btnEvent, Manager* pManager)
	{
		if (btnEvent.button == sf::Mouse::Left) { uCurrent = PRESSED;}
	}

	void Button::MouseReleased(sf::Event::MouseButtonEvent btnEvent, Manager* pManager)
	{
		if (btnEvent.button != sf::Mouse::Left) { return; }
		if (IsPointInside(btnEvent.x, btnEvent.y))
		{
			uCurrent = ACTIVE;
		}
		else
		{
			uCurrent = INACTIVE;
		}
	}

	void Button::MouseClicked(sf::Event::MouseButtonEvent btnEvent, Manager* pManager)
	{
		if (btnEvent.button == sf::Mouse::Left) { InvokeCallback(pManager); }
	}

	void Button::MouseEntered(sf::Event::MouseMoveEvent movEvent, Manager* pManager)
	{
		if (!pManager->IsPressed(NULL)) return; //If its pressing anything else ignore
		uCurrent = ACTIVE;
	}

	void Button::MouseLeft(sf::Event::MouseMoveEvent movEvent, Manager* pManager)
	{
		if (!pManager->IsPressed(NULL)) return; //If its pressing anything else ignore
		uCurrent = INACTIVE;
	}
}
