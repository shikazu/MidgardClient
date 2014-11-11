#ifndef _FRAME_H
#define _FRAME_H

#include "Widget.h"
#include "../common/FileStream.h"

namespace UI
{
    class Frame : public Widget//Basically a Widget containing child widgets and optionally a BG
    {
        public:
            Frame(uint32_t dwIdent, sf::Vector2f vPos = sf::Vector2f(0,0), sf::Vector2f vSize = sf::Vector2f(20,20));
            Frame(uint32_t dwIdent, float x = 0, float y = 0, float w = 20,  float h = 20);//Size is set by the texture
            virtual ~Frame();

            void SetTexture(sf::Texture &tex);
            void SetTexture(FileStream &stream);

        protected:
            virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

        private:
            sf::Texture* pTexture;
    };
}
#endif//_FRAME_H
