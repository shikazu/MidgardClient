#ifndef _PLAYER_H
#define _PLAYER_H
#include "SpriteRenderer.h"
#include "../common/Structs.h"
#include <array>

namespace PC
{
	enum Layer
	{
		ROBE = 0,//Default Robe gets drawn first then everything else - should implement a draw on top feature later
		BODY,
		HEAD,
		HG_LOWER,
		HG_MIDDLE,
		HG_UPPER,
		WEAPON,
		SHIELD,
		COUNT,//Keep this last it specifies the count of sprite layers - Currently Head + Body + 3 Headgears + 1 Garment + 1 Weapon + 1Shield
	};

	enum ActDesc
	{
		IDLE = 0,
		WALK,
		SIT,
		PICKING,
		AFTER_HIT,
		ATTACK_MOVE,
		HIT,
		HIT2,//no animation
		DEAD,
		STAND,
		ATTACK_BARE,
		ATTACK_WEAPON,
		CASTING
	};
}

class Player : public sf::Drawable
{
	public:
		Player(CHARACTER_INFO_NEO_UNION* pCharacterInfo = NULL);
		virtual ~Player();
		void SetPosition(sf::Vector2i vPos);
		void SetPosition(int32_t x, int32_t y);
		const sf::Vector2i& GetPosition() const;

	protected:
		void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	private:
		std::array<SpriteRenderer*, PC::COUNT> layers;
		sf::Vector2i vPos;
};

#endif // _PLAYER_H
