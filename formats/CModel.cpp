#include "CModel.h"

CModel::CModel(FileStream &flstream)
{
    uint32_t wSig;
    flstream.read(&wSig, 4);
    if (wSig != 0x4D535247)//GRSM
    {
        bValid = false;
        return;
    }

    bValid = true;
}

CModel::~CModel()
{
}

bool CModel::IsValid()
{
    return bValid;
}
