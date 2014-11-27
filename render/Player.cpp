#include <cstring>
#include <iostream>
#include "Player.h"
#include "../common/Globals.h"

Player::Player(CHARACTER_INFO_NEO_UNION* pCharacterInfo)
{
	char sPath[100], sPrefix[40];
	const char *sGender;
	CSprite *pSprite;
	CActor *pActor;

	int16_t shGender = 1;//now using local values
	if (shGender == 1)
	{
		sGender = S::MAN;
	}
	else
	{
		sGender = S::WOMAN;
	}

	//Head
	sprintf(sPath, "%s\\%s\\%s\\%d_%s.spr", S::PLAYER, S::HEAD, sGender, pCharacterInfo->shHead, sGender);
	pSprite = new CSprite(sPath);
	strcpy(sPath + strlen(sPath)-3, "act");
	pActor = new CActor(sPath);
	layers[PC::HEAD] = new SpriteRenderer(pSprite, pActor);

	//Body
	GetDB().GetJobSuffix(pCharacterInfo->shJob, sPrefix);
	sprintf(sPath, "%s\\%s\\%s\\%s_%s.spr", S::PLAYER, S::BODY, sGender, sPrefix, sGender);
	pSprite = new CSprite(sPath);
	strcpy(sPath + strlen(sPath)-3, "act");
	pActor = new CActor(sPath);
	layers[PC::BODY] = new SpriteRenderer(pSprite, pActor);

	//Headgear Upper
	GetDB().GetHeadgearSuffix(pCharacterInfo->shAccessory, sPrefix);
	sprintf(sPath, "%s\\%s\\%s_%s.spr", S::EQUIP, sGender, sGender, sPrefix);
	pSprite = new CSprite(sPath);
	strcpy(sPath + strlen(sPath)-3, "act");
	pActor = new CActor(sPath);
	layers[PC::HG_UPPER] = new SpriteRenderer(pSprite, pActor);

	//Headgear Middle
	GetDB().GetHeadgearSuffix(pCharacterInfo->shAccessory2, sPrefix);
	sprintf(sPath, "%s\\%s\\%s_%s.spr", S::EQUIP, sGender, sGender, sPrefix);
	pSprite = new CSprite(sPath);
	strcpy(sPath + strlen(sPath)-3, "act");
	pActor = new CActor(sPath);
	layers[PC::HG_MIDDLE] = new SpriteRenderer(pSprite, pActor);

	//Headgear Lower
	GetDB().GetHeadgearSuffix(pCharacterInfo->shAccessory3, sPrefix);
	sprintf(sPath, "%s\\%s\\%s_%s.spr", S::EQUIP, sGender, sGender, sPrefix);
	pSprite = new CSprite(sPath);
	strcpy(sPath + strlen(sPath)-3, "act");
	pActor = new CActor(sPath);
	layers[PC::HG_LOWER] = new SpriteRenderer(pSprite, pActor);

}

Player::~Player()
{
	delete layers[PC::BODY];
	delete layers[PC::HEAD];
	delete layers[PC::HG_LOWER] ;
	delete layers[PC::HG_MIDDLE];
	delete layers[PC::HG_UPPER] ;
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
	states.transform *= trform;
	target.draw(*layers[PC::BODY], states);
	target.draw(*layers[PC::HEAD], states);
	//target.draw(*layers[PC::HG_LOWER] , states);
	//target.draw(*layers[PC::HG_MIDDLE], states);
	//target.draw(*layers[PC::HG_UPPER] , states);
}
