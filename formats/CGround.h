#ifndef _CGROUND_H
#define _CGROUND_H
#include <vector>
#include "../common/FileStream.h"

class CGround///Handler for GND files
{
    public:
        struct Cell
        {
            float fHeight[4];//lower number means higher ground
            int32_t lTopSurface;//All three are IDs. If none for respective surface, value will be -1
            int32_t lFrontSurface;
            int32_t lRightSurface;
        };

        struct Surface
        {
            float fU[4];
            float fV[4];
            uint16_t wTexture;
            uint16_t wLightMap;
            uint8_t pColor[4];
        };

        struct Lightmap
        {
            uint8_t cBrightness[8*8];//a
            uint8_t cIntensity[8*8][3];//r,g,b
        };

        CGround(FileStream &flstream);
        virtual ~CGround();

        char* GetTexture(uint32_t dwIndex);
        Lightmap* GetLightmap(uint32_t dwIndex);
        Surface* GetSurface(uint32_t dwIndex);
        Cell* GetCell(uint32_t dwX, uint32_t dwY);

        uint32_t GetTextureCount();
        uint32_t GetLightmapCount();
        uint32_t GetSurfaceCount();
        uint32_t GetWidth();//Cell count is just product of width & height so not adding that
        uint32_t GetHeight();
        bool IsValid();

    private:
        bool bValid;
        uint16_t wVersion;
        uint32_t dwWidth;
        uint32_t dwHeight;
        float fZoom;
        std::vector<char*> vTextures;
        std::vector<Lightmap*> vLightmaps;
        std::vector<Surface*> vSurfaces;
        std::vector<Cell*> vCells;

        bool fetchLightmaps(FileStream &flstream, uint32_t dwLightmapCount);
        bool genLightmaps(FileStream &flstream, uint32_t dwLightmapCount);
};

#endif//_CGROUND_H
