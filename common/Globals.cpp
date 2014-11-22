#include "Globals.h"

ContentPipeline& GetPipe()
{
	static ContentPipeline cpipe("data.ini");
	return cpipe;
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
	if (bInit == false)
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

GameCursor& GetMouseCursor()
{
	static GameCursor GCursor;
	return GCursor;
}
