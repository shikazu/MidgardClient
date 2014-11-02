#include "CEffect.h"

CEffect::CEffect(FileStream &flstream)
{
    uint32_t wSig;
    flstream.read(&wSig, 4);
    if (wSig != 0x4D525453)//STRM
    {
        bValid = false;
        return;
    }

    uint32_t dwFrameCount, dwFPS, dwLayerCount;//not sure what this frame count is for
    flstream.read(&dwFrameCount, 4);
    flstream.read(&dwFPS, 4);
    flstream.read(&dwLayerCount, 4);
    vLayers.reserve(dwLayerCount);

    flstream.seek(16, flstream.CUR);
    for (uint32_t i = 0; i < dwLayerCount; i++)
    {
        Layer* pLayer = new Layer;
        fetchLayer(flstream, pLayer);
        vLayers.push_back(pLayer);
    }
    bValid = true;
}

CEffect::~CEffect()
{
    for (uint32_t i = 0; i < vLayers.size(); i++)
    {
        Layer* pLayer = vLayers.at(i);
        for (uint32_t j = 0; j < pLayer->vFrames.size(); j++)
        {
            Frame* pFrame = pLayer->vFrames.at(j);
            delete[] pFrame;
        }
        for (uint32_t j = 0; j < pLayer->vImages.size(); j++)
        {
            delete[] pLayer->vImages.at(j);
        }
        delete[] pLayer;
    }
}

void CEffect::fetchLayer(FileStream &flstream, CEffect::Layer* pLayer)
{
    uint32_t dwImageCount;//Texture count
    flstream.read(&dwImageCount, 4);
    pLayer->vImages.reserve(dwImageCount);
    for (uint32_t i = 0; i < dwImageCount; i++)
    {
        char* sFile = new char[256];
        flstream.read(sFile, 256);
        pLayer->vImages.push_back(sFile);
    }

    uint32_t dwFrameCount;//KeyFrame Count
    flstream.read(&dwFrameCount, 4);
    pLayer->vFrames.reserve(dwFrameCount);
    for (uint32_t i = 0; i < dwFrameCount; i++)
    {
        Frame* pFrame = new Frame;
        flstream.read(pFrame, sizeof(Frame));
        pLayer->vFrames.push_back(pFrame);
    }
}

CEffect::Layer* CEffect::GetLayer(uint32_t dwIndex)
{
    return vLayers.at(dwIndex);
}

uint32_t CEffect::GetLayerCount()
{
    return vLayers.size();
}

bool CEffect::IsValid()
{
    return bValid;
}
