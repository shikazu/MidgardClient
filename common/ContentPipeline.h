#ifndef _CONTENTPIPELINE_H_
#define _CONTENTPIPELINE_H_

#include <string>
#include <vector>
#include "FileStream.h"
#include "../formats/CGrf.h"
#include <SFML/Graphics.hpp>

class ContentPipeline
{
    public:
        ContentPipeline(std::string sIniFile);
        ~ContentPipeline();
        bool getFileStream(std::string sFile, FileStream &flstrm, bool bGrfFirst = false);
        bool getTexture(std::string sFile, sf::Texture *pTexture, bool bGrfFirst = false);

    private:
        static std::vector<CGrf*> vGrfs;
        bool getFromGrf(std::string sFile, FileStream &flstrm);
        bool getFromServer(std::string sFile, FileStream &flstrm);
};

#endif//_CONTENTPIPELINE_H_
