#ifndef _FILESTREAM_H
#define _FILESTREAM_H

#include <SFML/System.hpp>
#include <sstream>

class FileStream : public sf::InputStream
{
    public:
        enum
        {
            BEG = 0,
            CUR,
            END
        };

        FileStream();
        virtual ~FileStream();

        sf::Int64 read(void *data, sf::Int64 size);
        sf::Int64 seek(sf::Int64 position);
        sf::Int64 tell();
        sf::Int64 getSize();

        bool load(std::string sFile);
        void load(char* data, uint32_t dwLength);
        void seek(sf::Int64 offset, uint8_t from);
        uint8_t readByte();
        bool atEnd();
        //void readColor(sf::Color& color);

    private:
			std::stringstream *pStream;

			void clear();
};

#endif//_FILESTREAM_H
