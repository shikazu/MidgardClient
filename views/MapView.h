#ifndef _VIEWS_MAPVIEW_H_
#define _VIEWS_MAPVIEW_H_

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

class MapView
{
private:
    CAltitude *alt;
    CGround *gnd;

    std::vector<CSprite*> textures;

public:
    MapView(sf::RenderWindow &window, ContentPipeline &pipeline, std::string mapName);
    ~MapView();

    void draw();
};

#endif // _VIEWS_MAPVIEW_H_
