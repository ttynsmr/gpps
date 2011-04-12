#pragma once

class CLuaBase
{
public:
	CLuaBase();
	virtual ~CLuaBase();

	bool load( const std::string& strFilename );

	int CLuaBase::getIntegerVariable( const std::string& strVariableName );
	float CLuaBase::getRealVariable( const std::string& strVariableName );
	std::string CLuaBase::getString( const std::string& strVariableName );

protected:
	lua_State*		m_pLuaState;
};
