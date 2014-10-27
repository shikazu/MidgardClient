#include "GMapRes.h"

GMapRes::GMapRes(const char* sFile)
{
    std::fstream *pFile = new std::fstream(sFile, std::fstream::in | std::fstream::binary);
    uint32_t wSig;
    pFile->read((char*)&wSig, 4);
    if (wSig == 0x57535247)//GRSW
    {

    }
    pFile->close();
}

GMapRes::~GMapRes()
{
}
