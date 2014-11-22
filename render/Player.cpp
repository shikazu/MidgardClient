#include "Player.h"
#include "../common/Globals.h"

Player::Player(CHARACTER_INFO_NEO_UNION* pCharacterInfo)
{
	//Test Code
	//const char* jprefix = "´ÑÀÚ";//Ninja
	const char* jprefix = "ref_body";
	uint32_t dwHeadNum = 2;
	char a[60];
	CSprite *pSprite;
	CActor *pActor;

	sprintf(a, "%s\\%s\\%s\\%s_%s.spr", S::PLAYER, S::BODY, S::MAN, jprefix, S::MAN);
	pSprite = new CSprite(a);
	sprintf(a, "%s\\%s\\%s\\%s_%s.act", S::PLAYER, S::BODY, S::MAN, jprefix, S::MAN);
	pActor = new CActor(a);
  layers[PC::BODY] = new SpriteRenderer(pSprite, pActor);

	sprintf(a, "%s\\%s\\%s\\%d_%s.spr", S::PLAYER, S::HEAD, S::MAN, dwHeadNum, S::MAN);
	pSprite = new CSprite(a);
	sprintf(a, "%s\\%s\\%s\\%d_%s.act", S::PLAYER, S::HEAD, S::MAN, dwHeadNum, S::MAN);
	pActor = new CActor(a);
	layers[PC::HEAD] = new SpriteRenderer(pSprite, pActor);
}

Player::~Player()
{
	delete layers[PC::BODY];
	delete layers[PC::HEAD];
}

void Player::SetPosition(sf::Vector2i vPos)
{
	this->vPos = vPos;
}

void Player::SetPosition(int32_t x, int32_t y)
{
	vPos.x = x;
	vPos.y = y;
}

const sf::Vector2i& Player::GetPosition() const
{
	return vPos;
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	sf::Transform trform(	1, 0, vPos.x,
												0, 1, vPos.y,
												0, 0, 1
											);//Equivalent of translate
	//trform.translate(vPos.x, vPos.y);
	states.transform *= trform;
	target.draw(*layers[PC::BODY], states);
	target.draw(*layers[PC::HEAD], states);
}
