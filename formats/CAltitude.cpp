#include "CAltitude.h"
#include <malloc.h>

CAltitude::CAltitude(const char* sFile)
{

    std::fstream stream(sFile, std::fstream::in | std::fstream::binary);
    bValid = construct(stream);
    stream.close();
}

CAltitude::CAltitude(std::istream &stream)
{
    bValid = construct(stream);
}

bool CAltitude::construct(std::istream &stream)
{
    uint32_t wSig;
    stream.read((char*)&wSig, 4);
    if (wSig != 0x54415247)//GRAT
    {
        return false;
    }

    stream.read((char*)&wVersion, 2);
    stream.read((char*)&dwWidth, 4);
    stream.read((char*)&dwHeight, 4);
    uint32_t dwCellCount = dwWidth * dwHeight;
    vCells.reserve(dwCellCount);
    for (uint32_t i = 0; i < dwCellCount; i++)
    {
        Cell* pCell = (Cell*)malloc(sizeof(Cell));
        stream.read((char*)pCell, sizeof(Cell));
        vCells.push_back(pCell);
    }
    return true;
}

CAltitude::~CAltitude()
{
}

CAltitude::Cell* CAltitude::GetCell(uint32_t dwX, uint32_t dwY)
{
    return vCells.at(dwX + dwWidth*dwY);
}

uint32_t CAltitude::GetWidth()
{
    return dwWidth;
}

uint32_t CAltitude::GetHeight()
{
    return dwHeight;
}
