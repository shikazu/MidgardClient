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
			//sf::Transform trOriginal = states.transform;//backup
			sf::Transform trPlayer = GetTransform();
			trPlayer.translate(68, 118);
			/*)	1, 0, GetPosition(false).x + 68,
															0, 1, GetPosition(false).y + 118,
															0, 0 ,1
														);
			states.transform *= trPlayer;
			*/
			states.transform = trPlayer;
			target.draw(*player, states);
			//states.transform = trOriginal;//Restore the original
		}
		if (pSelected == this)
		{
			Frame::draw(target, states);//Draw the Texture - The Select image
		}
  }
}
