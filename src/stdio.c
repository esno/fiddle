#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <lua.h>
#include <lauxlib.h>

#include "fiddle/lua.h"

static int fdl_noop(lua_State *L) { return 0; };

//-- bytes, errno, strerrno = fprintf(stream, msg)
static int fdl_fprintf(lua_State *L) {
  luaL_Stream *udata = (luaL_Stream *) luaL_checkudata(L, 1, LUA_FILEHANDLE);
  const char *msg = luaL_checkstring(L, 2);
  int bytes = fprintf(udata->f, "%s", msg);

  if (bytes < 0) {
    lua_pushnil(L);
    lua_pushinteger(L, errno);
    lua_pushstring(L, (const char *) strerrorname_np(errno));
    return 3;
  }

  lua_pushinteger(L, bytes);
  return 1;
}

void fdl_stdio_luaopen(lua_State *L) {
  struct luaL_Reg lfuncs[] = {
    { "fprintf", fdl_fprintf },
    { NULL, NULL }
  };

	fdl_lua_register_global(L, lfuncs);

  luaL_Stream *p = (luaL_Stream *) lua_newuserdata(L, sizeof(luaL_Stream));
  p->closef = &fdl_noop;
  p->f = stdin;
  luaL_setmetatable(L, LUA_FILEHANDLE);
  lua_setglobal(L, "stdin");

  p = (luaL_Stream *) lua_newuserdata(L, sizeof(luaL_Stream));
  p->closef = &fdl_noop;
  p->f = stdout;
  luaL_setmetatable(L, LUA_FILEHANDLE);
  lua_setglobal(L, "stdout");

  p = (luaL_Stream *) lua_newuserdata(L, sizeof(luaL_Stream));
  p->closef = &fdl_noop;
  p->f = stderr;
  luaL_setmetatable(L, LUA_FILEHANDLE);
  lua_setglobal(L, "stderr");
}
