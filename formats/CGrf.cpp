#include "CGrf.h"
#include "../common/Des.h"
#include <cstring>
#include <sstream>
#include <zlib.h>

CGrf::CGrf()
{
	bOpen = false;
}

CGrf::CGrf(const std::string& sFile)
{
	Open(sFile);
}

CGrf::~CGrf()
{
	if (bOpen) Close();
}

uint8_t CGrf::Open(const std::string& sFile)
{
	if (bOpen) return 1;//Object already in use
	stream.open(sFile.c_str(), stream.binary | stream.in);
	if (!stream.is_open())
	{
		stream.clear();
		return 2;//File not opened
	}
	//Read the Header Entries
	//--Check Signature
	char pSig[16];
	stream.read(pSig, 16);
	if (strcmp(pSig, "Master of Magic") != 0)
	{
		stream.close();
		return 3;//Invalid File
	}
	//--Skip the Allow Encryption part for now -- will add it if needed
	stream.seekg(14, stream.cur);

	//--Get FT offset, number1, number2 and version
	uint32_t dwFileTableOffset;
	stream.read((char*)&dwFileTableOffset, 4);

	uint32_t dw1, dw2;
	stream.read((char*)&dw1, 4);
	stream.read((char*)&dw2, 4);
	uint32_t dwFileCount = dw2 - dw1 - 7;

	stream.read(pVersion, 4);

	//Now goto start of File Table and get the index
	stream.seekg(dwFileTableOffset, stream.cur);
	if (stream.eof())
	{
		stream.close();
		return 4;//No File Table - EOF reached
	}

	unsigned char *pBody;
	uint32_t dwBodySize;
	if (pVersion[1] == 0x01)//Version 0x01?? - Table header is not there
	{
		size_t curPos = stream.tellg();
		size_t fSize = (size_t)stream.seekg(0, stream.end).tellg();
		stream.seekg(curPos, stream.beg);

		dwBodySize = fSize - curPos;
		pBody = new unsigned char[dwBodySize];
		stream.read((char*)pBody, dwBodySize);
	}
	else if(pVersion[1] == 0x02)//Version 0x02?? - Table Header is present and table items are compressed.
	{
		uint32_t dwCompressed;//Length
		stream.read((char*)&dwCompressed, 4);
		stream.read((char*)&dwBodySize, 4);

		unsigned char* pCompressed = new unsigned char[dwCompressed];
		stream.read((char*)pCompressed, dwCompressed);

		pBody = new unsigned char[dwBodySize];
		unsigned long ulBodySize = dwBodySize;
		uncompress(pBody, &ulBodySize, pCompressed, dwCompressed);

		delete[] pCompressed;
		if (ulBodySize == 0 || ulBodySize != dwBodySize)
		{
			delete[] pBody;
			stream.close();
			return 5;//Failed to uncompress header
		}
	}
	if (pBody == nullptr)
	{
		stream.close();
		return 6;//Unknown version
	}

	//Now put the table body into a stringstream for easy access
	std::stringstream tblstream;
	tblstream.write((char*)pBody, dwBodySize);
	delete[] pBody;//no longer needed

	//Get all the FileTableItems and put them into the vector
	vItems.reserve(dwFileCount);
	for (uint32_t i = 0; i < dwFileCount; i++)
	{
		FileTableItem* pItem = new FileTableItem;
		getItem(pItem, tblstream);
		vItems.push_back(pItem);
	}
	//And we are done
	bOpen = true;
	return 0;
}

void CGrf::getItem(CGrf::FileTableItem *pItem, std::stringstream& tblstream)
{
	if (pVersion[1] == 0x01)
	{
		char pBuffer[256];
		uint32_t dwLength;//includes the size of the length and 2 skipped bytes
		tblstream.read((char*)&dwLength, 4);
		tblstream.seekg(2, tblstream.cur);
		tblstream.read(pBuffer, dwLength-6);
		DES::DecodeFilename((unsigned char*)&pBuffer[0], dwLength);

		pItem->dwCycle = 0;
		pBuffer[dwLength] = 0;
		pItem->sFile = new char[strlen(pBuffer) + 1];
		strcpy(pItem->sFile, pBuffer);

		uint32_t dw1, dw2, dw3;
		tblstream.seekg(4, tblstream.cur);
		tblstream.read((char*)&dw1, 4);
		tblstream.read((char*)&dw2, 4);
		tblstream.read((char*)&dw3, 4);

		pItem->dwCompressed  = dw1 - dw3 - 0x02CB;
		pItem->dwCompressedAligned = dw2 - 0x92CB;
		pItem->dwUncompressed = dw3;

		tblstream.read((char*)&(pItem->uFlags), 1);
		tblstream.read((char*)&(pItem->dwOffset), 4);

		//Setup For Decryption
		static const char *pSuffix[] = {".act", ".gat", ".gnd", ".str"};
		if (pItem->uFlags == NONE)
		{
			pItem->uFlags = MIXCRYPT;
		}
		else
		{
			bool a = true;
			for (uint8_t i = 0; i < 4; i++)
			{
				if (strcmp(strrchr(pItem->sFile, '.'), pSuffix[i]) == 0) {
					a = false;
					break;
				}
			}
			if (a)
			{
				uint32_t dwCount = 1, dwLength = pItem->dwCompressed, lop = 10;
				for (; dwLength >= lop; lop *= 10, dwCount++);
				pItem->dwCycle = dwCount;
			}
		}
	}
	else if (pVersion[1] == 0x02)
	{
		char pBuffer[256];
		uint32_t dwIndex = 0;
		do //Variable Length so we need this way
		{
			pBuffer[dwIndex++] = tblstream.get();
		}while (pBuffer[dwIndex-1] != '\0');

		pItem->dwCycle = 0;
		pItem->sFile = new char[strlen(pBuffer) + 1];
		strcpy(pItem->sFile, pBuffer);

		tblstream.read((char*)&(pItem->dwCompressed), 4);
		tblstream.read((char*)&(pItem->dwCompressedAligned), 4);
		tblstream.read((char*)&(pItem->dwUncompressed), 4);
		tblstream.read((char*)&(pItem->uFlags), 1);
		tblstream.read((char*)&(pItem->dwOffset), 4);

		//Setup for decryption
		if (pItem->uFlags == DES)
		{
			uint32_t dwCount = 1, dwLength = pItem->dwCompressed, lop = 10;
			for (; dwLength >= lop; lop *= 10, dwCount++);
			pItem->dwCycle = dwCount;
		}
	}
}

bool CGrf::Exists(const std::string &sFile)
{
	for( uint32_t i = 0; i < vItems.size(); i++)
	{
		if (strcmp(vItems.at(i)->sFile, sFile.c_str()) == 0)
		{
			return true;
		}
	}
	return false;
}

bool CGrf::GetContents(const std::string &sFile, FileStream &flstream)
{
	if (!bOpen) return false;

	//Search for File
	for (uint32_t i = 0; i < vItems.size(); i++)
	{
		if (strcmp(vItems.at(i)->sFile, sFile.c_str()) != 0) continue;
		FileTableItem* pItem = vItems.at(i);

		unsigned char *pCompressed   = new unsigned char[pItem->dwCompressedAligned + 1024];
		unsigned char *pUncompressed = new unsigned char[pItem->dwUncompressed + 1024];

		stream.seekg(pItem->dwOffset + 46);
		stream.read((char*)pCompressed, pItem->dwCompressedAligned);
		if (pItem->uFlags == DES || pItem->uFlags == UNKN || pVersion[1] == 0x01)
		{   //DES encoded data
			DES::Decode(pCompressed, pItem->dwCompressedAligned, pItem->dwCycle);
		}
		unsigned long ulUnComp = pItem->dwUncompressed;
		int32_t r = uncompress(pUncompressed, &ulUnComp, pCompressed, pItem->dwCompressedAligned);
		if (r != Z_OK || ulUnComp != pItem->dwUncompressed)
		{
			//Report Error based on value of r
			delete[] pUncompressed;
			delete[] pCompressed;
			return false;
		}
		flstream.load((char*)pUncompressed, ulUnComp);
		delete[] pUncompressed;
		delete[] pCompressed;
		return true;
	}
	return false;
}
void CGrf::Close()
{
	stream.close();
	bOpen = false;
}

bool CGrf::IsOpen() const
{
	return bOpen;
}
