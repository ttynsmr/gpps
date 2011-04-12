#include <windows.h>
#include <shlwapi.h>
#include <string>
#include "luna.h"
#include "CLua.h"
#include "../gpp_common/gritterpanel.h"

#pragma comment( lib, "shlwapi.lib" )

static CBitImage image;

//=============================================================================
class CLuaGppGraphics
{
public:
	CLuaGppGraphics(lua_State *L)
	{
	}

	~CLuaGppGraphics()
	{
	}

	// call from lua functions

	int SetPixel(lua_State *L)
	{
		int x = (int)lua_tointeger( L, -3 );
		int y = (int)lua_tointeger( L, -2 );
		bool bit = (int)lua_tointeger( L, -1 ) != 0;
		image.setPixel( x, y, bit );
		return 0;
	}

	int SetLine(lua_State *L)
	{
		int x = (int)lua_tointeger( L, -5 );
		int y = (int)lua_tointeger( L, -4 );
		int x2 = (int)lua_tointeger( L, -3 );
		int y2 = (int)lua_tointeger( L, -2 );
		bool bit = (int)lua_tointeger( L, -1 ) != 0;
		image.setLine( x, y, x2, y2, bit );
		return 0;
	}

	int Clear(lua_State *L)
	{
		image.clear();
		return 0;
	}

	int Debug(lua_State *L)
	{
		std::string strText( lua_tolstring( L, -1, NULL ) );
		OutputDebugString( strText.c_str() );
		return 0;
	}

	static const char className[];
	static const Luna<CLuaGppGraphics>::RegType Register[];
private:
};

//=============================================================================
const char CLuaGppGraphics::className[] = "CLuaGppGraphics";
const Luna<CLuaGppGraphics>::RegType CLuaGppGraphics::Register[] = {
	{ "SetPixel",		&CLuaGppGraphics::SetPixel		},
	{ "SetLine",		&CLuaGppGraphics::SetLine		},
	{ "Clear",			&CLuaGppGraphics::Clear			},
	{ "Debug",			&CLuaGppGraphics::Debug			},
	{ 0 }
};

class CLuaGpp : public CLuaBase
{
public:
	CLuaGpp() : CLuaBase()
	{
		luaL_openlibs( m_pLuaState );
		Luna<CLuaGppGraphics>::Register( m_pLuaState );
		if( PathFileExists(".\\gpp_lua.lua") )
		{
			OutputDebugString("exists\n");
		}
		load( std::string(".\\gpp_lua.lua") );
		lua_pcall( m_pLuaState, 0, 0, 0 );
	}
	virtual ~CLuaGpp(){}

	int GppDraw( unsigned char* pImg, int mode, int unknown1, int unknown2, int unknown3 )
	{
		lua_getglobal( m_pLuaState, "GppDraw" );
		if(!lua_isfunction(m_pLuaState, -1)){
			lua_pop(m_pLuaState, 1);
			return -1;
		}
		lua_call( m_pLuaState, 0, 0 );
		return 0;
	}

	// args: string buffer pointer, string buffer pointer size
	int GppInfo( char* pszDest, int iDestSize )
	{
		strncpy_s( pszDest, iDestSize, "LUA", iDestSize );
		return 0;
	}

	// args: unknown, unknown
	int GppCreate( int unknown1, int unknown2 )
	{
		lua_getglobal( m_pLuaState, "GppCreate" );
		if(!lua_isfunction(m_pLuaState, -1)){
			lua_pop(m_pLuaState, 1);
			return -1;
		}
		lua_call( m_pLuaState, 0, 0 );
		return 0;
	}

	int GppRelease(void)
	{
		lua_getglobal( m_pLuaState, "GppRelease" );
		if(!lua_isfunction(m_pLuaState, -1)){
			lua_pop(m_pLuaState, 1);
			return -1;
		}
		lua_call( m_pLuaState, 0, 0 );
		return 0;
	}

	int GppStart(void)
	{
		lua_getglobal( m_pLuaState, "GppStart" );
		if(!lua_isfunction(m_pLuaState, -1)){
			lua_pop(m_pLuaState, 1);
			return -1;
		}
		lua_call( m_pLuaState, 0, 0 );
		return 0;
	}

	int GppStop(void)
	{
		lua_getglobal( m_pLuaState, "GppStop" );
		if(!lua_isfunction(m_pLuaState, -1)){
			lua_pop(m_pLuaState, 1);
			return -1;
		}
		lua_call( m_pLuaState, 0, 0 );
		return 0;
	}

	int GppWorkFrame(void)
	{
		lua_getglobal( m_pLuaState, "GppWorkFrame" );
		if(!lua_isfunction(m_pLuaState, -1)){
			lua_pop(m_pLuaState, 1);
			return -1;
		}
		lua_call( m_pLuaState, 0, 0 );
		return 0;
	}
};

static CLuaGpp* pgpplua = NULL;

extern "C" {

// args: bitmap pointer, mode, unknown, unknown, unknown
int __stdcall GppDraw( unsigned char* pImg, int mode, int unknown1, int unknown2, int unknown3 )
{
	pgpplua->GppDraw( pImg, mode, unknown1, unknown2, unknown3 );

	switch( mode )
	{
	case 0:
		image.copyTo( pImg, 0 );
		break;
	case 1:
		image.copyTo( pImg, 1 );
		break;
	case 2:
		image.copyTo( pImg, 2 );
		break;
	}

	return 0;
}

// args: string buffer pointer, string buffer pointer size
int __stdcall GppInfo( char* pszDest, int iDestSize )
{
	strncpy_s( pszDest, iDestSize, "Lua plugin", iDestSize );
	return 0;
}

// args: unknown, unknown
int __stdcall GppCreate( int unknown1, int unknown2 )
{
	pgpplua = new CLuaGpp();
	return pgpplua->GppCreate( unknown1, unknown2 );
}

int __stdcall GppRelease(void)
{
	return pgpplua->GppRelease();
}

int __stdcall GppStart(void)
{
	return pgpplua->GppStart();
}

int __stdcall GppStop(void)
{
	return pgpplua->GppStop();
}

int __stdcall GppWorkFrame(void)
{
	return pgpplua->GppWorkFrame();
}

}
