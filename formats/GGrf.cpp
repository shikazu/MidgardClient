#include "GGrf.h"

#include <fstream>
#include <sstream>
#include <zlib.h>

namespace DES {
// Borrowed from eAthena

//----------------------------
//	grf decode data table
//----------------------------
static unsigned char BitMaskTable[8] = {
	0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01
};

static char	BitSwapTable1[64] = {
	58, 50, 42, 34, 26, 18, 10,  2, 60, 52, 44, 36, 28, 20, 12,  4,
	62, 54, 46, 38, 30, 22, 14,  6, 64, 56, 48, 40, 32, 24, 16,  8,
	57, 49, 41, 33, 25, 17,  9,  1, 59, 51, 43, 35, 27, 19, 11,  3,
	61, 53, 45, 37, 29, 21, 13,  5, 63, 55, 47, 39, 31, 23, 15,  7
};
static char	BitSwapTable2[64] = {
	40,  8, 48, 16, 56, 24, 64, 32, 39,  7, 47, 15, 55, 23, 63, 31,
	38,  6, 46, 14, 54, 22, 62, 30, 37,  5, 45, 13, 53, 21, 61, 29,
	36,  4, 44, 12, 52, 20, 60, 28, 35,  3, 43, 11, 51, 19, 59, 27,
	34,  2, 42, 10, 50, 18, 58, 26, 33,  1, 41,  9, 49, 17, 57, 25
};
static char	BitSwapTable3[32] = {
	16,  7, 20, 21, 29, 12, 28, 17,  1, 15, 23, 26,  5, 18, 31, 10,
     2,  8, 24, 14, 32, 27,  3,  9, 19, 13, 30,  6, 22, 11,  4, 25
};

static unsigned char NibbleData[4][64]={
	{
		0xef, 0x03, 0x41, 0xfd, 0xd8, 0x74, 0x1e, 0x47,  0x26, 0xef, 0xfb, 0x22, 0xb3, 0xd8, 0x84, 0x1e,
		0x39, 0xac, 0xa7, 0x60, 0x62, 0xc1, 0xcd, 0xba,  0x5c, 0x96, 0x90, 0x59, 0x05, 0x3b, 0x7a, 0x85,
		0x40, 0xfd, 0x1e, 0xc8, 0xe7, 0x8a, 0x8b, 0x21,  0xda, 0x43, 0x64, 0x9f, 0x2d, 0x14, 0xb1, 0x72,
		0xf5, 0x5b, 0xc8, 0xb6, 0x9c, 0x37, 0x76, 0xec,  0x39, 0xa0, 0xa3, 0x05, 0x52, 0x6e, 0x0f, 0xd9,
	}, {
		0xa7, 0xdd, 0x0d, 0x78, 0x9e, 0x0b, 0xe3, 0x95,  0x60, 0x36, 0x36, 0x4f, 0xf9, 0x60, 0x5a, 0xa3,
		0x11, 0x24, 0xd2, 0x87, 0xc8, 0x52, 0x75, 0xec,  0xbb, 0xc1, 0x4c, 0xba, 0x24, 0xfe, 0x8f, 0x19,
		0xda, 0x13, 0x66, 0xaf, 0x49, 0xd0, 0x90, 0x06,  0x8c, 0x6a, 0xfb, 0x91, 0x37, 0x8d, 0x0d, 0x78,
		0xbf, 0x49, 0x11, 0xf4, 0x23, 0xe5, 0xce, 0x3b,  0x55, 0xbc, 0xa2, 0x57, 0xe8, 0x22, 0x74, 0xce,
	}, {
		0x2c, 0xea, 0xc1, 0xbf, 0x4a, 0x24, 0x1f, 0xc2,  0x79, 0x47, 0xa2, 0x7c, 0xb6, 0xd9, 0x68, 0x15,
		0x80, 0x56, 0x5d, 0x01, 0x33, 0xfd, 0xf4, 0xae,  0xde, 0x30, 0x07, 0x9b, 0xe5, 0x83, 0x9b, 0x68,
		0x49, 0xb4, 0x2e, 0x83, 0x1f, 0xc2, 0xb5, 0x7c,  0xa2, 0x19, 0xd8, 0xe5, 0x7c, 0x2f, 0x83, 0xda,
		0xf7, 0x6b, 0x90, 0xfe, 0xc4, 0x01, 0x5a, 0x97,  0x61, 0xa6, 0x3d, 0x40, 0x0b, 0x58, 0xe6, 0x3d,
	}, {
		0x4d, 0xd1, 0xb2, 0x0f, 0x28, 0xbd, 0xe4, 0x78,  0xf6, 0x4a, 0x0f, 0x93, 0x8b, 0x17, 0xd1, 0xa4,
		0x3a, 0xec, 0xc9, 0x35, 0x93, 0x56, 0x7e, 0xcb,  0x55, 0x20, 0xa0, 0xfe, 0x6c, 0x89, 0x17, 0x62,
		0x17, 0x62, 0x4b, 0xb1, 0xb4, 0xde, 0xd1, 0x87,  0xc9, 0x14, 0x3c, 0x4a, 0x7e, 0xa8, 0xe2, 0x7d,
		0xa0, 0x9f, 0xf6, 0x5c, 0x6a, 0x09, 0x8d, 0xf0,  0x0f, 0xe3, 0x53, 0x25, 0x95, 0x36, 0x28, 0xcb,
	}
};

void NibbleSwap(unsigned char* Src, int len) {
	for(;0<len;len--,Src++) {
		*Src = (*Src>>4) | (*Src<<4);
	}
}

void BitConvert(unsigned char* Src, char* BitSwapTable) {
	int lop,prm;
	unsigned char tmp[8];
	memset(tmp,0,8);
	for(lop=0;lop!=64;lop++) {
		prm = BitSwapTable[lop]-1;
		if (Src[(prm >> 3) & 7] & BitMaskTable[prm & 7]) {
			tmp[(lop >> 3) & 7] |= BitMaskTable[lop & 7];
		}
	}
	memcpy(Src,tmp,8);
}

static void BitConvert4(unsigned char* Src) {
	int lop,prm;
	unsigned char tmp[8];
	tmp[0] = ((Src[7]<<5) | (Src[4]>>3)) & 0x3f;	// ..0 vutsr
	tmp[1] = ((Src[4]<<1) | (Src[5]>>7)) & 0x3f;	// ..srqpo n
	tmp[2] = ((Src[4]<<5) | (Src[5]>>3)) & 0x3f;	// ..o nmlkj
	tmp[3] = ((Src[5]<<1) | (Src[6]>>7)) & 0x3f;	// ..kjihg f
	tmp[4] = ((Src[5]<<5) | (Src[6]>>3)) & 0x3f;	// ..g fedcb
	tmp[5] = ((Src[6]<<1) | (Src[7]>>7)) & 0x3f;	// ..cba98 7
	tmp[6] = ((Src[6]<<5) | (Src[7]>>3)) & 0x3f;	// ..8 76543
	tmp[7] = ((Src[7]<<1) | (Src[4]>>7)) & 0x3f;	// ..43210 v

	for(lop=0;lop!=4;lop++) {
		tmp[lop] = (NibbleData[lop][tmp[lop*2]] & 0xf0)
		         | (NibbleData[lop][tmp[lop*2+1]] & 0x0f);
	}

	memset(tmp+4,0,4);
	for(lop=0;lop!=32;lop++) {
		prm = BitSwapTable3[lop]-1;
		if (tmp[prm >> 3] & BitMaskTable[prm & 7]) {
			tmp[(lop >> 3) + 4] |= BitMaskTable[lop & 7];
		}
	}
	Src[0] ^= tmp[4];
	Src[1] ^= tmp[5];
	Src[2] ^= tmp[6];
	Src[3] ^= tmp[7];
}

void decode(unsigned char* buf, size_t len, int cycle) {
	size_t lop,cnt=0;
	int type = cycle == 0;
	if(cycle<3) cycle=3;
	else if(cycle<5) cycle++;
	else if(cycle<7) cycle+=9;
	else cycle+=15;

	for(lop=0; lop*8<len; lop++, buf+=8)
	{
		if(lop<20 || (type==0 && lop%cycle==0)) { // des
			BitConvert(buf,BitSwapTable1);
			BitConvert4(buf);
			BitConvert(buf,BitSwapTable2);
		} else {
			if(cnt==7 && type==0) {
				unsigned char a;
				unsigned char tmp[8];
				memcpy(tmp,buf,8);
				cnt=0;
				buf[0]=tmp[3];
				buf[1]=tmp[4];
				buf[2]=tmp[6];
				buf[3]=tmp[0];
				buf[4]=tmp[1];
				buf[5]=tmp[2];
				buf[6]=tmp[5];
				a=tmp[7];
				if(a==0x00) a=0x2b;
				else if(a==0x2b) a=0x00;
				else if(a==0x01) a=0x68;
				else if(a==0x68) a=0x01;
				else if(a==0x48) a=0x77;
				else if(a==0x77) a=0x48;
				else if(a==0x60) a=0xff;
				else if(a==0xff) a=0x60;
				else if(a==0x6c) a=0x80;
				else if(a==0x80) a=0x6c;
				else if(a==0xb9) a=0xc0;
				else if(a==0xc0) a=0xb9;
				else if(a==0xeb) a=0xfe;
				else if(a==0xfe) a=0xeb;
				buf[7]=a;
			}
			cnt++;
		}
	}
}

void decodeFilename(unsigned char* buf, size_t len)
{
	size_t lop;
	for(lop=0; lop*8<len; lop++, buf+=8)
	{
		NibbleSwap(buf, 8);
		BitConvert(buf,BitSwapTable1);
		BitConvert4(buf);
		BitConvert(buf,BitSwapTable2);
	}
}

} /* namespace DES */

GGrf::GGrf() {
	m_opened = false;
	memset((char*)&m_header, 0, sizeof(Header));
}

GGrf::~GGrf() {
	if (m_opened)
		close();
}

bool GGrf::open(const std::string& fn) {
	if (m_opened) {
		std::cerr << "Can't open GRF file. Object already in use. Close it first." << std::endl;
		return(false);
	}

	m_fp.open(fn.c_str(), std::ios_base::binary);
	if (!m_fp.is_open()) {
		//_log(ROINT__ERROR, "Error opening GRF file %s", fn.c_str());
		m_fp.clear(); // Clear bad flag bit
		return(false);
	}

	m_fp.read((char*)&m_header, sizeof(Header));
	m_filecount = m_header.number2 - m_header.number1 - 7;

	// Go to the starting offset to read the GRF index.
	m_fp.seekg(m_header.fileTableOffset, std::ios_base::cur);
	if (m_fp.eof()) {
		m_fp.close();
        //_log(ROINT__ERROR, "Error seeking to position %d", 46 + m_header.fileTableOffset);
		return(false);
	}

	std::stringstream ss;
	if ((m_header.version & 0xFF00) == 0x0100) {
		size_t cur = m_fp.tellg();
		size_t fileSize = (size_t)m_fp.seekg(0, std::ios::end).tellg();
		m_fp.seekg(cur, std::ios::beg);
		int size = fileSize - cur;
		unsigned char *body;
		body = new unsigned char[size];
		m_fp.read((char*)body, size);
		ss.write((char*)body, size);
		delete[] body;
		m_items = new FileTableItem_Ver1[m_filecount];
	}
	else if ((m_header.version & 0xFF00) == 0x0200) {
	m_fp.read((char*)&m_filetableheader, sizeof(unsigned int) * 2);
	m_filetableheader.body = new unsigned char[m_filetableheader.compressedLength];
	m_filetableheader.uncompressedBody = new unsigned char[m_filetableheader.uncompressedLength];
	m_fp.read((char*)m_filetableheader.body, m_filetableheader.compressedLength);
	unsigned long ul = m_filetableheader.uncompressedLength;

	uncompress(m_filetableheader.uncompressedBody, &ul, m_filetableheader.body, m_filetableheader.compressedLength);
	if (ul == 0) {
		//_log(ROINT__ERROR, "Cannot uncompress FileTableHeader");
		return(false);
	}
	if (ul != m_filetableheader.uncompressedLength) {
		//_log(ROINT__WARNING, "GRF Warning: Uncompressed lengths for FileTableHeader differ! (%d/%d)", m_filetableheader.uncompressedLength, ul);
	}

	ss.write((char*)m_filetableheader.uncompressedBody, m_filetableheader.uncompressedLength);

	// Delete unneeded data
	delete[] m_filetableheader.body;
	m_filetableheader.body = NULL;
	delete[] m_filetableheader.uncompressedBody;
	m_filetableheader.uncompressedBody = NULL;

		m_items = new FileTableItem_Ver2[m_filecount];
	}

	int i;
	for (i = 0; i < m_filecount; i++)
		m_items[i].readStream(ss);

	m_opened = true;
	//_log(ROINT__DEBUG, "GRF File %s successfully opened!", fn.c_str());
	return(true);
}

void GGrf::close() {
	if (!m_opened) {
		//_log(ROINT__ERROR, "GRF already closed");
		return;
	}

	//delete[] m_filetableheader.body;
	//delete[] m_filetableheader.uncompressedBody;
	delete[] m_items;
	m_items = NULL;
	if (m_fp.is_open())
		m_fp.close();
	m_opened = false;
}

bool GGrf::isOpen() const {
	return(m_opened);
}

bool GGrf::write(const std::string& s, std::ostream& out) {
	if (!m_opened)
		return(false);

	// search for file
	for (int i = 0; i < m_filecount; i++) {
		if (m_items[i].filename == s) {
			unsigned char *body;
			unsigned char *uncompressed;
			body = new unsigned char[m_items[i].compressedLengthAligned + 1024]; // 1K extra for safety
			uncompressed = new unsigned char[m_items[i].uncompressedLength + 1024]; // 1K extra for safety

			unsigned long ul;

			m_fp.seekg(m_items[i].offset + 46);
			m_fp.read((char*)body, m_items[i].compressedLengthAligned);
			if ((m_items[i].flags == 3) || (m_items[i].flags == 5) || ((m_header.version & 0xFF00) == 0x0100)) {
				// DES encoded. Let's decode!
				DES::decode(body, m_items[i].compressedLengthAligned, m_items[i].cycle);
			}

			ul = m_items[i].uncompressedLength;
			int r;
			if ((r = uncompress(uncompressed, &ul, body, m_items[i].compressedLengthAligned)) != Z_OK) {
				switch(r) {
					case Z_MEM_ERROR:
						//_log(ROINT__ERROR, "Error uncompressing data Z_MEM_ERROR");
						break;
					case Z_BUF_ERROR:
						//_log(ROINT__ERROR, "Error uncompressing data Z_BUF_ERROR");
						break;
					case Z_STREAM_ERROR:
						//_log(ROINT__ERROR, "Error uncompressing data Z_STREAM_ERROR");
						break;
					case Z_DATA_ERROR:
						//_log(ROINT__ERROR, "Error uncompressing data Z_DATA_ERROR");
						break;
					default:
						//_log(ROINT__ERROR, "Unknown error when uncompressing data: %d", r);
						break;
				}
				delete[] uncompressed;
				delete[] body;
				return(false);
			}

			//_logif(((long)ul != m_items[i].uncompressedLength), ROINT__ERROR, "GRF Warning: Uncompressed lengths for file %s differs!", m_items[i].filename)

			out.write((char*)uncompressed, ul);

			delete[] body;
			delete[] uncompressed;

			return(true);
		}
	}
	return(false);
}

bool GGrf::save(const std::string& s, const std::string& filename) {
	if (!m_opened)
		return(false);

	std::ofstream out(filename.c_str(), std::ios_base::out | std::ios_base::binary);
	bool r = write(s, out);
	out.close();
	return(r);
}

std::string GGrf::getFilename(const unsigned int& i) const {
	if (!m_opened)
		return("");

	if (i >= (unsigned int)m_filecount)
		return("");

	return(m_items[i].filename);
}

unsigned int GGrf::getCount() const {
	if (!m_opened)
		return(0);

	return(m_filecount);
}

bool GGrf::fileExists(const std::string& fn) const {
	for (int i = 0; i < m_filecount; i++)
		if (!strcmp(m_items[i].filename, fn.c_str()))
			return(true);
	return(false);
}

const GGrf::FileTableItem& GGrf::operator[] (const unsigned int& i) const {
	return(m_items[i]);
}

const GGrf::FileTableItem& GGrf::getItem(const unsigned int& i) const {
	return(m_items[i]);
}

GGrf::FileTableItem::FileTableItem() {
	filename = NULL;
	compressedLength = 0;
	compressedLengthAligned = 0;
	uncompressedLength = 0;
	flags = 0;
	offset = 0;
	cycle = 0;
}

GGrf::FileTableItem::FileTableItem(const FileTableItem& f) {
	if (f.filename != NULL) {
		filename = new char[strlen(f.filename) + 1];
		strcpy(filename, f.filename);
	}
	else {
		filename = NULL;
	}
	compressedLength = f.compressedLength;
	compressedLengthAligned = f.compressedLengthAligned;
	uncompressedLength = f.uncompressedLength;
	flags = f.flags;
	offset = f.offset;
	cycle = f.cycle;
}

GGrf::FileTableItem::~FileTableItem() {
	if (filename != NULL)
		delete[] filename;
}

bool GGrf::FileTableItem_Ver1::readStream(std::istream& ss) {
	char buf[256];
	int idx;

	unsigned int len;
	ss.read((char*)&len, sizeof(len));
	ss.seekg(2, std::ios::cur);
	ss.read(buf, len-6);
	DES::decodeFilename((unsigned char*)&buf[0], len);
	idx = len;

	cycle = 0;

	buf[idx] = 0;
	filename = new char[strlen(buf) + 1];
	strcpy(filename, buf);

	unsigned int len1, len2, len3;

	ss.seekg(4, std::ios::cur);

	ss.read((char*)&len1, sizeof(unsigned int));
	ss.read((char*)&len2, sizeof(unsigned int));
	ss.read((char*)&len3, sizeof(unsigned int));
	compressedLength = len1 - len3 - 0x2CB;
	compressedLengthAligned = len2 - 0x92CB;
	uncompressedLength = len3;
	ss.read(&flags, 1);
	ss.read((char*)&offset, sizeof(unsigned int));

	// Setup for decryption
	static const char *suffix[] = {".act", ".gat", ".gnd", ".str"};
	bool a = true;

	if (flags == 0) {
		flags = 2;
	}
	else {
		for (int i=0; i<4; i++) {
			if (strncmp(strrchr(filename, '.'), suffix[i], 4) == 0) {
				a = false;
				break;
			}
		}
		if (a) {
			int lop;
			int srccount;
			int srclen = compressedLength;
			for (lop = 10, srccount = 1; srclen >= lop; lop = lop * 10, srccount++);
			cycle = srccount;
		}
	}

	return(true);
}

bool GGrf::FileTableItem_Ver2::readStream(std::istream& ss) {
	char buf[256];
	int idx;
	char c;
	idx = 0;
	c = -1;
	while (c != 0) {
		ss.get(c);
		buf[idx++] = c;
	}

	cycle = 0;

	buf[idx] = 0;
	filename = new char[strlen(buf) + 1];
	strcpy(filename, buf);

	ss.read((char*)&compressedLength, sizeof(unsigned int));
	ss.read((char*)&compressedLengthAligned, sizeof(unsigned int));
	ss.read((char*)&uncompressedLength, sizeof(unsigned int));
	ss.read(&flags, 1);
	ss.read((char*)&offset, sizeof(unsigned int));

	// Setup for decryption
	if (flags == 3) {
		int lop;
		int srccount;
		int srclen = compressedLength;
		for (lop = 10, srccount = 1; srclen >= lop; lop = lop * 10, srccount++);
		cycle = srccount;
	}

	return(true);
}

GGrf::FileTableItem& GGrf::FileTableItem::operator = (const FileTableItem& f) {
	if (filename != NULL)
		delete[] filename;

	if (f.filename != NULL) {
		filename = new char[strlen(f.filename) + 1];
		strcpy(filename, f.filename);
	}
	else {
		filename = NULL;
	}
	filename = new char[strlen(f.filename) + 1];
	strcpy(filename, f.filename);
	compressedLength = f.compressedLength;
	compressedLengthAligned = f.compressedLengthAligned;
	uncompressedLength = f.uncompressedLength;
	flags = f.flags;
	offset = f.offset;
	cycle = f.cycle;

	return(*this);
}
