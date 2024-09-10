#include <string.h>
#include <errno.h>
#include <sys/wait.h>

#include <lua.h>
#include <lauxlib.h>

#include "fiddle/lua.h"

//-- rc, errno, strerrno = waitpid(pid)
static int _fdl_waitpid(lua_State *L) {
  pid_t pid = luaL_checkinteger(L, 1);
  int stat_loc = -1;
  if (waitpid(pid, &stat_loc, 0) != pid) {
    lua_pushnil(L);
    lua_pushinteger(L, errno);
    lua_pushstring(L, (const char *) strerrorname_np(errno));
    return 3;
  }

  lua_pushinteger(L, stat_loc);
  return 1;
}

void fdl_posix_luaopen(lua_State *L) {
  struct luaL_Reg lfuncs[] = {
    { "waitpid", _fdl_waitpid },
    { NULL, NULL }
  };

	fdl_lua_register_global(L, lfuncs);
}
