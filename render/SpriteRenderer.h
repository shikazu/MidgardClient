#ifndef _SPRITERENDERER_H
#define _SPRITERENDERER_H

#include <SFML/Graphics.hpp>
#include "../formats/CSprite.h"
#include "../formats/CActor.h"

class SpriteRenderer : public sf::Drawable
{
	public:
		//Constructor and Destructor
		SpriteRenderer(CSprite *pSprite, CActor *pActor);
		virtual ~SpriteRenderer();

		//Methods - Setters (Position, Action Index & Frame Index)
		void SetAction(uint16_t wAct, uint32_t dwFrame=0);//Frame num resets to zero
		void UpdateFrame(bool bForward = true);//Increment/Decrement Frame Count;

		//Methods - Getters (Position, Action Index, Frame Index, Frame Count)
		const uint16_t GetActIndex() const;
		const uint32_t GetFrameIndex() const;
		const uint32_t GetFrameCount() const;
		const float GetDelay() const;
		CSprite* GetSprite();
		CActor* GetActor();

	protected:
		virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	private:
		CSprite *sprite;
		CActor *actor;
		mutable std::vector<sf::Transform> vTransforms;
		uint16_t wAct;
		uint32_t dwFrame;
		mutable bool bUpdateGeometry;

		void updateTransforms() const;
};
#endif // _SPRITERENDERER_H
