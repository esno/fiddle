#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "fiddle.h"

int fdl_lua_dofile(lua_State *L, const char *script) {
  if (luaL_loadfile(L, script) != LUA_OK)
    return FDL_NOK;

  return FDL_OK;
}

lua_State *fdl_lua_new(void) {
  lua_State *L = luaL_newstate();
  luaL_openlibs(L);

  return L;
}
