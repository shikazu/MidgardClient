#include "FileStream.h"
#include <fstream>

FileStream::FileStream()
{
    pStream = new std::stringstream();
}

FileStream::~FileStream()
{
    delete pStream;
}

sf::Int64 FileStream::read(void *data, sf::Int64 size)
{
    return pStream->read((char*)data, size).gcount();
}

sf::Int64 FileStream::seek(sf::Int64 position)
{
    pStream->seekg(position, pStream->beg);
    return pStream->tellg();
}

sf::Int64 FileStream::tell()
{
    return pStream->tellg();
}

sf::Int64 FileStream::getSize()
{
    int32_t lCurPos = pStream->tellg();//Save Position
    pStream->seekg(0, pStream->end);
    sf::Int64 size =  pStream->tellg();
    pStream->seekg(lCurPos, pStream->beg);//Restore Position
    return size;
}

void FileStream::seek(sf::Int64 offset, uint8_t from)
{
    std::stringstream::seekdir dir;
    switch(from)
    {
        case BEG: {dir = pStream->beg; break;}
        case CUR: {dir = pStream->cur; break;}
        case END: {dir = pStream->end; break;}
        default: return;
    }
    pStream->seekg(offset, dir);
}

uint8_t FileStream::readByte()
{
    return pStream->get();
}

bool FileStream::load(std::string sFile)
{
    std::ifstream flstream;
    flstream.open(sFile.c_str(), flstream.in | flstream.binary);
    if (!flstream.is_open())
    {
        return false;
    }

    flstream.seekg(0, flstream.end);
    uint32_t dwLength = flstream.tellg();
    if (dwLength == 0)
    {
        flstream.close();
        return true;
    }
    flstream.seekg(0, flstream.beg);

    char sBuffer[dwLength];
    flstream.read(sBuffer, dwLength);//Read from file
		clear();//Free up the stream
    pStream->write(sBuffer, dwLength);//and Fill into stream
    flstream.close();

    return true;
}

void FileStream::load(char* data, uint32_t dwLength)
{
    clear();//free up the stream
    pStream->write(data, dwLength);
}

bool FileStream::atEnd()
{
	return (pStream->tellg() == getSize());
}

void FileStream::clear()
{
    delete pStream;
    pStream = new std::stringstream();
}
