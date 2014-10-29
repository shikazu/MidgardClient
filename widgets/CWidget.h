#ifndef _CWIDGET_H
#define _CWIDGET_H

class CWidget
{
    public:
        CWidget(uint32_t x, uint32_t y, uint32_t w, uint32_t h);
        virtual ~CWidget();

        void Move(int32_t dwXdelta, int32_t dwYdelta);
        void Resize(int32_t dwWdelta, int32_t dwHdelta)
        void MoveTo(uint32_t x, uint32_t y);
        void ResizeTo(uint32_t w, uint32_t h);

    protected:
        virtual void Paint();


    private:
        uint32_t dwLeft;
        uint32_t dwTop;
        uint32_t dwWidth;
        uint32_t dwHeight;

};

#endif//_CWIDGET_H
