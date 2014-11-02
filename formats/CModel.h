#ifndef _CMODEL_H
#define _CMODEL_H
#include <vector>
#include "../common/FileStream.h"

class CModel///Handler for RSM files
{
    public:
        CModel(FileStream &flstream);
        virtual ~CModel();

        bool IsValid();

    private:
        bool bValid;
};

#endif//_CMODEL_H
