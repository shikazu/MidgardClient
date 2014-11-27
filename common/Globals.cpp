#include "Globals.h"

ContentPipeline& GetPipe()
{
	static ContentPipeline cpipe("data.ini");
	return cpipe;
}

LuaDB& GetDB()
{
	static LuaDB luaint;
	return luaint;
}

char* GetMsg(uint32_t dwIndex)
{
	static int32_t aEndOffsets[2540];
	static FileStream stream;
	static bool bInit = false;

	if (!bInit)
	{
		GetPipe().getFileStream("data\\msgstringtable.txt", stream);
    for (int32_t i = 0; i < 2540; i++)
		{
      if (stream.atEnd()) break;
      while (stream.readByte() != '#' && !stream.atEnd());
			aEndOffsets[i] = stream.tell();
		}
    stream.seek(0);
		bInit = true;
	}

	int32_t lStart = 0;
	if (dwIndex != 0)
	{
		lStart = aEndOffsets[dwIndex-1];
	}
	int32_t lSize = aEndOffsets[dwIndex] - 1 - lStart;
	char *pMsg = new char[lSize+1];
	if (lSize != 0)
	{
		stream.seek(lStart);
		char c = stream.readByte();
		while (c == '\r' || c == '\n')//skip preceding form feed and carriage return
		{
			c = stream.readByte();
			lSize--;
		}
		stream.seek(-1, stream.CUR);
		stream.read(pMsg, lSize);
		stream.seek(0);
	}
	pMsg[lSize] = 0;
	return pMsg;//make sure to delete after use
}

GameCursor& GetMouseCursor()
{
	static GameCursor GCursor;
	return GCursor;
}

///Max Number of Fonts available
#define FONTCOUNT 2

struct FontData
{
	sf::Font font;
	FileStream stream;
};

const sf::Font& GetFont(uint32_t dwFontID)
{
	static FontData pFonts[FONTCOUNT];
	static bool bInit = false;
	if (!bInit)
	{
		std::string sFont[FONTCOUNT] = {"GistLight.otf", "AlexandriaFLF.ttf"};
		for (uint32_t i = 0; i < FONTCOUNT; i++)
		{
			if (!GetPipe().getFileStream(sFont[i], pFonts[i].stream))
			{
				continue;
			}
			if (!pFonts[i].font.loadFromStream(pFonts[i].stream))
			{
				continue;
			}
		}
		bInit = true;
	}
	return pFonts[dwFontID].font;
}
