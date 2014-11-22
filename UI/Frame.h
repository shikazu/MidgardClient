#ifndef _FRAME_H
#define _FRAME_H
#include "Widget.h"

namespace UI
{
	class Frame : public Widget//Basically a Widget containing child widgets + a Texture - ofcourse more functionality may get added on later
	{
		public:
			Frame(uint32_t dwIdent, sf::Vector2i vPos = sf::Vector2i(0,0), sf::Vector2u vSize = sf::Vector2u(0,0));
			Frame(uint32_t dwIdent, int32_t x = 0, int32_t y = 0, uint32_t w = 20,  uint32_t h = 20);//Size is also reset by the texture if the current size doesnt fit the image
			virtual ~Frame();

			void SetTexture(std::string sFile);

		protected:
			virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

		private:
			sf::Texture* pTexture;
	};
}
#endif//_FRAME_H
