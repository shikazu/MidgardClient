#ifndef _GAMECURSOR_H
#define _GAMECURSOR_H

#include <SFML/System.hpp>
#include "../render/SpriteRenderer.h"

enum CursorState
{
	CRS_DEFAULT = 0,
	CRS_NPC,
	CRS_BUTTON,
	CRS_PC,
	CRS_CAMERA,
	CRS_BATTLE,//Mob
	CRS_BATTLE2,
	CRS_DOOR,
	CRS_INVALID
};

class GameCursor : public sf::Drawable
{
	public:
		GameCursor();
		virtual ~GameCursor();

		void CheckMouse(sf::Event event);//Used for detecting mouse movements & mouse clicks
		void SetState(CursorState cState);//Used by Objects to set the cursor State

	protected:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const;

	private:
		sf::Vector2i vPos;
		SpriteRenderer *render;
		CSprite* pSprite;
		CActor* pActor;
		bool bVisible;
};

#endif // _GAMECURSOR_H
