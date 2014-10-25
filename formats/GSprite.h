#ifndef _FORMATS_GSPRITE_H
#define _FORMATS_GSPRITE_H
#include <SFML/Graphics.hpp>
#include <vector>
#include <fstream>

class GSprite
{
public:
    GSprite(const char* filename);
    virtual ~GSprite();
    sf::Image* GetImage(uint16_t index);
    uint16_t GetImageCount();

private:
    bool is8bpp;
    uint16_t version;
    uint16_t imgCount;
    std::vector<sf::Image*> images;

    void fetchBmp(std::fstream* fp, sf::Image* image, sf::Color* palette);
    void fetchTga(std::fstream* fp, sf::Image* image);
};
#endif//_FORMATS_GSPRITE_H
