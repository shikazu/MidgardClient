#ifndef _PLAYERFRAME_H
#define _PLAYERFRAME_H

#include "Frame.h"
#include "../render/Player.h"

namespace UI
{
	class PlayerFrame : public Frame//Used for displaying Player Sprites.
	{
		public:
			PlayerFrame(uint32_t dwIdent, sf::Vector2i vPos = sf::Vector2i(0,0), Player* player = NULL);
			PlayerFrame(uint32_t dwIdent, int32_t x = 0, int32_t y = 0, Player* player = NULL);
			virtual ~PlayerFrame();

			void Select();
			void SetPlayer(Player* player);
			void DeletePlayer();
			static PlayerFrame* GetSelected();

		protected:
			virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
			virtual void MouseClicked(sf::Event::MouseButtonEvent btnEvent, Manager *pManager);
			virtual void MouseEntered(sf::Event::MouseMoveEvent movEvent, Manager *pManager);
			virtual void MouseLeft(sf::Event::MouseMoveEvent movEvent, Manager *pManager);

		private:
			Player* player;
			static PlayerFrame* pSelected;

	};
}
#endif // _PLAYERFRAME_H
