#include "CModel.h"

CModel::CModel(const char* sFile)
{
    std::fstream stream(sFile, std::fstream::in | std::fstream::binary);
    bValid = construct(stream);
    stream.close();
}

CModel::CModel(std::istream &stream)
{
    bValid = construct(stream);
}

bool CModel::construct(std::istream &stream)
{
    uint32_t wSig;
    stream.read((char*)&wSig, 4);
    if (wSig != 0x4D535247)//GRSM
    {
        return false;
    }

    return true;
}

CModel::~CModel()
{
}

bool CModel::IsValid()
{
    return bValid;
}
