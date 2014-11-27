#ifndef _LUADB_H
#define _LUADB_H
#include <string>
#include <cstdarg>

extern "C"
{
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
}

class LuaDB
{
	public:
		LuaDB();
		virtual ~LuaDB();

		void GetHeadgearSuffix(int16_t shAccessory, char* sName);
		void GetRobeSuffix(int32_t lRobe, char* sName);
		void GetJobName(int16_t shJob, int16_t shGender, char* sName);
    void GetJobSuffix(int16_t shJob, char* sName);
    void GetMobName(int16_t shMob, char* sName);
		void GetHandSuffix(int16_t shJob, char* sName);
		void GetPaletteSuffix(int16_t shJob, char* sName);


		bool IsShrunk(int16_t shJob);
		int16_t GetCashMount(int16_t shJob);


	protected:

	private:
		lua_State *L;

		void loadFile(std::string sFile);
		void callFunc(const char* sFunc, const char* sFormat, ...);
};

#endif//_LUADB_H
