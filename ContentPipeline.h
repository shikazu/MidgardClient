#ifndef _CONTENTPIPELINE_H_
#define _CONTENTPIPELINE_H_

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>

#include "formats/GGrf.h"

class ContentPipeline
{
private:
    static std::vector<GGrf*> grf_handles;
public:
    ContentPipeline(std::string dataIniPath);

    bool getFileStream(std::string filePath, std::ostream& out);
};


#endif // _CONTENTPIPELINE_H_
