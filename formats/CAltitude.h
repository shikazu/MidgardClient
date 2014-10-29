#ifndef _CALTITUDE_H
#define _CALTITUDE_H
#include <vector>
#include <fstream>
#include <SFML/System/InputStream.hpp>

class CAltitude///Handler for GAT files
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

        CAltitude(const char* sFile);
        CAltitude(std::istream &stream);
        virtual ~CAltitude();

        Cell* GetCell(uint32_t dwX, uint32_t dwY);
        uint32_t GetWidth();
        uint32_t GetHeight();
        bool IsValid();

    private:
        bool bValid;
        uint16_t wVersion;
        uint32_t dwWidth;
        uint32_t dwHeight;
        std::vector<Cell*> vCells;
        bool construct(std::istream &stream);
};

#endif//_CALTITUDE_H
