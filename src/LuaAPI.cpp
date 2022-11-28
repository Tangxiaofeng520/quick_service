//
// Created by diandian on 11/23/22.
//

#include <cstring>
#include "LuaAPI.h"
#include <memory>
#include "qs.h"
using namespace std;
void LuaAPI::Register(lua_State *luaState) {
    static luaL_Reg lualibs[] = {
    //第一个参数 lua 中方法名字 第二参数 c++ 中参数
            {"new_service",NewService},
            //以后一个 {NULL,NULL} 表示结束
            {NULL,NULL},
    };
    luaL_newlib(luaState,lualibs);//创建一张表 讲lualibs注册
    lua_setglobal(luaState,"cqs");
}

int LuaAPI::NewService(lua_State *luaState) {
    int num = lua_gettop(luaState);//获取参数的个数
    if(lua_isstring(luaState, 1) == 0){  //1:是 0:不是
        lua_pushinteger(luaState, -1);
        return 1;
    }
    size_t len = 0;
    const char *type = lua_tolstring(luaState,1,&len);
    char *newstr = new char[len+1];//后面加/0
    memcpy(newstr, type, len);
    newstr[len] = '\0';
    auto t = make_shared<string>(newstr);
    service_mgr* srv_mgr = qs::inst->get_service_mgr();
    srv_mgr->new_service(t);
    return 0;
}

int LuaAPI::send_message(lua_State *luaState)
{

}