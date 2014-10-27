#include "GMapRsm.h"

GMapRsm::GMapRsm(const char* sFile)
{
    std::fstream *pFile = new std::fstream(sFile, std::fstream::in | std::fstream::binary);
    uint32_t wSig;
    pFile->read((char*)&wSig, 4);
    if (wSig == 0x4D535247)//GRSM
    {

    }
    pFile->close();
}

GMapRsm::~GMapRsm()
{
}
