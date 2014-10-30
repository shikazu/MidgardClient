#include "CSprite.h"

CSprite::CSprite(const char* sFile, sf::Color *pPalette)
{
    std::fstream stream(sFile, std::fstream::in | std::fstream::binary);
    bValid = construct(stream, pPalette);
    stream.close();
}

CSprite::CSprite(std::istream &stream, sf::Color *pPalette)
{
    bValid = construct(stream, pPalette);
}

bool CSprite::construct(std::istream &stream, sf::Color *pPalette)
{
    uint16_t wSig;
    stream.read((char*)&wSig, 2);
    if (wSig != 0x5053)
    {
        return false;
    }

    uint16_t wImgCount;
    bool bIs8Bit;//Flag for 8BitPerPixel or not
    stream.read((char*)&wVersion, 2);//Get version
    stream.read((char*)&wImgCount, 2);//Get BMP count
    if (wImgCount > 0)
    {
        bIs8Bit = true;//Contains 8bpp images - bmp
        stream.seekg(2, stream.cur);//Skip TGA count - will be zero anyways
    }
    else
    {
        bIs8Bit = false;//Contains 32bpp images - tga
        stream.read((char*)&wImgCount, 2);//Get TGA count
    }

    if (bIs8Bit && pPalette == 0)
    {
        pPalette = (sf::Color*)malloc(256 * sizeof(sf::Color));
        //Get the Palette
        int iPos = stream.tellg();//Save Current Position
        stream.seekg(-1024, stream.end);//Skip to start of palette
        for (uint16_t i = 0; i < 256; i++)
        {
            uint8_t rgba[4];
            stream.read((char*)rgba, 4);
            pPalette[i].r = rgba[0];
            pPalette[i].g = rgba[1];
            pPalette[i].b = rgba[2];
            if (i == 0 || (rgba[0] == 0xFF && rgba[1] == 0x00 && rgba[2] == 0xFF))//index = 0 as well as magenta will be transparent
            {
                pPalette[i].a = 0;
            }
            else
            {
                pPalette[i].a = 255;
            }
        }
        stream.seekg(iPos, stream.beg);
    }

    //Now for the textures
    vTextures.reserve(wImgCount);
    for (uint16_t i = 0; i < wImgCount; i++)
    {
        uint16_t wWidth, wHeight;
        stream.read((char*)&wWidth , 2);
        stream.read((char*)&wHeight, 2);
        sf::Image image;
        image.create(wWidth, wHeight);//, sf::Color::Transparent);
        if (bIs8Bit)
        {
            fetchBmp(stream, &image, pPalette);
        }
        else
        {
            fetchTga(stream, &image);
        }
        sf::Texture *pTexture = new sf::Texture();
        pTexture->loadFromImage(image);
        //pTexture->setSmooth(true);
        vTextures.push_back(pTexture);
    }
    return true;
}

CSprite::~CSprite()
{
}

void CSprite::fetchBmp(std::istream &stream, sf::Image* pImage, sf::Color* pPalette)
{
    uint16_t wDataLen, x = 0, y = 0;
    uint16_t wWidth = pImage->getSize().x;
    stream.read((char*)&wDataLen, 2);
    for (uint16_t i = 0;i < wDataLen; i++)
    {
        uint8_t len = 1, index = stream.get();
        if (index == 0)//For index 0 RLE is followed
        {
            i++;
            len = stream.get();
        }
        for (uint8_t j = 0; j < len; j++)
        {
            sf::Color color = pPalette[index];
            pImage->setPixel(x, y, color);
            x++;
            if (x >= wWidth)
            {
                x = 0;
                y++;
            }
        }
    }
}

void CSprite::fetchTga(std::istream &stream, sf::Image* pImage)
{
    uint16_t wWidth = pImage->getSize().x, wHeight = pImage->getSize().y;
    uint16_t x = 0, y = 0;
    uint32_t dwDataLen =  wWidth * wHeight;
    uint8_t rgba[4];
    for (uint32_t i = 0; i < dwDataLen; i++)
    {
        stream.read((char*)rgba, 4);
        if (rgba[0] == 0xFF && rgba[1] == 0x00 && rgba[2] == 0xFF)
        {
            rgba[3] = 0;
        }
        sf::Color color(rgba[0], rgba[1], rgba[2], rgba[3]);
        pImage->setPixel(x, y, color);
        x++;
        if (x >= wWidth)
        {
            x = 0;
            y++;
        }
    }
}

sf::Texture* CSprite::GetTexture(uint16_t index)
{
    return vTextures.at(index);
}

uint16_t CSprite::GetTextureCount()
{
    return vTextures.size();
}

bool CSprite::IsValid()
{
    return bValid;
}
