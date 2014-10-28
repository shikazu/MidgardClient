#include "GMapGnd.h"
#include <malloc.h>

GMapGnd::GMapGnd(const char* sFile)
{
    std::fstream pFile(sFile, std::fstream::in | std::fstream::binary);
    this->init(pFile);
    pFile.close();
}

GMapGnd::GMapGnd(std::istream &stream)
{
    this->init(stream);
}

GMapGnd::~GMapGnd()
{
}

void GMapGnd::init(std::istream &stream)
{
    uint32_t wSig;
    stream.read((char*)&wSig, 4);
    if (wSig == 0x4E475247)//GRGN
    {
        stream.read((char*)&wVersion, 2);
        stream.read((char*)&dwWidth, 4);
        stream.read((char*)&dwHeight, 4);
        stream.read((char*)&fZoom, 4);

        //Read Textures
        uint32_t dwTextureCount, dwTextureNameLen;
        stream.read((char*)&dwTextureCount, 4);
        stream.read((char*)&dwTextureNameLen, 4);
        vTextures.reserve(dwTextureCount);
        for (uint32_t i = 0; i < dwTextureCount; i++)
        {
            char* sTexture = (char*)malloc(dwTextureNameLen+1);
            sTexture[dwTextureNameLen] = 0;
            stream.read(sTexture, dwTextureNameLen);
            vTextures.push_back(sTexture);
        }
        //Fetch/Generate Lightmaps
        uint32_t dwLightmapCount;
        stream.read((char*)&dwLightmapCount, 4);
        if (wVersion >= 0x0107)
        {
            fetchLightmaps(stream, dwLightmapCount);
        }
        else
        {
            genLightmaps(stream, dwLightmapCount);
        }
        if (!bValid)
        {
            return;
        }

        //Read surfaces
        uint32_t dwSurfaceCount;
        stream.read((char*)&dwSurfaceCount, 4);
        vSurfaces.reserve(dwSurfaceCount);
        for (uint32_t i = 0; i < dwSurfaceCount; i++)
        {
            Surface* pSurface = (Surface*)malloc(sizeof(Surface));
            stream.read((char*)pSurface, sizeof(Surface));
            vSurfaces.push_back(pSurface);
        }

        //Read Cells
        uint32_t dwCellCount = dwWidth * dwHeight;
        vCells.reserve(dwCellCount);
        for (uint32_t i = 0; i < dwCellCount; i++)
        {
            Cell* pCell = (Cell*)malloc(sizeof(Cell));
            if (wVersion >= 0x0107)
            {
                stream.read((char*)pCell, sizeof(Cell));
            }
            else//Surface IDs are 16 bit
            {
                uint16_t wTopSurface, wFrontSurface, wRightSurface;
                stream.read((char*)pCell->fHeight, sizeof(pCell->fHeight));
                stream.read((char*)&wTopSurface, 2);
                stream.read((char*)&wFrontSurface, 2);
                stream.read((char*)&wRightSurface, 2);

                pCell->lTopSurface = wTopSurface;
                pCell->lFrontSurface = wFrontSurface;
                pCell->lRightSurface = wRightSurface;
            }
            vCells.push_back(pCell);
        }
    }
}

void GMapGnd::fetchLightmaps(std::istream &stream, uint32_t dwLightmapCount)
{
    uint32_t dwLMwidth, dwLMHeight, dwLMCells;//Should be 8, 8, 1
    stream.read((char*)&dwLMwidth, 4);
    stream.read((char*)&dwLMHeight, 4);
    stream.read((char*)&dwLMCells, 4);
    if (dwLMwidth != 8 || dwLMHeight != 8 || dwLMCells != 1)
    {
        bValid = false;
        return;
    }

    vLightmaps.reserve(dwLightmapCount);
    for (uint32_t i = 0; i < dwLightmapCount; i++)
    {
        Lightmap* pLM = (Lightmap*)malloc(sizeof(Lightmap));
        stream.read((char*)pLM, sizeof(Lightmap));
        vLightmaps.push_back(pLM);
    }
}

void GMapGnd::genLightmaps(std::istream &stream, uint32_t dwLightmapCount)
{
    uint8_t pLightMapIndices[dwLightmapCount][4];
    stream.read((char*)pLightMapIndices, dwLightmapCount * 4);

    uint32_t dwColorChannels;
    stream.read((char*)&dwColorChannels, 4);

    char sColorChannels[dwColorChannels][40];
    stream.read((char*)sColorChannels, dwColorChannels * 40);

    for (uint32_t i = 0; i < dwLightmapCount; i++)
    {
        uint8_t* pLMIndex = pLightMapIndices[i];
        if (pLMIndex[0] >= dwColorChannels || pLMIndex[1] >= dwColorChannels || pLMIndex[2] >= dwColorChannels || pLMIndex[3] >= dwColorChannels)
        {
            bValid = false;
            return;
        }
        char *sChannel_a = sColorChannels[pLMIndex[0]], *sChannel_r = sColorChannels[pLMIndex[1]];
        char *sChannel_g = sColorChannels[pLMIndex[2]], *sChannel_b = sColorChannels[pLMIndex[3]];
        Lightmap* pLM = (Lightmap*)malloc(sizeof(Lightmap));
        uint32_t aux = 0;
        for (uint32_t j = 0;  j < 64; j++, aux+=5)
        {
            uint32_t dwLM_i = (j/8) + 8*(j%8);//0-56,1-57,...7-63 where each of the ranges will be seperated by 8
            uint32_t dwLow = aux%8, dwHigh = aux/8;
            uint8_t a, r, g, b;
            a = (sChannel_a[dwHigh] & (0xF8 >> dwLow)) << dwLow;
            r = (sChannel_r[dwHigh] & (0xF8 >> dwLow)) << dwLow;
            g = (sChannel_g[dwHigh] & (0xF8 >> dwLow)) << dwLow;
            b = (sChannel_b[dwHigh] & (0xF8 >> dwLow)) << dwLow;
            if (dwLow >= 4)
            {
                a += ((0xF8 << (8-dwLow)) & sChannel_a[dwHigh+1]) >> (8-dwLow);
                r += ((0xF8 << (8-dwLow)) & sChannel_r[dwHigh+1]) >> (8-dwLow);
                g += ((0xF8 << (8-dwLow)) & sChannel_g[dwHigh+1]) >> (8-dwLow);
                b += ((0xF8 << (8-dwLow)) & sChannel_b[dwHigh+1]) >> (8-dwLow);
            }
            pLM->cBrightness[dwLM_i] = a;
            pLM->cIntensity[dwLM_i][0] = r;
            pLM->cIntensity[dwLM_i][1] = g;
            pLM->cIntensity[dwLM_i][2] = b;
         }
    }
}

char* GMapGnd::GetTexture(uint32_t dwIndex)
{
    return vTextures.at(dwIndex);
}
GMapGnd::Lightmap* GMapGnd::GetLightmap(uint32_t dwIndex)
{
    return vLightmaps.at(dwIndex);
}
GMapGnd::Surface* GMapGnd::GetSurface(uint32_t dwIndex)
{
    return vSurfaces.at(dwIndex);
}
GMapGnd::Cell* GMapGnd::GetCell(uint32_t dwX, uint32_t dwY)
{
    return vCells.at(dwX + dwWidth * dwY);
}

uint32_t GMapGnd::GetTextureCount()
{
    return vTextures.size();
}
uint32_t GMapGnd::GetLightmapCount()
{
    return vLightmaps.size();
}
uint32_t GMapGnd::GetSurfaceCount()
{
    return vSurfaces.size();
}
uint32_t GMapGnd::GetWidth()
{
    return dwWidth;
}
uint32_t GMapGnd::GetHeight()
{
    return dwHeight;
}
