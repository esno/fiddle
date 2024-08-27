#ifndef fdl_lua_h
#define fdl_lua_h 1

#include <lua.h>

int fdl_lua_dofile(lua_State *L, const char *script);
lua_State *fdl_lua_new(void);

#endif
