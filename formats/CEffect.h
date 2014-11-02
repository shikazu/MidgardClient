#ifndef _GEFFECT_H
#define _GEFFECT_H
#include <vector>
#include "../common/FileStream.h"

class CEffect///Handler for STR files used for creating Effects
{
    public:
        struct Frame //Animation KeyFrame
        {
            uint32_t dwFrameNum;
            uint32_t dwFrameType;
            float fX;
            float fY;
            float fU;
            float fV;
            float fUS;
            float fVS;
            float fU2;
            float fV2;
            float fUS2;
            float fVS2;
            float fAX[4];//Vertice coordinates
            float fAY[4];//Vertice coordinates
            float fAniFrame;
            uint32_t dwAniType;
            float fAniDelta;
            float fRotation;
            uint32_t dwRGBA;//should be converted to color
            uint32_t dwSrcBlend;
            uint32_t dwDstBlend;
            uint32_t dwMtPreset;
        };

        struct Layer
        {
            std::vector<char*> vImages;
            std::vector<CEffect::Frame*> vFrames;
        };

        CEffect(FileStream &flstream);
        virtual ~CEffect();

        Layer* GetLayer(uint32_t dwIndex);
        uint32_t GetLayerCount();
        bool IsValid();

    private:
        bool bValid;
        uint32_t dwVersion;
        std::vector<Layer*> vLayers;

        void fetchLayer(FileStream &flstream, CEffect::Layer* pLayer);
};

#endif//_CEFFECT_H
