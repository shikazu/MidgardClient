#include "GSprite.h"

GSprite::GSprite(const char* filename)
{
    std::fstream *fp =  new std::fstream(filename, std::fstream::in | std::fstream::binary);
    uint16_t signature;
    fp->read((char*)&signature, 2);
    if (signature == 0x5053)
    {
        fp->read((char*)&version, 2);//Get version
        fp->read((char*)&imgCount, 2);//Get BMP count
        if (imgCount > 0)
        {
            this->is8bpp = true;//Contains 8bpp images - bmp
            fp->seekg(2, fp->cur);//Skip TGA count - will be zero anyways
        }
        else
        {
            this->is8bpp = false;//Contains 32bpp images - tga
            fp->read((char*)&imgCount, 2);//Get TGA count
        }

        sf::Color palette[256];
        if (is8bpp)
        {
            //Get the Palette
            int position = fp->tellg();//Save Current Position
            fp->seekg(-1024, fp->end);//Skip to start of palette
            for (uint16_t i = 0; i < 256; i++)
            {
                uint8_t rgba[4];
                fp->read((char*)rgba, 4);
                palette[i].r = rgba[0];
                palette[i].g = rgba[1];
                palette[i].b = rgba[2];
                if (i == 0 || (rgba[0] == 0xFF && rgba[1] == 0x00 && rgba[2] == 0xFF))//index = 0 as well as magenta will be transparent
                {
                    palette[i].a = 0;
                }
                else
                {
                    palette[i].a = 255;
                }
            }
            fp->seekg(position, fp->beg);
        }

        //Now for the images
        for (uint16_t i = 0; i < imgCount; i++)
        {
            uint16_t width, height;
            fp->read((char*)&width , 2);
            fp->read((char*)&height, 2);
            sf::Image* image = new sf::Image();
            image->create(width, height);//, sf::Color::Transparent);
            if (this->is8bpp)
            {
                this->fetchBmp(fp, image, palette);
            }
            else
            {
                this->fetchTga(fp, image);
            }
            images.push_back(image);
        }
    }
    fp->close();
}

GSprite::~GSprite()
{
    for (uint16_t i = 0; i < imgCount; i++)
    {
        delete images.at(i);
    }
}

void GSprite::fetchBmp(std::fstream* fp, sf::Image* image, sf::Color* palette)
{
    uint16_t dataLength, x = 0, y = 0;
    uint16_t width = image->getSize().x;
    fp->read((char*)&dataLength, 2);
    for (uint16_t i = 0;i < dataLength; i++)
    {
        uint8_t len = 1, index = fp->get();
        if (index == 0)
        {
            i++;
            len = fp->get();
        }
        for (uint8_t j = 0; j < len; j++)
        {
            sf::Color color = palette[index];
            image->setPixel(x, y, color);
            x++;
            if (x >= width)
            {
                x = 0;
                y++;
            }
        }
    }
}

void GSprite::fetchTga(std::fstream* fp, sf::Image* image)
{
    uint16_t width = image->getSize().x, height = image->getSize().y;
    uint16_t x = 0, y = 0;
    uint32_t dataLength =  width * height;
    uint8_t rgba[4];
    for (uint32_t i = 0; i < dataLength; i++)
    {
        fp->read((char*)rgba, 4);
        if (rgba[0] == 0xFF && rgba[1] == 0x00 && rgba[2] == 0xFF)
        {
            rgba[3] = 0;
        }
        sf::Color color(rgba[0], rgba[1], rgba[2], rgba[3]);
        image->setPixel(x, y, color);
        x++;
        if (x >= width)
        {
            x = 0;
            y++;
        }
    }
}

sf::Image* GSprite::GetImage(uint16_t index)
{
    return this->images.at(index);
}

uint16_t GSprite::GetImageCount()
{
    return this->imgCount;
}
