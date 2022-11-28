//
// Created by diandian on 11/21/22.
//

#ifndef QUICK_SERVICE_LUAMANAGER_H
#define QUICK_SERVICE_LUAMANAGER_H

#include <vector>
#include "string"

using namespace std;
extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

class CLuaHelper
{
public:
    CLuaHelper(void);
    ~CLuaHelper(void);

public:
    lua_State* GetLuaState();

    bool	Attach(lua_State* L);

    bool	Deattch();

    bool	LoadAllLua(string strDir);

    bool	LoadAllLua(const char* pszDir);

    bool	LoadScriptFile(const char* pszLuaFile);

    bool	LoadScriptFile(vector<string>& vtScriptList);

    bool    DoLuaScript(string strLuaScript);

    bool	CallLuaFunction(string strFuncName, char* pStrParamSig, ...);

    bool	CallLuaTableFunc(string strTableName, string strFuncName, char* pStrParamSig, ...);

    bool	GetStackParams(char* pStrParamSig, ...);

    bool    RegisterFunction(const char* pszLibName, const luaL_Reg* l);

    bool	RegisterFunction(const char* name, lua_CFunction fn);

    //获取堆栈脚本变量
    bool	GetStackValue_Ptr(int nStackIndex, void*& ptrValue);
    bool	GetStackValue_Int(int nStackIndex, int& intValue);
    bool	GetStackValue_double(int nStackIndex, double& doubleValue);
    bool    GetStackValue_String(int nStackIndex, const char*& strValue);

    //获取全局脚本变量
    int	GetGlobalVarInt(const char* pszVarName);
    bool	GetGlobalVarboolean(const char* pszVarName);
    double  GetGlobalVardouble(const char* pszVarName);
    const char*	GetGlobalVarString(const char* pszVarName);

protected:

    lua_State*			m_pLuaState;
};

class CLuaManager : public CLuaHelper
{
private:
    ~CLuaManager(void);

public:
    bool	Init();

    bool	Close();

    CLuaManager(void);

public:
    static CLuaManager* GetInstancePtr();
};



#endif //QUICK_SERVICE_LUAMANAGER_H
