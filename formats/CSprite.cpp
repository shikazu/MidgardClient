#include "CSprite.h"

CSprite::CSprite(FileStream &flstream, sf::Color *pPalette)
{
    uint16_t wSig;
    flstream.read(&wSig, 2);
    if (wSig != 0x5053)
    {
        bValid = false;
        return;
    }

    uint16_t wImgCount;
    bool bIs8Bit;//Flag for 8BitPerPixel or not
    flstream.read(&wVersion, 2);//Get version
    flstream.read(&wImgCount, 2);//Get BMP count
    if (wImgCount > 0)
    {
        bIs8Bit = true;//Contains 8bpp images - bmp
        flstream.seek(2, flstream.CUR);//Skip TGA count - will be zero anyways
    }
    else
    {
        bIs8Bit = false;//Contains 32bpp images - tga
        flstream.read(&wImgCount, 2);//Get TGA count
    }

    if (bIs8Bit && pPalette == 0)
    {
        pPalette = new sf::Color[256];
        //Get the Palette
        int iPos = flstream.tell();//Save Current Position
        flstream.seek(0, flstream.END);
        flstream.seek(-1024, flstream.END);//Skip to start of palette

        for (uint16_t i = 0; i < 256; i++)
        {
            uint8_t rgba[4];
            flstream.read(rgba, 4);
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
        flstream.seek(iPos, flstream.BEG);
    }

    //Now for the textures
    vTextures.reserve(wImgCount);
    for (uint16_t i = 0; i < wImgCount; i++)
    {
        uint16_t wWidth, wHeight;
        flstream.read(&wWidth , 2);
        flstream.read(&wHeight, 2);
        sf::Image image;
        image.create(wWidth, wHeight);//, sf::Color::Transparent);
        if (bIs8Bit)
        {
            fetchBmp(flstream, &image, pPalette);
        }
        else
        {
            fetchTga(flstream, &image);
        }
        sf::Texture *pTexture = new sf::Texture();
        pTexture->loadFromImage(image);
        //pTexture->setSmooth(true);
        vTextures.push_back(pTexture);
    }
    bValid = true;
}

CSprite::~CSprite()
{
    for (uint32_t i = 0; i < vTextures.size(); i++)
    {
        delete[] vTextures.at(i);
    }
}

void CSprite::fetchBmp(FileStream &flstream, sf::Image* pImage, sf::Color* pPalette)
{
    uint16_t wDataLen, x = 0, y = 0;
    uint16_t wWidth = pImage->getSize().x;
    flstream.read(&wDataLen, 2);
    for (uint16_t i = 0;i < wDataLen; i++)
    {
        uint8_t len = 1, index = flstream.readByte();
        if (index == 0)//For index 0 RLE is followed
        {
            i++;
            len = flstream.readByte();
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

void CSprite::fetchTga(FileStream &flstream, sf::Image* pImage)
{
    uint16_t wWidth = pImage->getSize().x, wHeight = pImage->getSize().y;
    uint16_t x = 0, y = 0;
    uint32_t dwDataLen =  wWidth * wHeight;
    uint8_t rgba[4];
    for (uint32_t i = 0; i < dwDataLen; i++)
    {
        flstream.read(rgba, 4);
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
