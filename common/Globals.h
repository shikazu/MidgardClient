#ifndef _GLOBALS_H
#define _GLOBALS_H
#include "ContentPipeline.h"
#include "FileStream.h"
#include "GameCursor.h"
#include "LuaDB.h"

///Constants
namespace S
{
	const char *const PLAYER = "data\\sprite\\인간족";
	const char *const EQUIP  = "data\\sprite\\악세사리";
	const char *const ITEM   = "data\\sprite\\아이템";
	const char *const MOB    = "data\\sprite\\몬스터";
	const char *const NPC    = "data\\sprite\\npc";

	const char *const HEAD = "머리통";
	const char *const BODY = "몸통";
	const char *const ROBE = "로브";
	const char *const SHLD = "방패";

	const char *const WOMAN = "여";
	const char *const MAN = "남";
}

///Global Pipeline Reference
ContentPipeline& GetPipe();

///Global Lua DB reference
LuaDB& GetDB();

///Get the Message String based on index
char* GetMsg(uint32_t dwIndex);

///Global Mouse Cursor Reference
GameCursor& GetMouseCursor();

///Get the Font by providing the Font Index
const sf::Font& GetFont(uint32_t dwFontID);

///Widget IDs

//--- Manager IDs ---//
#define MAIN_MANAGER 0//For Game Window
#define ERRR_MANAGER 1//For Error Windows

//--- MessageBox IDs---//
#define MSGBOX_FRAM 10
#define MSGBOX_PBTN 11
#define MSGBOX_NBTN 12

//--- LoginView IDs---//
#define LV_FRAME    100
#define LV_LOGINBTN 101
#define LV_EXITBTN  102
#define LV_USERIN   103
#define LV_PASSIN   104
#define LV_SAVECHK  105

//--- CharView IDs---//
#define CV_FRAME    106
#define CV_OKBTN    107
#define CV_CANCBTN  108
#define CV_DELBTN   109
#define CV_MAKEBTN  110
#define CV_CAN2BTN  111

#define CV_NAMELBL  112
#define CV_JOBLBL   113
#define CV_LVLLBL   114
#define CV_EXPLBL   115
#define CV_HPLBL    116
#define CV_SPLBL    117
#define CV_MAPLBL   118

#define CV_STRLBL   120
#define CV_AGILBL   121
#define CV_VITLBL   122
#define CV_INTLBL   123
#define CV_DEXLBL   124
#define CV_LUKLBL   125

#define CV_LEFTBTN  126
#define CV_RIGTBTN  127

#define CV_SPRFRAME 128//IDs 128-133 will be used for PlayerFrames

#endif//_GLOBALS_H
