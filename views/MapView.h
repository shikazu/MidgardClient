#ifndef _VIEWS_MAPVIEW_H_
#define _VIEWS_MAPVIEW_H_

#include <windows.h>

#include <string>
#include <vector>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "../formats/CAltitude.h"
#include "../formats/CGround.h"
#include "../formats/CActor.h"
#include "../formats/CSprite.h"
#include "../common/ContentPipeline.h"
#include "../common/FileStream.h"

#include <GL/glew.h>
#include <GL/glut.h>
#include <SFML/OpenGL.hpp>

class MapView
{
private:
    CAltitude *alt;
    CGround *gnd;

    std::vector<CSprite*> textures;
    std::vector<GLuint> vbo_ground;
    std::vector<sf::Vector3f> ground;

    uint32_t frame;

public:
    MapView(sf::Window &window, ContentPipeline &pipeline, std::string mapName);
    ~MapView();

    void draw();
};

#endif // _VIEWS_MAPVIEW_H_
