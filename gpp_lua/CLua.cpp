#include <string>
#include <lua.hpp>
#include "CLua.h"

#pragma comment( lib, "lua5/lua5.1.lib" )

//=============================================================================
CLuaBase::CLuaBase()
{
	m_pLuaState = luaL_newstate();
}

//=============================================================================
CLuaBase::~CLuaBase()
{
	lua_close( m_pLuaState );
}

//=============================================================================
bool CLuaBase::load( const std::string& strFilename )
{
	bool bResult = luaL_loadfile( m_pLuaState, strFilename.c_str() ) == 0;
	if( bResult == false )
	{
		return false;
	}

	return bResult;
}

//=============================================================================
int CLuaBase::getIntegerVariable( const std::string& strVariableName )
{
	lua_settop( m_pLuaState, 1 );
	lua_getglobal( m_pLuaState, strVariableName.c_str() );
	return (int)lua_tonumber( m_pLuaState, 2 );
}

//=============================================================================
float CLuaBase::getRealVariable( const std::string& strVariableName )
{
	lua_settop( m_pLuaState, 1 );
	lua_getglobal( m_pLuaState, strVariableName.c_str() );
	return (float)lua_tonumber( m_pLuaState, 2 );
}

//=============================================================================
std::string CLuaBase::getString( const std::string& strVariableName )
{
	lua_settop( m_pLuaState, 1 );
	lua_getglobal( m_pLuaState, strVariableName.c_str() );
	return std::string( lua_tostring( m_pLuaState, 2 ) );
}
