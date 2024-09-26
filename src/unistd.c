#include <unistd.h>
#include <errno.h>
#include <string.h>

#include <lua.h>
#include <lauxlib.h>

#include "fiddle/lua.h"

//-- rc, errno, strerrno = chdir(path)
static int fdl_chdir(lua_State *L) {
  const char *path = luaL_checkstring(L, 1);

  if (chdir(path) == -1) {
    lua_pushnil(L);
    lua_pushinteger(L, errno);
    lua_pushstring(L, (const char *) strerrorname_np(errno));
    return 3;
  }

  lua_pushboolean(L, 1);
  return 1;
}

void fdl_unistd_luaopen(lua_State *L) {
  struct luaL_Reg lfuncs[] = {
    { "chdir", fdl_chdir },
    { NULL, NULL }
  };

	fdl_lua_register_global(L, lfuncs);
}
