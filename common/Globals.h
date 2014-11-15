#ifndef _GLOBALS_H
#define _GLOBALS_H
#include "ContentPipeline.h"
#include "FileStream.h"

///Reserved IDs for Widgets + Manager
#define MAIN_MANAGER 0//For Game Window
#define ERRR_MANAGER 1//For Error Windows

#define MSGBOX_FRAM 10
#define MSGBOX_PBTN 11
#define MSGBOX_NBTN 12

///User Widgets (Not manager) should start from following value - dont use reserved ones.
#define USER_CUSTOM 30

static ContentPipeline *DataPipe;

const sf::Font& GetFont(uint32_t dwFontID);

#endif//_GLOBALS_H
