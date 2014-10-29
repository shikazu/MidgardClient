#ifndef _CSPRITE_H
#define _CSPRITE_H
#include <SFML/Graphics.hpp>
#include <vector>
#include <fstream>

class CSprite///Handler for SPR files
{
    public:
        CSprite(const char* sFile, sf::Color* pPalette = 0);
        CSprite(std::istream &stream, sf::Color* pPalette = 0);
        virtual ~CSprite();

        sf::Texture* GetTexture(uint16_t wIndex);
        uint16_t GetTextureCount();
        bool IsValid();

    private:
        bool bValid;
        uint16_t wVersion;
        std::vector<sf::Texture*> vTextures;

        bool construct(std::istream &stream, sf::Color* pPalette = 0);
        void fetchBmp(std::istream &stream, sf::Image* pImage, sf::Color* pPalette);
        void fetchTga(std::istream &stream, sf::Image* pImage);
};
#endif//_CSPRITE_H
