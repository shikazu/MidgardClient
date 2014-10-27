#ifndef _FORMATS_GSPRITE_H
#define _FORMATS_GSPRITE_H
#include <SFML/Graphics.hpp>
#include <vector>
#include <fstream>

class GSprite//Handler for SPR files
{
    public:
        GSprite(const char* sFile, sf::Color* pPalette = 0);
        virtual ~GSprite();
        bool CheckValid();
        sf::Image* GetImage(uint16_t wIndex);
        uint16_t GetImageCount();

    private:
        bool bIs8Bit;
        bool bValid;
        uint16_t wVersion;
        std::vector<sf::Image*> vImages;

        void fetchBmp(std::fstream* pFile, sf::Image* pImage, sf::Color* pPalette);
        void fetchTga(std::fstream* pFile, sf::Image* pImage);
};
#endif//_FORMATS_GSPRITE_H
