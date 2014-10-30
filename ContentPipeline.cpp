#include "ContentPipeline.h"

#include <SFML/Network.hpp>
std::vector<CGrf*> ContentPipeline::grf_handles;

ContentPipeline::ContentPipeline(std::string dataIniPath)
{
    if(this->grf_handles.size() == 0)
    {
        std::cout << "Initializing the content pipeline." << std::endl;
        std::fstream ini(dataIniPath);
        if(ini.is_open())
        {
            std::string line;
            while (std::getline(ini, line))
            {
                std::istringstream iss(line);
                int key;
                char seperator;
                std::string value;
                if (!(iss >> key >> seperator >> value))
                {
                    continue;    // error
                }

                std::cout << "Reading GRF File: " << value << std::endl;

                CGrf *grf = new CGrf();
                if(grf->open(value))
                {
                    std::cout << "Reading GRF finished." << std::endl;
                }
                else
                {
                    std::cout << "Reading GRF failed." << std::endl;
                }
                this->grf_handles.push_back(grf);
            }
        }
        else
        {
            std::cout << "GRF Support disabled! No data.ini found!" << std::endl;
        }
    }

}

bool ContentPipeline::getFileStream(std::string filePath, std::ostream& out)
{
    // GRF Reading
    for(uint16_t i = 0; i < this->grf_handles.size(); i++)
    {
        if(this->grf_handles.at(i)->fileExists(filePath))
        {
            this->grf_handles.at(i)->write(filePath, out);
            std::cout << filePath << " exists!" << std::endl;
            return true;
        }
    }

    // Data Folder reading
    std::ifstream iStream(filePath, std::ifstream::in|std::ifstream::binary);
    if(iStream.is_open())
    {
        int length = iStream.tellg();
        char buffer[length];
        iStream.read(buffer, length);
        out.write(buffer,length);
        std::cout << filePath << " exists!" << std::endl;
        return true;
    }

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
    std::cout << filePath << " doesn't exists!" << std::endl;
    return false;
}
