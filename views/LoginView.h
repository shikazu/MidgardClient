#ifndef _VIEWS_LOGINVIEW_H_
#define _VIEWS_LOGINVIEW_H_

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>

class LoginView
{
private:
    sf::RenderWindow *window;
    bool isLoggedIn;

    void draw();
    bool prepare();

    std::vector<sf::Sprite> sprites;
    std::vector<sf::Texture> textures;

public:
    LoginView(sf::RenderWindow *window);
};

#endif

