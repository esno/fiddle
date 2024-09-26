#include <ftw.h>
#include <errno.h>
#include <string.h>

#include <lua.h>
#include <lauxlib.h>

#include "fiddle/lua.h"

//-- rc, errno, strerrno = nftw(dirpath, fn, nopenfd, flags)
static int fdl_nftw(lua_State *L) {
  const char *dirpath = luaL_checkstring(L, 1);
  luaL_checktype(L, 2, LUA_TFUNCTION);

  if (lua_iscfunction(L, 2) == 0) {
    lua_pushnil(L);
    return 1;
  }

  lua_CFunction func = lua_topointer(L, 2);
  int nopenfd = luaL_checkinteger(L, 3);
  int flags = luaL_checkinteger(L, 4);

  int rc;
  int fn(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf) {
    lua_pushcfunction(L, func);
    lua_pushstring(L, fpath);
    return fdl_lua_pcall(L, 1, 1);
  }

  if (nftw(dirpath, &fn, nopenfd, flags) != 0) {
    lua_pushnil(L);
    return 1;
  }

  lua_pushboolean(L, 1);
  return 1;
}

void fdl_ftw_luaopen(lua_State *L) {
  struct luaL_Reg lfuncs[] = {
    { "nftw", fdl_nftw },
    { NULL, NULL }
  };

	fdl_lua_register_global(L, lfuncs);

  lua_pushinteger(L, FTW_ACTIONRETVAL);
  lua_setglobal(L, "FTW_ACTIONRETVAL");

  lua_pushinteger(L, FTW_CONTINUE);
  lua_setglobal(L, "FTW_CONTINUE");

  lua_pushinteger(L, FTW_SKIP_SIBLINGS);
  lua_setglobal(L, "FTW_SKIP_SIBLINGS");

  lua_pushinteger(L, FTW_SKIP_SUBTREE);
  lua_setglobal(L, "FTW_SKIP_SUBTREE");

  lua_pushinteger(L, FTW_STOP);
  lua_setglobal(L, "FTW_STOP");

  lua_pushinteger(L, FTW_CHDIR);
  lua_setglobal(L, "FTW_CHDIR");

  lua_pushinteger(L, FTW_DEPTH);
  lua_setglobal(L, "FTW_DEPTH");

  lua_pushinteger(L, FTW_MOUNT);
  lua_setglobal(L, "FTW_MOUNT");

  lua_pushinteger(L, FTW_PHYS);
  lua_setglobal(L, "FTW_PHYS");

  lua_pushinteger(L, FTW_DP);
  lua_setglobal(L, "FTW_DP");

  lua_pushinteger(L, FTW_SL);
  lua_setglobal(L, "FTW_SL");

  lua_pushinteger(L, FTW_SLN);
  lua_setglobal(L, "FTW_SLN");
}
