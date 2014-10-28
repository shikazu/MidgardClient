#ifndef _FORMATS_GMAPALT_H
#define _FORMATS_GMAPALT_H
#include <vector>
#include <fstream>
#include <SFML/System/InputStream.hpp>

class GMapAlt//Handler for GAT files
{
    public:
        enum //suffixes -> W means walkable, S means snipable
        {
            LAND_WS = 0,//The LAND prefix is generic - for e.g. It could also mean the air in the case of sky maps
            LAND_S,
            WATER,
            WATER_WS,
            WATER_S,
            CLIFF_S,
            CLIFF
            //Remaining values are unknown (if used at all)
        };

        struct Cell
        {
            float fHeight[4];
            uint32_t dwType;
        };

        GMapAlt(const char* sFile);
        GMapAlt(std::istream &stream);
        virtual ~GMapAlt();

        Cell* GetCell(uint32_t dwX, uint32_t dwY);
        uint32_t GetWidth();
        uint32_t GetHeight();

    private:
        uint16_t wVersion;
        uint32_t dwWidth;
        uint32_t dwHeight;
        std::vector<Cell*> vCells;
};

#endif//_FORMATS_GMAPALT_H
