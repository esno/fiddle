#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "fiddle.h"
#include "fiddle/lua.h"

#ifndef FDL_CPATH
#define FDL_CPATH "/usr/local/lib/fiddle/?.so"
#endif
#ifndef FDL_PATH
#define FDL_PATH "/usr/local/share/fiddle/lua/?.lua;/usr/local/share/fiddle/lua/?/init.lua"
#endif

static int _pcall(lua_State *L, int larg, int lret) {
  if (lua_pcall(L, larg, lret, 0) != LUA_OK) {
    lerror();
    return FDL_NOK;
  }

  return FDL_OK;
}

static void _push_args(lua_State *L, int argc, const char *argv[]) {
  lua_newtable(L); {
    for (int i = 1; i <= argc; ++i) {
      lua_pushnumber(L, i);
      lua_pushstring(L, argv[i]);
      lua_rawset(L, -3);
    }
    lua_setglobal(L, "arg");
  }
}

int fdl_lua_dofile(lua_State *L, const char *script) {
  if (luaL_loadfile(L, script) != LUA_OK) {
    ERROR(lua_tostring(L, -1));
    return FDL_NOK;
  }

  return _pcall(L, 0, LUA_MULTRET);
}

lua_State *fdl_lua_new(int argc, const char *argv[]) {
  lua_State *L = luaL_newstate();
  luaL_openlibs(L);
  fdl_log_luaopen(L);

  lua_getglobal(L, "package"); {
    lua_pushstring(L, FDL_CPATH);
    lua_setfield(L, -2, "cpath");
    lua_pushstring(L, FDL_PATH);
    lua_setfield(L, -2, "path");
  }

  _push_args(L, argc, argv);

  return L;
}

void fdl_lua_register_global(lua_State *L, luaL_Reg *lfuncs) {
  while (lfuncs->name) {
    lua_pushcfunction(L, lfuncs->func);
    lua_setglobal(L, lfuncs->name);
    lfuncs += 1;
  }
}
