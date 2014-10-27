#include "GEffect.h"

GEffect::GEffect(const char* sFile)
{
    std::fstream *pFile =  new std::fstream(sFile, std::fstream::in | std::fstream::binary);
    uint32_t wSig;
    pFile->read((char*)&wSig, 4);
    if (wSig == 0x4D525453)//STRM
    {
        uint32_t dwFrameCount, dwFPS, dwLayerCount;//not sure what this frame count is for
        pFile->read((char*)&dwFrameCount, 4);
        pFile->read((char*)&dwFPS, 4);
        pFile->read((char*)&dwLayerCount, 4);
        pFile->seekg(16, pFile->cur);
        vLayers.reserve(dwLayerCount);
        for (uint32_t i = 0; i < dwLayerCount; i++)
        {
            Layer* pLayer = (Layer*)malloc(sizeof(Layer));
            fetchLayer(pFile, pLayer);
            vLayers.push_back(pLayer);
        }
    }
    pFile->close();
}

GEffect::~GEffect()
{
}

void GEffect::fetchLayer(std::fstream* pFile, GEffect::Layer* pLayer)
{
    uint32_t dwImageCount;//Texture count
    pFile->read((char*)&dwImageCount, 4);
    pLayer->vImages.reserve(dwImageCount);
    for (uint32_t i = 0; i < dwImageCount; i++)
    {
        char* sFile = (char*)malloc(256);
        pFile->read(sFile, 256);
        pLayer->vImages.push_back(sFile);
    }
    uint32_t dwFrameCount;//KeyFrame Count
    pFile->read((char*)&dwFrameCount, 4);
    pLayer->vFrames.reserve(dwFrameCount);
    for (uint32_t i = 0; i < dwFrameCount; i++)
    {
        Frame* pFrame = (Frame*)malloc(sizeof(Frame));
        pFile->read((char*)pFrame, sizeof(Frame));
        pLayer->vFrames.push_back(pFrame);
    }
}

GEffect::Layer* GEffect::GetLayer(uint32_t dwIndex)
{
    return vLayers.at(dwIndex);
}

uint32_t GEffect::GetLayerCount()
{
    return vLayers.size();
}
