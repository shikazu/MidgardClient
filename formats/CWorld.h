#ifndef _CWORLD_H
#define _CWORLD_H
#include <vector>
#include "../common/FileStream.h"

class CWorld///Handler for RSW files
{
    public:
        CWorld(FileStream &flstream);
        virtual ~CWorld();

        bool IsValid();

    private:
        bool bValid;
};

#endif//_CWORLD_H
