#include "GameCursor.h"
#include "Globals.h"

GameCursor::GameCursor()
{
	pSprite = new CSprite("data\\cursors.spr");
	pActor = new CActor("data\\cursors.act");
	render = new SpriteRenderer(pSprite, pActor);
	render->SetAction(CRS_DEFAULT);
	bVisible = true;
	vPos = sf::Mouse::getPosition();
}

GameCursor::~GameCursor()
{
	delete render;
	delete pActor;
	delete pSprite;
}

void GameCursor::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (bVisible)
	{
		//sf::Vector2i vPos = sf::Mouse::getPosition();
		sf::Transform trform(	1, 0, vPos.x,
													0, 1, vPos.y,
													0, 0, 1
												);//Equivalent of translate
		//trform.translate(vPos.x, vPos.y);
		states.transform *= trform;
		target.draw(*render, states);
	}
}

void GameCursor::CheckMouse(sf::Event event)
{
	if (event.type == event.MouseLeft)
	{
		bVisible = false;
	}
	if (event.type == event.MouseEntered)
	{
		bVisible = true;
	}
	if (event.type == event.MouseMoved)
	{
		vPos.x = event.mouseMove.x;
		vPos.y = event.mouseMove.y;
	}
	if (event.type == event.MouseButtonPressed)
	{
		if (render->GetActIndex() == CRS_BUTTON)
		{
			render->SetAction(CRS_BUTTON, 1);
		}
		vPos.x = event.mouseButton.x;
		vPos.y = event.mouseButton.y;
	}
	if (event.type == event.MouseButtonReleased)
	{
		if (render->GetActIndex() == CRS_BUTTON)
		{
			render->SetAction(CRS_BUTTON, 0);
		}
		vPos.x = event.mouseButton.x;
		vPos.y = event.mouseButton.y;
	}
}

void GameCursor::SetState(CursorState cState)
{
	render->SetAction(cState);
}
