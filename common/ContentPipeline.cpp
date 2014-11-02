#include "ContentPipeline.h"
#include <SFML/Network.hpp>
#include <sstream>
#include <iostream>//Need to be  removed once std::cout is removed

std::vector<CGrf*> ContentPipeline::vGrfs;
ContentPipeline::~ContentPipeline()
{
}
ContentPipeline::ContentPipeline(std::string sIniFile)
{
    if (vGrfs.size() != 0) //Already loaded ini
    {
        return;
    }
    std::cout << "Initializing the content pipeline." << std::endl;
    std::fstream inistream(sIniFile);//fstream is already included in FileStream so no need for extra
    if (inistream.is_open())
    {
        std::string sLine;
        while (std::getline(inistream, sLine))
        {
            std::istringstream iss(sLine);//Parsing the easy way
            int key;
            char seperator;
            std::string value;
            if (!(iss >> key >> seperator >> value))
            {
                continue;    //Invalid Line or comment
            }

            std::cout << "Reading GRF File: " << value << std::endl;
            CGrf *grf = new CGrf(value);
            if(grf->IsOpen())
            {
                std::cout << "Reading GRF finished." << std::endl;
            }
            else
            {
                std::cout << "Reading GRF failed." << std::endl;
            }
            vGrfs.push_back(grf);
        }
    }
    else
    {
        std::cout << "GRF Support disabled! No data.ini found!" << std::endl;
    }
}

bool ContentPipeline::getFileStream(std::string sFile, FileStream &flstream, bool bGrfFirst)
{
    bool bStatus;
    if (bGrfFirst)
    {
        bStatus = getFromGrf(sFile, flstream);
        if (!bStatus)
        {
            bStatus = flstream.load(sFile);// Data Folder reading
        }
    }
    else
    {
        bStatus = flstream.load(sFile);// Data Folder reading
        if (!bStatus)
        {
            bStatus = getFromGrf(sFile, flstream);
        }
    }
    if (!bStatus)
    {
        bStatus = getFromServer(sFile, flstream);
    }
    if (!bStatus)//Debug purpose
        std::cout << sFile << " doesn't exists!" << std::endl;

    return bStatus;
}

bool ContentPipeline::getFromGrf(std::string sFile, FileStream &flstream)    // GRF Reading
{
    for ( uint16_t i = 0; i < vGrfs.size(); i++)
    {
        if (vGrfs.at(i)->Exists(sFile))
        {
            vGrfs.at(i)->GetContents(sFile, flstream);
            return true;
        }
    }
    return false;
}

bool ContentPipeline::getFromServer(std::string sFile, FileStream &flstream)
{
    /*
    std::string url = "http://example.com";
    std::string path = "/grf.php";

    sf::Http::Request request;
    request.setMethod(sf::Http::Request::Post);
    request.setUri(path);
    request.setHttpVersion(1, 1); // HTTP 1.1
    request.setField("Content-Type", "application/x-www-form-urlencoded");
    request.setBody("file=" + filePath);

    sf::Http http(url);
    sf::Http::Response response = http.sendRequest(request);
    if (response.getStatus() == sf::Http::Response::Ok)
    {
        out.write(response.getBody().c_str(),response.getBody().size());
        std::cout << filePath << " exists!" << std::endl;
        std::cout << response.getBody() << std::endl;
        return true;
    }*/

    // Not Found!
    return false;
}
