#include "CWorld.h"

CWorld::CWorld(FileStream &flstream)
{
    uint32_t wSig;
    flstream.read(&wSig, 4);
    if (wSig != 0x57535247)//GRSW
    {
        bValid = false;
        return;
    }

    bValid = true;
}

CWorld::~CWorld()
{
}

bool CWorld::IsValid()
{
    return bValid;
}
