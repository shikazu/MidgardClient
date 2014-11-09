#ifndef _DRAW_H
#define _DRAW_H
#include <SFML/Graphics.hpp>
///Defines some drawing functions for convenience

namespace Draw
{
    void Text(sf::RenderWindow &window, sf::RenderStates states, sf::String &text, sf::Font &font, sf::Color &color, sf::Vector2f &pos, sf::Vector2f &size, uint32_t dwStartIndex = 0);
    void Image(sf::RenderWindow &window, sf::RenderStates states, sf::Texture &texture, sf::Vector2f &pos, sf::Vector2f &size);
    void Rectangle(sf::RenderWindow &window, sf::RenderStates states, sf::Color &color, sf::Vector2f &pos, sf::Vector2f &size, bool bFill = false, float lRadius = 0);//Supports Rounded Corners
}
#endif//_DRAW_H
