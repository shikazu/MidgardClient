#include "Frame.h"

namespace UI
{
    Frame::Frame(uint32_t dwIdent, float x, float y, float w, float h):Widget(dwIdent, ENABLED|VISIBLE|DRAGGABLE|CLICKABLE, x, y, w, h)
    {
        pTexture = NULL;
    }
    Frame::Frame(uint32_t dwIdent, sf::Vector2f vPos, sf::Vector2f vSize):Widget(dwIdent, ENABLED|VISIBLE|DRAGGABLE|CLICKABLE, vPos, vSize)
    {
        pTexture = NULL;
    }
    Frame::~Frame()
    {
        if (pTexture != NULL)
        {
            delete pTexture;
        }
    }
    void Frame::SetTexture(FileStream &stream)
    {
        sf::Image image;
        image.loadFromStream(stream);
        image.createMaskFromColor(sf::Color::Magenta);
        sf::Vector2u vImageSize = image.getSize();

        if (GetWidth()  < vImageSize.x) { SetWidth( vImageSize.x); }
        if (GetHeight() < vImageSize.y) { SetHeight(vImageSize.y); }
        sf::IntRect rect(0, 0, GetWidth(), GetHeight());

        if (pTexture != NULL)
        {
            delete pTexture;
        }
        pTexture = new sf::Texture();
        pTexture->loadFromImage(image, rect);
    }
    void Frame::SetTexture(sf::Texture &tex)
    {
        pTexture = &tex;
    }

    void Frame::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        if (pTexture == NULL)
        {
            DrawBackground(target, states, false);
        }
        else
        {
            const sf::Texture* pBackup = states.texture;
            states.texture = pTexture;
            DrawBackground(target, states, true);
            states.texture = pBackup;//Restore texture
        }
        Widget::draw(target, states);
    }
}
