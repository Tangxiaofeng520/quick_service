//
// Created by diandian on 11/23/22.
//

#ifndef QUICK_SERVICE_LUAAPI_H
#define QUICK_SERVICE_LUAAPI_H

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
};

class LuaAPI {
public:
    static void Register(lua_State *luaState);

    static int NewService(lua_State *luaState);

    int send_message(lua_State *luaState);
};


#endif //QUICK_SERVICE_LUAAPI_H
