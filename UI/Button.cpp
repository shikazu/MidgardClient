#include "Button.h"
#include "../common/Globals.h"

namespace UI
{
	Button::Button(uint32_t dwIdent, sf::Vector2i vPos):Widget(dwIdent, ENABLED|VISIBLE|CLICKABLE|TEXTURED, vPos)
	{
		uCurrent = INACTIVE;
		pCallback = NULL;
		pTextures[INACTIVE] = pTextures[ACTIVE] = pTextures[PRESSED] = NULL;
	}

	Button::Button(uint32_t dwIdent, int32_t x, int32_t y):Widget(dwIdent, ENABLED|VISIBLE|CLICKABLE|TEXTURED, x, y)
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
		if (!GetPipe().getTexture(sFile, pTextures[uState]))
		{
			delete pTextures[uState];
			return;
		}

		//Adjust the Width and Height
		sf::Vector2u vImageSize = pTextures[uState]->getSize();
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
		//sf::Transform t;
		//t.translate(GetPosition());
		//states.transform = t;
		states.transform = GetTransform();

		//Put the texture
		if (pTextures[uCurrent] == NULL)
		{
			if (uCurrent != INACTIVE && pTextures[INACTIVE] != NULL)
			{
				states.texture = pTextures[INACTIVE];
			}
			else
			{
				return;
			}
		}
		else
		{
			states.texture = pTextures[uCurrent];
		}

		//set the blend mode
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
			GetMouseCursor().SetState(CRS_DEFAULT);
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
		GetMouseCursor().SetState(CRS_BUTTON);
	}

	void Button::MouseLeft(sf::Event::MouseMoveEvent movEvent, Manager* pManager)
	{
		if (!pManager->IsPressed(NULL)) return; //If its pressing anything else ignore
		uCurrent = INACTIVE;
		GetMouseCursor().SetState(CRS_DEFAULT);
	}
}
