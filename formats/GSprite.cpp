#include "GSprite.h"

GSprite::GSprite(const char* sFile, sf::Color *pPalette)
{
    std::fstream *pFile =  new std::fstream(sFile, std::fstream::in | std::fstream::binary);
    uint16_t wSig;
    pFile->read((char*)&wSig, 2);
    if (wSig == 0x5053)
    {
        uint16_t wImgCount;
        pFile->read((char*)&wVersion, 2);//Get version
        pFile->read((char*)&wImgCount, 2);//Get BMP count
        if (wImgCount > 0)
        {
            bIs8Bit = true;//Contains 8bpp images - bmp
            pFile->seekg(2, pFile->cur);//Skip TGA count - will be zero anyways
        }
        else
        {
            bIs8Bit = false;//Contains 32bpp images - tga
            pFile->read((char*)&wImgCount, 2);//Get TGA count
        }

        if (bIs8Bit && pPalette == 0)
        {
            pPalette = (sf::Color*)malloc(256 * sizeof(sf::Color));
            //Get the Palette
            int iPos = pFile->tellg();//Save Current Position
            pFile->seekg(-1024, pFile->end);//Skip to start of palette
            for (uint16_t i = 0; i < 256; i++)
            {
                uint8_t rgba[4];
                pFile->read((char*)rgba, 4);
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
            pFile->seekg(iPos, pFile->beg);
        }

        //Now for the images
        vImages.reserve(wImgCount);
        for (uint16_t i = 0; i < wImgCount; i++)
        {
            uint16_t wWidth, wHeight;
            pFile->read((char*)&wWidth , 2);
            pFile->read((char*)&wHeight, 2);
            sf::Image* pImage = new sf::Image();
            pImage->create(wWidth, wHeight);//, sf::Color::Transparent);
            if (this->bIs8Bit)
            {
                this->fetchBmp(pFile, pImage, pPalette);
            }
            else
            {
                this->fetchTga(pFile, pImage);
            }
            vImages.push_back(pImage);
        }
        bValid = true;
    }
    else
    {
        bValid = false;
    }
    pFile->close();
}

GSprite::~GSprite()
{
    uint16_t wImgCount = vImages.size();
    for (uint16_t i = 0; i < wImgCount; i++)
    {
        delete vImages.at(i);
    }
    vImages.clear();
}

void GSprite::fetchBmp(std::fstream* pFile, sf::Image* pImage, sf::Color* pPalette)
{
    uint16_t wDataLen, x = 0, y = 0;
    uint16_t wWidth = pImage->getSize().x;
    pFile->read((char*)&wDataLen, 2);
    for (uint16_t i = 0;i < wDataLen; i++)
    {
        uint8_t len = 1, index = pFile->get();
        if (index == 0)//For index 0 RLE is followed
        {
            i++;
            len = pFile->get();
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

void GSprite::fetchTga(std::fstream* pFile, sf::Image* pImage)
{
    uint16_t wWidth = pImage->getSize().x, wHeight = pImage->getSize().y;
    uint16_t x = 0, y = 0;
    uint32_t dwDataLen =  wWidth * wHeight;
    uint8_t rgba[4];
    for (uint32_t i = 0; i < dwDataLen; i++)
    {
        pFile->read((char*)rgba, 4);
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

sf::Image* GSprite::GetImage(uint16_t index)
{
    return vImages.at(index);
}

uint16_t GSprite::GetImageCount()
{
    return vImages.size();
}

bool GSprite::CheckValid()
{
    return bValid;
}
