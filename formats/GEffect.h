#ifndef _FORMATS_GEFFECT_H
#define _FORMATS_GEFFECT_H
#include <vector>
#include <fstream>
#include <malloc.h>

class GEffect//Handler for STR files used for creating Effects
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
            std::vector<GEffect::Frame*> vFrames;
        };

        GEffect(const char* sFile);
        virtual ~GEffect();

        Layer* GetLayer(uint32_t dwIndex);
        uint32_t GetLayerCount();

    private:
        uint32_t dwVersion;
        std::vector<Layer*> vLayers;
        void fetchLayer(std::fstream* pFile, GEffect::Layer* pLayer);
};

#endif//_FORMATS_GEFFECT_H
