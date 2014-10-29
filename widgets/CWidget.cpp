#include "CWidget.h"

CWidget::CWidget(uint32_t x, uint32_t y, uint32_t w, uint32_t h)
{
    this->dwLeft = x;
    this->dwTop = y;
    this->dwWidth = w;
    this->dwHeight = h;
}

CWidget::~CWidget()
{
}

void CWidget::Paint()
{
}

void CWidget::Move(int32_t dwXdelta, int32_t dwYdelta)
{
    this->dwLeft += dwXdelta;
    this->dwTop  += dwYdelta;
    this->Paint();
}

void CWidget::MoveTo(uint32_t x, uint32_t y)
{
    this->dwLeft = x;
    this->dwTop  = y;
    this->Paint();
}

void CWidget::Resize(int32_t dwWdelta, int32_t dwHdelta)
{
    this->dwWidth  += dwWdelta;
    this->dwHeight += dwHdelta;
    this->Paint();
}

void CWidget::ResizeTo(uint32_t w, uint32_t h)
{
    this->dwWidth = w;
    this->dwHeight = h;
    this->Paint();
}
