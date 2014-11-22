#ifndef _CGRF_H_
#define _CGRF_H_
#include <fstream>
#include <vector>
#include "../common/FileStream.h"

class CGrf {
	public:
		CGrf();
		CGrf(const std::string& sFile);
		virtual ~CGrf();
		uint8_t Open(const std::string& sFile);
		bool IsOpen() const;
		void Close();

		bool Exists(const std::string& sFile);
		bool GetContents(const std::string& sFile, FileStream &flstream);

	private:
		enum FLG
		{
			NONE = 0x00,
			FILE = 0x01,
			MIXCRYPT = 0x02,
			DES = 0x03,
			UNKN = 0x05
		};

		struct FileTableItem
		{
			char* sFile;//FileName
			uint32_t dwCompressed;//Lengths
			uint32_t dwCompressedAligned;//rounded to closest multiple of 4
			uint32_t dwUncompressed;
			uint8_t  uFlags;
			uint32_t dwOffset;
			uint32_t dwCycle;//needed for DES decoding
		};

		char pVersion[4];
		bool bOpen;
		std::ifstream stream;
		std::vector<CGrf::FileTableItem*> vItems;

		void getItem(CGrf::FileTableItem *pItem, std::stringstream& tblstream);
};
#endif//_CGRF_H_
