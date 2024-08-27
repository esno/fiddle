#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "fiddle.h"
#include "fiddle/log.h"

static int _pcall(lua_State *L, int larg, int lret) {
  if (lua_pcall(L, larg, lret, 0) != LUA_OK) {
    ERROR(lua_tostring(L, -1));
    return FDL_NOK;
  }

  return FDL_OK;
}

int fdl_lua_dofile(lua_State *L, const char *script) {
  if (luaL_loadfile(L, script) != LUA_OK) {
    ERROR(lua_tostring(L, -1));
    return FDL_NOK;
  }

  return _pcall(L, 0, LUA_MULTRET);
}

lua_State *fdl_lua_new(void) {
  lua_State *L = luaL_newstate();
  luaL_openlibs(L);
  fdl_log_luaopen(L);

  return L;
}

void fdl_lua_register_global(lua_State *L, luaL_Reg *lfuncs) {
  while (lfuncs->name) {
    lua_pushcfunction(L, lfuncs->func);
    lua_setglobal(L, lfuncs->name);
    lfuncs += 1;
  }
}
