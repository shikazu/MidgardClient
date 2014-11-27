#include "PlayerFrame.h"
#include "../views/CharView.h"
#include <iostream>

namespace UI
{
	PlayerFrame* PlayerFrame::pSelected = NULL;
	PlayerFrame::PlayerFrame(uint32_t dwIdent, sf::Vector2i vPos, Player* player):Frame(dwIdent, vPos)
	{
		this->player = player;
	}

	PlayerFrame::PlayerFrame(uint32_t dwIdent, int32_t x, int32_t y, Player* player):Frame(dwIdent, x, y)
	{
		this->player = player;
	}

	PlayerFrame::~PlayerFrame()
	{
		if (pSelected != this) { DeletePlayer(); }
	}

	void PlayerFrame::Select()
	{
		pSelected = this;
	}

	void PlayerFrame::SetPlayer(Player* player)
	{
		this->player = player;
	}

	void PlayerFrame::DeletePlayer()
	{
		if (player != NULL)
		{
			delete player;
			player = NULL;
		}
	}

	PlayerFrame* PlayerFrame::GetSelected()
	{
		return pSelected;
	}

  void PlayerFrame::draw(sf::RenderTarget& target, sf::RenderStates states) const
  {
  	if (player != NULL)
		{
			sf::Transform trPlayer = GetTransform();
			trPlayer.translate(68, 118);
			states.transform = trPlayer;
			target.draw(*player, states);
		}
		if (pSelected == this)
		{
			Frame::draw(target, states);//Draw the Texture - The Select image
		}
  }

  void PlayerFrame::MouseClicked(sf::Event::MouseButtonEvent btnEvent, Manager *pManager)
  {
  	if (btnEvent.button == sf::Mouse::Left)
		{
			pSelected = this;
		}
  }

  void PlayerFrame::MouseEntered(sf::Event::MouseMoveEvent movEvent, Manager *pManager)
  {
		GetMouseCursor().SetState(CRS_BUTTON);
  }

  void PlayerFrame::MouseLeft(sf::Event::MouseMoveEvent movEvent, Manager *pManager)
  {
		GetMouseCursor().SetState(CRS_DEFAULT);
  }
}
