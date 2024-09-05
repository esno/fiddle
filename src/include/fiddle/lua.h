#ifndef fdl_lua_h
#define fdl_lua_h 1

#include <lua.h>
#include <lauxlib.h>

#include "fiddle/log.h"

int fdl_lua_dofile(lua_State *L, const char *script);
lua_State *fdl_lua_new(int argc, const char *argv[]);
void fdl_lua_register_global(lua_State *L, luaL_Reg *lfuncs);

#define lerror() ERROR("%s\n", lua_tostring(L, -1))

#endif
