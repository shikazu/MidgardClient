#include "UIWidget.h"
#include "UIManager.h"

UIWidget::UIWidget(uint32_t x, uint32_t y, uint32_t w, uint32_t h, UIWidget *parent)
{
    pParent = parent;
    vPos.x  = x;
    vPos.y  = y;
    vSize.x = w;
    vSize.y = h;
    pColors[ForeColor] = sf::Color::White;
    pColors[BackColor] = sf::Color::Transparent;
    pColors[BorderColor] = sf::Color::Black;
    fBorder = 1.0;
    bClickable = true;//Disabled for widgets that dont respond to any mouse press/release actions
    bHasFocus = false;//Enabled for only 1 widget at a time for Keyboard/Text Entry focus
}

UIWidget::~UIWidget()
{
}

//Getters and Setters
void UIWidget::Position(sf::Vector2u p) {vPos = p;}
void UIWidget::Position(uint32_t x, uint32_t y) {vPos.x = x; vPos.y = y;}
sf::Vector2u UIWidget::Position() {return vPos;}

void UIWidget::Size(sf::Vector2u s) {vSize = s;}
void UIWidget::Size(uint32_t w, uint32_t h) {vSize.x = w; vSize.y = h;}
sf::Vector2u UIWidget::Size() {return vSize;}

void UIWidget::Color(sf::Color c, uint8_t uType)
{
    if (uType >= InvalidColor) {return;}
    pColors[uType] = c;
}
sf::Color UIWidget::Color(uint8_t uType)
{
    if (uType >= InvalidColor) {return sf::Color::Black;}
    return pColors[uType];
}

void UIWidget::BorderWidth(float bd) {fBorder = bd;}
float UIWidget::BorderWidth() {return fBorder;}

void UIWidget::SetFocus() {bHasFocus = true;}
void UIWidget::KillFocus() {bHasFocus = false;}
bool UIWidget::IsClickable() {return bClickable;}


void UIWidget::Paint(sf::RenderWindow &window)
{
    sf::RectangleShape shape(sf::Vector2f(vSize.x, vSize.y));
    shape.setFillColor(pColors[BackColor]);
    shape.setPosition(vPos.x, vPos.y);
    shape.setOutlineColor(pColors[BorderColor]);
    shape.setOutlineThickness(fBorder);
    window.draw(shape);
}

bool UIWidget::IsPointInside(uint32_t x, uint32_t y)
{
    sf::IntRect rect(vPos.x, vPos.y, vSize.x, vSize.y);
    return rect.contains(x, y);
}

void UIWidget::MousePressed(sf::Event::MouseButtonEvent btnEvent) {}
void UIWidget::MouseReleased(sf::Event::MouseButtonEvent btnEvent) {}
void UIWidget::MouseClicked(sf::Event::MouseButtonEvent btnEvent) {}

void UIWidget::MouseEntered(sf::Event::MouseMoveEvent movEvent) {}
void UIWidget::MouseLeft(sf::Event::MouseMoveEvent movEvent) {}
void UIWidget::MouseMoved(sf::Event::MouseMoveEvent movEvent) {}

void UIWidget::KeyPressed(sf::Event::KeyEvent keyEvent) {}
void UIWidget::KeyReleased(sf::Event::KeyEvent keyEvent) {}
void UIWidget::TextEntered(sf::Event::TextEvent textEvent) {}
