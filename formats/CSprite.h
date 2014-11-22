#ifndef _CSPRITE_H
#define _CSPRITE_H
#include <vector>
#include "../common/FileStream.h"
#include <SFML/Graphics.hpp>

class CSprite///Handler for SPR files
{
	public:
		CSprite(sf::String sFile, sf::Color* pPalette = NULL);
		CSprite(FileStream &stream, sf::Color* pPalette = NULL);
		virtual ~CSprite();

		const sf::Texture* GetTexture(uint16_t wIndex) const;
		const uint16_t GetTextureCount() const;
		const bool IsValid() const;

	private:
		bool bValid;
		uint16_t wVersion;
		std::vector<sf::Texture*> vTextures;

		void construct(FileStream &flstream, sf::Color* pPalette);
		void fetchBmp(FileStream &flstream, sf::Image* pImage, sf::Color* pPalette);
		void fetchTga(FileStream &flstream, sf::Image* pImage);
};
#endif//_CSPRITE_H
