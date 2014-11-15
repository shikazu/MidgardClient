#ifndef _FRAME_H
#define _FRAME_H
#include "Widget.h"

namespace UI
{
	class Frame : public Widget//Basically a Widget containing child widgets + a Texture - ofcourse more functionality may get added on later
	{
		public:
			Frame(uint32_t dwIdent, sf::Vector2f vPos = sf::Vector2f(0,0), sf::Vector2f vSize = sf::Vector2f(0,0));
			Frame(uint32_t dwIdent, float x = 0, float y = 0, float w = 20,  float h = 20);//Size is also reset by the texture if the current size doesnt fit the image
			virtual ~Frame();

			void SetTexture(std::string sFile);

		protected:
			virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

		private:
			sf::Texture* pTexture;
	};
}
#endif//_FRAME_H
