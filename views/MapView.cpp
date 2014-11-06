#include "MapView.h"

#include <iostream>
#include <thread>

MapView::MapView(sf::RenderWindow &window, ContentPipeline &pipeline, std::string mapName)
{
    FileStream flstream;
    if(pipeline.getFileStream("data\\prontera.gat", flstream, true))
    {
        this->alt = new CAltitude(flstream);
        std::cout << "Alt Width: " << this->alt->GetWidth() << std::endl;
        std::cout << "Alt Height: " << this->alt->GetHeight() << std::endl;
    }

    if(pipeline.getFileStream("data\\prontera.gnd", flstream, true))
    {
        this->gnd = new CGround(flstream);
        std::cout << "Gnd Width: " << this->gnd->GetWidth() << std::endl;
        std::cout << "Gnd Height: " << this->gnd->GetHeight() << std::endl;

        for(int i = 0; i < this->gnd->GetTextureCount(); i++)
        {
            if(pipeline.getFileStream(std::string("data\\texture\\") + this->gnd->GetTexture(i), flstream, true))
            {
                this->textures.push_back(new CSprite(flstream));
                //std::cout << "data\\texture\\" << this->gnd->GetTexture(i) << " found!" << std::endl;
            }
            else
            {
                std::cout << "data\\texture\\" << this->gnd->GetTexture(i) << " not found!" << std::endl;
            }
        }
    }
}

MapView::~MapView()
{
    if(this->alt != nullptr)
    {
        delete this->alt;
    }
    if(this->gnd != nullptr)
    {
        delete this->gnd;
    }
}

void MapView::draw()
{
    for(uint32_t x = 0; x <  this->gnd->GetWidth(); x++)
    {
        for(uint32_t y = 0; y < this->gnd->GetHeight(); y++)
        {
            std::cout << *this->gnd->GetCell(x,y)->fHeight << " ";
        }
        std::cout << std::endl;
    }

    std::this_thread::sleep_for(std::chrono::seconds(5));
}
