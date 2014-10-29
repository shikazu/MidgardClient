#include "CEffect.h"

CEffect::CEffect(const char* sFile)
{
    std::fstream stream(sFile, std::fstream::in | std::fstream::binary);
    bValid = construct(stream);
    stream.close();
}

CEffect::CEffect(std::istream &stream)
{
    bValid = construct(stream);
}

bool CEffect::construct(std::istream &stream)
{
    uint32_t wSig;
    stream.read((char*)&wSig, 4);
    if (wSig != 0x4D525453)//STRM
    {
        return false;
    }

    uint32_t dwFrameCount, dwFPS, dwLayerCount;//not sure what this frame count is for
    stream.read((char*)&dwFrameCount, 4);
    stream.read((char*)&dwFPS, 4);
    stream.read((char*)&dwLayerCount, 4);
    stream.seekg(16, stream.cur);
    vLayers.reserve(dwLayerCount);
    for (uint32_t i = 0; i < dwLayerCount; i++)
    {
        Layer* pLayer = (Layer*)malloc(sizeof(Layer));
        fetchLayer(stream, pLayer);
        vLayers.push_back(pLayer);
    }
    return true;
}

CEffect::~CEffect()
{
}

void CEffect::fetchLayer(std::istream &stream, CEffect::Layer* pLayer)
{
    uint32_t dwImageCount;//Texture count
    stream.read((char*)&dwImageCount, 4);
    pLayer->vImages.reserve(dwImageCount);
    for (uint32_t i = 0; i < dwImageCount; i++)
    {
        char* sFile = (char*)malloc(256);
        stream.read(sFile, 256);
        pLayer->vImages.push_back(sFile);
    }

    uint32_t dwFrameCount;//KeyFrame Count
    stream.read((char*)&dwFrameCount, 4);
    pLayer->vFrames.reserve(dwFrameCount);
    for (uint32_t i = 0; i < dwFrameCount; i++)
    {
        Frame* pFrame = (Frame*)malloc(sizeof(Frame));
        stream.read((char*)pFrame, sizeof(Frame));
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
