#include "LoginView.h"

LoginView::LoginView(sf::RenderWindow *window)
{
    this->window = window;
    this->isLoggedIn = false;

    if(this->prepare())
    {
        this->draw();
    }
}

void LoginView::draw()
{
    while(!this->isLoggedIn)
    {
        if(this->window->isOpen())
        {
            sf::Event event;
            while (window->pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                {
                    // end the program
                    window->close();
                }
            }
            window->clear(sf::Color::Black);
            window->draw(this->sprites.at(0));
            window->display();
        }
    }
}

bool LoginView::prepare()
{
    sf::Sprite sprite;
    sf::Texture texture;
    if (texture.loadFromFile("data/login.jpg", sf::IntRect(0, 0, this->window->getSize().x, this->window->getSize().y)))
    {

        this->textures.push_back(texture);
        sprite.setTexture(this->textures.at(0));
        this->sprites.push_back(sprite);
        return true;
    }
    return false;
}
