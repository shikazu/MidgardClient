#include "Globals.h"
#include <cstring>
#include <iostream>

void *l_alloc (void *ud, void *ptr, size_t osize, size_t nsize)
{
  (void)ud;
  (void)osize;
  if (nsize == 0) {
    free(ptr);
    return NULL;
  }
  else
    return realloc(ptr, nsize);
}

LuaDB::LuaDB()
{
	L = lua_newstate(l_alloc, 0);
	luaL_openlibs(L);

	//Load Files in admin folder
	std::string sAdmin[] = {
		"PCIds",
		"PCNames",
		"PCPals",
		"PCPaths",
		"PCHands",
		"PCImfs",
		"PCFuncs",
		"pcidentity",
//		"pcjobname",//needed?
//		"pcjobnamegender",//needed?
	};
	uint32_t dwCount = sizeof(sAdmin)/sizeof(std::string);
	for (uint32_t i = 0; i < dwCount; i++)
	{
		loadFile("admin\\" + sAdmin[i]);
	}

	//Load files in datainfo folder
	std::string sDataInfo[] = {
		"accessoryid",
		"accname",
		"accname_f",
		"helpmsgstr",
		"npcidentity",
		"jobidentity",
		"jobname",
		"jobname_f",
		"kaframovemapservicelist",
		"kaframovemapservicelist_f",
		"npclocationradius",
		"npclocationradius_f",
//		"pcjobnamegender",//needed?
//		"pcjobnamegender_f",//needed?
		"petinfo",
		"shadowtable",
		"shadowtable_f",
		"spriterobeid",
		"spriterobename",
		"spriterobename_f",
		"weapontable",
		"weapontable_f",
	};
	dwCount = sizeof(sDataInfo)/sizeof(std::string);
	for (uint32_t i = 0; i < dwCount; i++)
	{
		loadFile("datainfo\\" + sDataInfo[i]);
	}

	//Rest to be added later
}

LuaDB::~LuaDB()
{
	lua_close(L);
}

void LuaDB::GetHeadgearSuffix(int16_t shAccessory, char* sName)
{
	callFunc("ReqAccName", "d>s", shAccessory, sName);
}

void LuaDB::GetRobeSuffix(int32_t lRobe, char* sName)
{
	callFunc("ReqRobSprName", "d>s", lRobe, sName);
}

void LuaDB::GetJobName(int16_t shJob, int16_t shGender, char* sName)
{
	int32_t lJob;
	callFunc("MapPCJobName", "dd>d", shGender, shJob, &lJob);
	if (lJob == -1)
	{
		lJob = shJob;
	}
	callFunc("ReqPCJobName", "dd>s", shGender, lJob, sName);
	if (*sName == 0 )
	{
		strcpy(sName, "Poring");//default name used by RO
	}
}

void LuaDB::GetJobSuffix(int16_t shJob, char* sName)
{
	int32_t lJob;
	callFunc("MapPCPath", "d>d", shJob, &lJob);
	if (lJob == -1)
	{
		lJob = shJob;
	}
	callFunc("ReqPCPath", "d>s", lJob, sName);
	if (*sName == 0)
	{
		callFunc("ReqPCPath", "d>s", 0, sName);//Novice
	}
}

void LuaDB::GetHandSuffix(int16_t shJob, char* sName)
{
	int32_t lJob;
	callFunc("MapPCHandPath", "d>d", shJob, &lJob);
	if (lJob == -1)
	{
		lJob = shJob;
	}
	callFunc("ReqPCHandPath", "d>s", lJob, sName);
	if (*sName == 0)
	{
		callFunc("ReqPCHandPath", "d>s", 0, sName);//Novice
	}
}

void LuaDB::GetPaletteSuffix(int16_t shJob, char* sName)
{
	int32_t lJob;
	callFunc("MapPCPal", "d>d", shJob, &lJob);
	if (lJob == -1)
	{
		lJob = shJob;
	}
	callFunc("ReqPCPal", "d>s", lJob, sName);
	if (*sName == 0)
	{
		callFunc("ReqPCPal", "d>s", 0, sName);//Novice
	}
}

void LuaDB::GetMobName(int16_t shMob, char* sName)
{
	callFunc("ReqJobName", "d>s", shMob, sName);
	if (*sName == 0)
	{
		callFunc("ReqJobName", "d>s", 1002, sName);//Poring
	}
}

bool LuaDB::IsShrunk(int16_t shJob)
{
	bool bOut;
	callFunc("IsDwarf", "d>b", shJob, &bOut);
	return bOut;
}

int16_t LuaDB::GetCashMount(int16_t shJob)
{
	int16_t shMount;
	callFunc("GetHalter", "d>d", shJob, &shMount);
	return shMount;
}

void LuaDB::loadFile(std::string sFile)
{
	FileStream stream;
	std::string sFullPath = "data\\luafiles514\\lua files\\" + sFile + ".lub";
	if (!GetPipe().getFileStream(sFullPath, stream))
	{
		sFullPath.pop_back();
		sFullPath.push_back('a');//change lub to lua
		if (!GetPipe().getFileStream(sFullPath, stream))
		{
			std::cout << "File '" << sFullPath.c_str() << "' not found!\n";
			return;
		}
	}
	uint32_t dwSize = stream.getSize();
	char sBuffer[dwSize];
	stream.read(sBuffer, dwSize);
	luaL_loadbuffer(L, sBuffer, dwSize, NULL);//sFile.c_str());
	std::cout << "File '" << sFile.c_str();
	if (lua_pcall(L, 0, 0, 0) != 0)
	{
		std::cout << "' has errors :\n" << lua_tostring(L, -1) << std::endl;
	}
	else
	{
		std::cout << "' loaded successfully \n";
	}
}

void LuaDB::callFunc(const char* sFunc, const char* sFormat, ...)
{
	va_list args;
	va_start(args, sFormat);

	//Get the Function
	lua_getglobal(L, sFunc);

	//Push the Inputs
	int32_t i = 0;
	while (*(sFormat+i) != '>' && *(sFormat+i) != '\0')
	{
		char c = *(sFormat+i);
		switch (c)
		{
			case 'b': {lua_pushboolean(L, va_arg(args, bool));    break;}
			case 'd': {lua_pushinteger(L, va_arg(args, int32_t)); break;}
			case 's': {lua_pushstring( L, va_arg(args, char*));   break;}
		}
		i++;
	}

	//Call the Function
  int32_t j = strlen(sFormat) - (i+1);
  int32_t s = lua_pcall(L, i, j, 0);
	if (s != 0)
	{
		std::cout << "Function Call " << sFunc << " Failed" << std::endl;
		std::cout << "\t" << lua_tostring(L, -1) << std::endl;
	}

	//Get the outputs
  while (j > 0)
	{
		i++;
    char c = *(sFormat + i);
    switch (c)
    {
    	case 'b': {*(va_arg(args, bool*))    = (s==0) ? lua_toboolean(L, -j) : false; break;}
    	case 'd': {*(va_arg(args, int32_t*)) = (s==0) ? lua_tointeger(L, -j) : -1   ; break;}
    	case 's': {
    		if (s==0)
				{
					strcpy(va_arg(args, char*), lua_tostring( L, -j));
				}
				else
				{
					*(va_arg(args, char*)) = 0;
				}
				break;
			}
    }
    j--;
	}
}
