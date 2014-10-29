#include "CWorld.h"

CWorld::CWorld(const char* sFile)
{
    std::fstream stream(sFile, std::fstream::in | std::fstream::binary);
    bValid = construct(stream);
    stream.close();
}

CWorld::CWorld(std::istream &stream)
{
    bValid = construct(stream);
}

bool CWorld::construct(std::istream& stream)
{
    uint32_t wSig;
    stream.read((char*)&wSig, 4);
    if (wSig != 0x57535247)//GRSW
    {
        return false;
    }
    return true;
}

CWorld::~CWorld()
{
}

bool CWorld::IsValid()
{
    return bValid;
}
