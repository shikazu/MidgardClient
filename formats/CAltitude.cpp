#include "CAltitude.h"

CAltitude::CAltitude(FileStream &flstream)
{
    uint32_t wSig;
    flstream.read(&wSig, 4);
    if (wSig != 0x54415247)//GRAT
    {
        bValid = false;
        return;
    }

    flstream.read(&wVersion, 2);
    flstream.read(&dwWidth, 4);
    flstream.read(&dwHeight, 4);
    uint32_t dwCellCount = dwWidth * dwHeight;
    vCells.reserve(dwCellCount);
    for (uint32_t i = 0; i < dwCellCount; i++)
    {
        Cell* pCell = new Cell;
        flstream.read(pCell, sizeof(Cell));
        vCells.push_back(pCell);
    }
    bValid = true;
}

CAltitude::~CAltitude()
{
    for (uint32_t i = 0; i < vCells.size(); i++)
    {
        delete[] vCells.at(i);
    }
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

bool CAltitude::IsValid()
{
    return bValid;
}
