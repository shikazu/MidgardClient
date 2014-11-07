#include "MapView.h"

#include <iostream>
#include <thread>

MapView::MapView(sf::Window &window, ContentPipeline &pipeline, std::string mapName)
{
    this->frame = 0;

    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        std::cout << "Something effed up";
    }

    FileStream flstream;
    if(pipeline.getFileStream(std::string("data\\") + mapName + ".gat", flstream, true))
    {
        this->alt = new CAltitude(flstream);
        std::cout << "Alt Width: " << this->alt->GetWidth() << std::endl;
        std::cout << "Alt Height: " << this->alt->GetHeight() << std::endl;
    }

    if(pipeline.getFileStream(std::string("data\\") + mapName + ".gnd", flstream, true))
    {
        this->gnd = new CGround(flstream);
        std::cout << "Gnd Width: " << this->gnd->GetWidth() << std::endl;
        std::cout << "Gnd Height: " << this->gnd->GetHeight() << std::endl;

        for(float x = 0; x <  this->gnd->GetWidth(); x++)
        {
            for(float y = 0; y < this->gnd->GetHeight(); y++)
            {
                this->ground.push_back(sf::Vector3f(x - 0.5,y - 0.5,this->gnd->GetCell(x,y)->fHeight[0] * 0.05));
                this->ground.push_back(sf::Vector3f(x - 0.5,y + 0.5,this->gnd->GetCell(x,y)->fHeight[2] * 0.05));
                this->ground.push_back(sf::Vector3f(x + 0.5,y + 0.5,this->gnd->GetCell(x,y)->fHeight[3] * 0.05));
                this->ground.push_back(sf::Vector3f(x + 0.5,y - 0.5,this->gnd->GetCell(x,y)->fHeight[1] * 0.05));

                /*this->ground.push_back(sf::Vector3f(x - 0.5,y - 0.5,1)); // left-lower
                this->ground.push_back(sf::Vector3f(x - 0.5,y + 0.5,1)); // left-upper
                this->ground.push_back(sf::Vector3f(x + 0.5,y + 0.5,1)); // right-upper
                this->ground.push_back(sf::Vector3f(x + 0.5,y - 0.5,1)); // right-lower*/
                /*for(uint32_t z = 0; z < 4; z++)
                {*/
                    /*vertices[(3*z)+0] = x;
                    vertices[(3*z)+1] = y;
                    vertices[(3*z)+2] = ;*/
                /*    this->ground.push_back(sf::Vector3f(x,y,this->gnd->GetCell(x,y)->fHeight[z]));

                    std::cout << x << " " << y << " " << this->gnd->GetCell(x,y)->fHeight[z] << std::endl;
                    std::this_thread::sleep_for(std::chrono::milliseconds(250));
                }*/

                /*GLuint vbo;
                glGenBuffers(1, &vbo);
                glBindBuffer(GL_ARRAY_BUFFER, vbo);
                glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

                this->vbo_ground.push_back(vbo);*/


            }
        }

        for(uint32_t i = 0; i < this->gnd->GetTextureCount(); i++)
        {
            if(pipeline.getFileStream(std::string("data\\texture\\") + this->gnd->GetTexture(i), flstream, true))
            {
                this->textures.push_back(new CSprite(flstream));
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
    //std::cout << this->frame++ << std::endl;
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    for(uint32_t i = 0; i < this->ground.size(); i+=4)
    {
        glBegin(GL_QUADS);
        glColor3f(1,1,1);

        glVertex3f((float)this->ground.at(i+0).x,(float)this->ground.at(i+0).y,(float)this->ground.at(i+0).z);
        glVertex3f((float)this->ground.at(i+1).x,(float)this->ground.at(i+1).y,(float)this->ground.at(i+1).z);
        glVertex3f((float)this->ground.at(i+2).x,(float)this->ground.at(i+2).y,(float)this->ground.at(i+2).z);
        glVertex3f((float)this->ground.at(i+3).x,(float)this->ground.at(i+3).y,(float)this->ground.at(i+3).z);

        glEnd();
    }
}
