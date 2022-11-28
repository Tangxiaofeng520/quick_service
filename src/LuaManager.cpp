//
// Created by diandian on 11/21/22.
//

#include <cstring>
#include "LuaManager.h"

CLuaHelper::CLuaHelper()
{
    m_pLuaState = NULL;
}

CLuaHelper::~CLuaHelper()
{
    //因为CLuaHelper是帮助类，所以这个指针必须在对象析构前要调用Deattch,
    //而且m_pLuaState赋值也只能是通过Attach来做;

    m_pLuaState = NULL;
}


bool CLuaHelper::Attach(lua_State* L)
{
    m_pLuaState = L;

    return true;
}

bool CLuaHelper::Deattch()
{
    m_pLuaState = NULL;

    return true;
}

bool CLuaHelper::LoadAllLua(const char* pszDir)
{
    std::vector<std::string> vtFiles;

    //CommonFunc::GetDirFiles(pszDir, "*.lua", vtFiles, true);

    return LoadScriptFile(vtFiles);
}

bool CLuaHelper::LoadAllLua(std::string strDir)
{
    return LoadAllLua(strDir.c_str());
}

lua_State* CLuaHelper::GetLuaState()
{
    return m_pLuaState;
}

int CLuaHelper::GetGlobalVarInt( const char* pszVarName )
{
    //ERROR_RETURN_NULL(m_pLuaState != NULL);

    lua_getglobal(m_pLuaState, pszVarName);

    if(!lua_isnumber(m_pLuaState, -1))
    {
        lua_settop(m_pLuaState, 0);

        return 0;
    }

    return (int )lua_tointeger(m_pLuaState, -1);
}

bool CLuaHelper::GetGlobalVarboolean( const char* pszVarName )
{
    ////ERROR_RETURN_FALSE(m_pLuaState != NULL);

    lua_getglobal(m_pLuaState, pszVarName);

    if(!lua_isboolean(m_pLuaState, -1))
    {
        lua_settop(m_pLuaState, 0);

        return 0;
    }

    return lua_toboolean(m_pLuaState, -1);
}

const char * CLuaHelper::GetGlobalVarString( const char* pszVarName )
{
    //ERROR_RETURN_NULL(m_pLuaState != NULL);

    lua_getglobal(m_pLuaState, pszVarName);

    if(!lua_isstring(m_pLuaState, -1))
    {
        lua_settop(m_pLuaState, 0);

        return 0;
    }

    return lua_tostring(m_pLuaState, -1);
}

double CLuaHelper::GetGlobalVardouble( const char* pszVarName )
{
    //ERROR_RETURN_NULL(m_pLuaState != NULL);

    lua_getglobal(m_pLuaState, pszVarName);

    if(!lua_isnumber(m_pLuaState, -1))
    {
        lua_settop(m_pLuaState, 0);

        return 0;
    }

    return lua_tonumber(m_pLuaState, -1);
}


bool CLuaHelper::GetStackValue_Ptr(int nStackIndex, void*& ptrValue)
{
    if(!lua_isuserdata(m_pLuaState, nStackIndex))
    {
        return false;
    }

    ptrValue = lua_touserdata(m_pLuaState, nStackIndex);

    return true;
}

bool CLuaHelper::GetStackValue_Int(int nStackIndex, int & intValue)
{
    if(!lua_isnumber(m_pLuaState, nStackIndex))
    {
        return false;
    }

    intValue = (int )lua_tointeger(m_pLuaState, nStackIndex);

    return true;
}

bool CLuaHelper::GetStackValue_double(int nStackIndex, double& doubleValue)
{
    if(!lua_isnumber(m_pLuaState, nStackIndex))
    {
        return false;
    }

    doubleValue = lua_tonumber(m_pLuaState, nStackIndex);

    return true;
}

bool CLuaHelper::GetStackValue_String(int nStackIndex, const char *& strValue)
{
    if(!lua_isstring(m_pLuaState, nStackIndex))
    {
        return false;
    }

    strValue = lua_tostring(m_pLuaState, nStackIndex);

    return true;
}

bool CLuaHelper::CallLuaFunction( std::string strFuncName, char* pStrParamSig, ... )
{
    ////ERROR_RETURN_FALSE(pStrParamSig != NULL);
    char* pInParam = pStrParamSig;
    char* pOutParam = strchr(pStrParamSig, '=');
    ////ERROR_RETURN_FALSE(pOutParam != NULL);

    int nStackTop = lua_gettop(m_pLuaState);
    pOutParam += 1;

    va_list VarList;
    va_start(VarList, pStrParamSig);

    lua_getglobal(m_pLuaState, strFuncName.c_str());

    if(!lua_isfunction(m_pLuaState, -1))
    {
        va_end(VarList);
        lua_settop(m_pLuaState, nStackTop);
        return false;
    }

    bool	bInParamEnd		= false;
    int		nInParamCount	= 0;
    int		nOutParamCount  = (int)strlen(pOutParam);

    while(!bInParamEnd)
    {
        luaL_checkstack(m_pLuaState, 1, "too many arguments");
        char cParmSig = *pInParam;
        switch(cParmSig)
        {
            case 'd':
            {
                lua_pushnumber(m_pLuaState, va_arg(VarList, double));
                nInParamCount++;
            }
                break;
            case 'f':
            {
                lua_pushnumber(m_pLuaState, va_arg(VarList, float));
                nInParamCount++;
            }
                break;
            case 'i':
            {
                lua_pushinteger(m_pLuaState, va_arg(VarList, int));
                nInParamCount++;
            }
                break;
            case 'l':
            {
                lua_pushinteger(m_pLuaState, va_arg(VarList, long long));
                nInParamCount++;
            }
                break;
            case 's':
            {
                lua_pushstring(m_pLuaState, va_arg(VarList, char*));
                nInParamCount++;
            }
                break;
            case 'p':
            {
                lua_pushlightuserdata(m_pLuaState, va_arg(VarList, void*));
                nInParamCount++;
            }
                break;
            case 'b':
            {
                lua_pushboolean(m_pLuaState, va_arg(VarList, bool));
                nInParamCount++;
            }
                break;
            case 'x':
            {
                char * pData = va_arg(VarList, char*);
                int nLen = va_arg(VarList, int);
                lua_pushlstring(m_pLuaState, pData, nLen);
                nInParamCount++;
            }
                break;
            case '=':
            {
                bInParamEnd = true;
            }
                break;
            default:
            {

            }
                break;
        }

        pInParam++;
    }

    if(lua_pcall(m_pLuaState, nInParamCount, nOutParamCount, 0) != 0)
    {
        const char* sresult = lua_tostring(m_pLuaState, -1);
        va_end(VarList);
        lua_settop(m_pLuaState, nStackTop);
        return false;
    }

    int nRetIndex = -nOutParamCount;
    bool bOutParamEnd = false;
    while(!bOutParamEnd)
    {
        char cParmSig = *pOutParam;
        switch(cParmSig)
        {
            case 'd':
            {
                ////ERROR_RETURN_FALSE(lua_isnumber(m_pLuaState, nRetIndex));
                *va_arg(VarList, double*) = lua_tonumber(m_pLuaState, nRetIndex);
            }
                break;
            case 'f':
            {
                ////ERROR_RETURN_FALSE(lua_isnumber(m_pLuaState, nRetIndex));
                *va_arg(VarList, float*) = (float)lua_tonumber(m_pLuaState, nRetIndex);
            }
                break;
            case 'i':
            {
                ////ERROR_RETURN_FALSE(lua_isinteger(m_pLuaState, nRetIndex));
                *va_arg(VarList, int*) = (int)lua_tointeger(m_pLuaState, nRetIndex);
            }
                break;
            case 'l':
            {
                ////ERROR_RETURN_FALSE(lua_isinteger(m_pLuaState, nRetIndex));
                *va_arg(VarList, long long*) = (long long)lua_tointeger(m_pLuaState, nRetIndex);
            }
                break;
            case 's':
            {
                ////ERROR_RETURN_FALSE(lua_isstring(m_pLuaState, nRetIndex));
                *va_arg(VarList, const char**) = lua_tostring(m_pLuaState, nRetIndex);
            }
                break;
            case 'p':
            {
                ////ERROR_RETURN_FALSE(lua_isuserdata(m_pLuaState, nRetIndex));
                *va_arg(VarList, void**) = lua_touserdata(m_pLuaState, nRetIndex);
            }
                break;
            case 'b':
            {
                ////ERROR_RETURN_FALSE(lua_isboolean(m_pLuaState, nRetIndex));
                *va_arg(VarList, bool*) = lua_toboolean(m_pLuaState, nRetIndex);
            }
                break;
            case 0:
                bOutParamEnd = true;
                break;
            default:
            {

            }
                break;
        }

        nRetIndex++;
        pOutParam++;
    }

    va_end(VarList);
    lua_settop(m_pLuaState, nStackTop);

    return true;
}


bool CLuaHelper::CallLuaTableFunc(std::string strTableName, std::string strFuncName, char* pStrParamSig, ...)
{
    ////ERROR_RETURN_FALSE(pStrParamSig != NULL);
    char* pInParam = pStrParamSig;
    char* pOutParam = strchr(pStrParamSig, '=');
    ////ERROR_RETURN_FALSE(pOutParam != NULL);

    int nStackTop = lua_gettop(m_pLuaState);
    pOutParam += 1;

    va_list VarList;
    va_start(VarList, pStrParamSig);

    lua_getglobal(m_pLuaState, strTableName.c_str());
    if (!lua_istable(m_pLuaState, -1))
    {
        va_end(VarList);
        lua_settop(m_pLuaState, nStackTop);
        return false;
    }

    lua_getfield(m_pLuaState, -1, strFuncName.c_str());
    if (!lua_isfunction(m_pLuaState, -1))
    {
        va_end(VarList);
        lua_settop(m_pLuaState, nStackTop);
        return false;
    }

    bool	bInParamEnd = false;
    int		nInParamCount = 0;
    int		nOutParamCount = (int)strlen(pOutParam);

    while (!bInParamEnd)
    {
        luaL_checkstack(m_pLuaState, 1, "too many arguments");
        char cParmSig = *pInParam;
        switch (cParmSig)
        {
            case 'd':
                lua_pushnumber(m_pLuaState, va_arg(VarList, double));
                nInParamCount++;
                break;
            case 'f':
                lua_pushnumber(m_pLuaState, va_arg(VarList, float));
                nInParamCount++;
                break;
            case 'i':
                lua_pushinteger(m_pLuaState, va_arg(VarList, int));
                nInParamCount++;
                break;
            case 'l':
                lua_pushinteger(m_pLuaState, va_arg(VarList, long long));
                nInParamCount++;
                break;
            case 's':
                lua_pushstring(m_pLuaState, va_arg(VarList, char*));
                nInParamCount++;
                break;
            case 'p':
                lua_pushlightuserdata(m_pLuaState, va_arg(VarList, void*));
                nInParamCount++;
                break;
            case 'b':
                lua_pushboolean(m_pLuaState, va_arg(VarList, bool));
                nInParamCount++;
                break;
            case '=':
                bInParamEnd = true;
                break;
            default:
            {

            }
                break;
        }

        pInParam++;
    }

    if (lua_pcall(m_pLuaState, nInParamCount, nOutParamCount, 0) != 0)
    {
        const char* sresult = lua_tostring(m_pLuaState, -1);
        va_end(VarList);
        lua_settop(m_pLuaState, nStackTop);
        return false;
    }

    int nRetIndex = -nOutParamCount;
    bool bOutParamEnd = false;
    while (!bOutParamEnd)
    {
        char cParmSig = *pOutParam;
        switch (cParmSig)
        {
            case 'd':
                //ERROR_RETURN_FALSE(lua_isnumber(m_pLuaState, nRetIndex));
                *va_arg(VarList, double*) = lua_tonumber(m_pLuaState, nRetIndex);
                break;
            case 'f':
                //ERROR_RETURN_FALSE(lua_isnumber(m_pLuaState, nRetIndex));
                *va_arg(VarList, float*) = (float)lua_tonumber(m_pLuaState, nRetIndex);
                break;
            case 'i':
                //ERROR_RETURN_FALSE(lua_isinteger(m_pLuaState, nRetIndex));
                *va_arg(VarList, int*) = (int)lua_tointeger(m_pLuaState, nRetIndex);
                break;
            case 'l':
                //ERROR_RETURN_FALSE(lua_isinteger(m_pLuaState, nRetIndex));
                *va_arg(VarList, long long*) = (long long)lua_tointeger(m_pLuaState, nRetIndex);
                break;
            case 's':
                //ERROR_RETURN_FALSE(lua_isstring(m_pLuaState, nRetIndex));
                *va_arg(VarList, const char**) = lua_tostring(m_pLuaState, nRetIndex);
                break;
            case 'p':
                //ERROR_RETURN_FALSE(lua_isuserdata(m_pLuaState, nRetIndex));
                *va_arg(VarList, void**) = lua_touserdata(m_pLuaState, nRetIndex);
                break;
            case 'b':
                //ERROR_RETURN_FALSE(lua_isboolean(m_pLuaState, nRetIndex));
                *va_arg(VarList, bool*) = lua_toboolean(m_pLuaState, nRetIndex);
                break;
            case 0:
                bOutParamEnd = true;
                break;
            default:
            {

            }
                break;
        }

        nRetIndex++;
        pOutParam++;
    }

    va_end(VarList);
    lua_settop(m_pLuaState, nStackTop);

    return true;
}

bool CLuaHelper::LoadScriptFile(const char* pszLuaFile)
{
    if(m_pLuaState == NULL)
    {
        return false;
    }

    if(luaL_dofile(m_pLuaState, pszLuaFile))
    {
        return false;
    }

    return true;
}

bool CLuaHelper::LoadScriptFile( std::vector<std::string>& vtScriptList )
{
    for(std::vector<std::string>::iterator itor = vtScriptList.begin(); itor != vtScriptList.end(); itor++)
    {
        if(!LoadScriptFile(itor->c_str()))
        {
            return false;
        }
    }

    return true;
}

bool CLuaHelper::DoLuaScript(std::string strLuaScript)
{
    if (m_pLuaState == NULL)
    {
        return false;
    }

    if (luaL_dostring(m_pLuaState, strLuaScript.c_str()))
    {
        return false;
    }

    return true;
}

bool CLuaHelper::GetStackParams( char* pStrParamSig, ... )
{
    bool bRet		= true;

    va_list VarList;
    va_start(VarList, pStrParamSig);

    int nArgNum = (int)strlen(pStrParamSig);
    if(nArgNum < 0)
    {
        va_end(VarList);
        lua_settop(m_pLuaState, 0);
        return false;
    }

    for(int i = 0; i < nArgNum; i++)
    {
        luaL_checkstack(m_pLuaState, 1, "too many arguments");

        char cParmSig = pStrParamSig[i];

        switch(cParmSig)
        {
            case 'd':
            {
                if(!lua_isnumber(m_pLuaState, i + 1))
                {
                    bRet = false;
                    break;
                }

                *va_arg(VarList, double*) = lua_tonumber(m_pLuaState, i + 1);
            }
                break;
            case 'f':
            {
                if (!lua_isnumber(m_pLuaState, i + 1))
                {
                    bRet = false;
                    break;
                }

                *va_arg(VarList, float*) = (float)lua_tonumber(m_pLuaState, i + 1);
            }
                break;
            case 'i':
            {
                if(!lua_isinteger(m_pLuaState, i + 1))
                {
                    bRet = false;
                    break;
                }

                *va_arg(VarList, int*) = (int)lua_tointeger(m_pLuaState, i + 1);
            }
                break;
            case 'l':
            {
                if (!lua_isinteger(m_pLuaState, i + 1))
                {
                    bRet = false;
                    break;
                }

                *va_arg(VarList, long long*) = (long long)lua_tointeger(m_pLuaState, i + 1);
            }
                break;
            case 's':
            {
                if(!lua_isstring(m_pLuaState, i + 1))
                {
                    bRet = false;
                    break;
                }

                *va_arg(VarList, const char**) = lua_tostring(m_pLuaState, i + 1);
            }
                break;
            case 'p':
            {
                if (!lua_isstring(m_pLuaState, i + 1))
                {
                    bRet = false;
                    break;
                }
                *va_arg(VarList, void**) = lua_touserdata(m_pLuaState, i + 1);
            }
                break;
            case 'b':
            {
                if(!lua_isboolean(m_pLuaState, i + 1))
                {
                    bRet = false;
                    break;
                }

                *va_arg(VarList, bool*) = (bool)lua_toboolean(m_pLuaState, i + 1);
            }
                break;
            default:
            {
                bRet = false;
            }
                break;
        }
    }

    va_end(VarList);
    lua_settop(m_pLuaState, 0);
    return bRet;
}

bool CLuaHelper::RegisterFunction( const char* pszLibName, const luaL_Reg* l )
{
    //luaL_register(m_pLuaState, libname, l);

    return true;
}

bool CLuaHelper::RegisterFunction(const char* name, lua_CFunction fn)
{
    lua_register(m_pLuaState, name, fn);

    return true;
}


/*
LuaManager luamgr;
luamgr.Init();
luamgr.RegisterFunction("TestAdd", ToLua_TestAdd);
luamgr.LoadAllLua("F:\\GameRoot\\Server\\Lua\\Debug");
int value = luamgr.GetGlobalVarInt("a");
int a = 10;
int b = 20;
int c = 0;
TestLuaClass LuaClass;
luamgr.CallLuaFunction("add", "iip=i", a, b, &LuaClass, &c);
*/


CLuaManager::CLuaManager(void)
{
    m_pLuaState = NULL;
}

CLuaManager::~CLuaManager(void)
{
    Close();
}

CLuaManager* CLuaManager::GetInstancePtr()
{
    static CLuaManager _StaticValue;

    return &_StaticValue;
}

bool CLuaManager::Close()
{
    if (m_pLuaState != NULL)
    {
        lua_close(m_pLuaState);
        m_pLuaState = NULL;
    }

    return true;
}

bool CLuaManager::Init()
{
    //ERROR_RETURN_FALSE(m_pLuaState == NULL);

    m_pLuaState = luaL_newstate();

    //ERROR_RETURN_FALSE(m_pLuaState != NULL);

    luaL_openlibs(m_pLuaState);

    return true;
}