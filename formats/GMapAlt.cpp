#include "GMapAlt.h"
#include <malloc.h>

GMapAlt::GMapAlt(const char* sFile)
{
    std::fstream *pFile = new std::fstream(sFile, std::fstream::in | std::fstream::binary);
    uint32_t wSig;
    pFile->read((char*)&wSig, 4);
    if (wSig == 0x54415247)//GRAT
    {
        pFile->read((char*)&wVersion, 2);
        pFile->read((char*)&dwWidth, 4);
        pFile->read((char*)&dwHeight, 4);
        uint32_t dwCellCount = dwWidth * dwHeight;
        vCells.reserve(dwCellCount);
        for (uint32_t i = 0; i < dwCellCount; i++)
        {
            Cell* pCell = (Cell*)malloc(sizeof(Cell));
            pFile->read((char*)pCell, sizeof(Cell));
            vCells.push_back(pCell);
        }
    }
    pFile->close();
}

GMapAlt::~GMapAlt()
{
}

GMapAlt::Cell* GMapAlt::GetCell(uint32_t dwX, uint32_t dwY)
{
    return vCells.at(dwX + dwWidth*dwY);
}

uint32_t GMapAlt::GetWidth()
{
    return dwWidth;
}

uint32_t GMapAlt::GetHeight()
{
    return dwHeight;
}
