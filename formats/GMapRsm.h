#ifndef _FORMATS_GMAPRSM_H
#define _FORMATS_GMAPRSM_H
#include <vector>
#include <fstream>

class GMapRsm//Handler for RSM files
{
    public:
        GMapRsm(const char* sFile);
        virtual ~GMapRsm();
    protected:
    private:
};

#endif // _FORMATS_GMAPRSM_H
