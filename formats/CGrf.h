#ifndef _CGRF_H_
#define _CGRF_H_

#include <string>
#include <fstream>
#include <cstring>


/// Reads, decompresses and decrypts the grf structure and returns readable data.
/// This currently supports only GRF 2.0 version files
class CGrf {
public:
#pragma pack(push,1)
	/// Header common to all GRF files
	struct Header {
		char signature[16];
		unsigned char allowEncryption[14];
		unsigned int fileTableOffset;
		unsigned int number1;
		unsigned int number2;
		unsigned int version;
	};

	/// Holds a sequence of compressed (and uncompressed) FileTableItem structures
	struct FileTableHeader {
		unsigned int compressedLength;
		unsigned int uncompressedLength;
		unsigned char* body;
		unsigned char* uncompressedBody;
	};
#pragma pack(pop)

	class FileTableItem;

	/** Presents information on each file inside of the GRF */
	class FileTableItem {
	public:
		FileTableItem();
		FileTableItem(const FileTableItem&);
		~FileTableItem();
		virtual bool readStream(std::istream& ss) = 0;

		FileTableItem& operator = (const FileTableItem&);


		char* filename;
		int32_t compressedLength;
		int32_t compressedLengthAligned;
		int32_t uncompressedLength;
		/**
		 * Bitmask indicating what this file is
		 * <pre>
		 * 0x01 -> File
		 * 0x02 -> MIXCRYPT
		 * 0x03 -> DES
		 * 0x05 -> ??
		 * </pre>
		 *
		 * Source: OpenKore project
		 */
		char flags;
		int32_t offset;
		int32_t cycle; // for DES Decoding purposes
	};

	class FileTableItem_Ver1 : public FileTableItem {
	public:
		virtual bool readStream(std::istream& ss);
	};

	class FileTableItem_Ver2 : public FileTableItem {
	public:
		virtual bool readStream(std::istream& ss);
	};

protected:
	Header m_header;
	bool m_opened;
	int32_t m_filecount;
	std::ifstream m_fp;
	FileTableHeader m_filetableheader;

	/** List of files contained in the GRF */
	FileTableItem *m_items;

private:
	unsigned long m_seekstart;

public:
	CGrf();
	~CGrf();

	bool open(const std::string&);
	void close();

	bool isOpen() const;

	/** Reads a file from the GRF and writes into the stream. Returns false if error. */
	bool write(const std::string&, std::ostream&);

	/** Saves the file f into the file filename */
	bool save(const std::string& f, const std::string& filename);
	bool save(const std::string& f, const std::wstring& filename);

	bool fileExists(const std::string& fn) const;

	unsigned int getCount() const;
	std::string getFilename(const unsigned int& i) const;
	const FileTableItem& operator[] (const unsigned int& i) const;
	const FileTableItem& getItem(const unsigned int& i) const;
};

#endif//_CGRF_H_
