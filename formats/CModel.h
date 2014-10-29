#ifndef _CMODEL_H
#define _CMODEL_H
#include <vector>
#include <fstream>

class CModel///Handler for RSM files
{
    public:
        CModel(const char* sFile);
        CModel(std::istream &stream);
        virtual ~CModel();

        bool IsValid();

    private:
        bool bValid;
        bool construct(std::istream &stream);
};

#endif//_CMODEL_H
