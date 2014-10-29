#ifndef _CWORLD_H
#define _CWORLD_H
#include <vector>
#include <fstream>

class CWorld///Handler for RSW files
{
    public:
        CWorld(const char* sFile);
        CWorld(std::istream &stream);
        virtual ~CWorld();

        bool IsValid();

    private:
        bool bValid;
        bool construct(std::istream &stream);
};

#endif//_CWORLD_H
