#include <unistd.h>
#include <string.h>
#include <errno.h>

#include <lua.h>
#include <lauxlib.h>

#include "fiddle/lua.h"

static int _fdl_fclose(lua_State *L);

static luaL_Stream *_pushfile(lua_State *L) {
  luaL_Stream *p = (luaL_Stream *) lua_newuserdata(L, sizeof(luaL_Stream));
  p->closef = &_fdl_fclose;
  luaL_setmetatable(L, LUA_FILEHANDLE);
  return p;
}

//-- bool, errno, strerrno = close(stream)
static int _fdl_fclose(lua_State *L) {
  luaL_Stream *udata = (luaL_Stream *) luaL_checkudata(L, 1, LUA_FILEHANDLE);
  if (fclose(udata->f) == -1) {
    lua_pushnil(L);
    lua_pushinteger(L, errno);
    lua_pushstring(L, (const char *) strerrorname_np(errno));
    return 3;
  }

  lua_pushboolean(L, 1);
  return 1;
}

//-- bool, errno, strerrno = execvp(cmd)
static int _fdl_execvp(lua_State *L) {
  char *cmd = (char *) luaL_checkstring(L, 1);
  char *p = strtok(cmd, " ");
  char *argv[1024];
  memset(argv, 0, sizeof(char *) * 1024);

  for (int i = 0; p != NULL; ++i) {
    argv[i] = p;
    p = strtok(NULL, " ");
  }

  if (execvp(argv[0], argv) == -1) {
    lua_pushnil(L);
    lua_pushinteger(L, errno);
    lua_pushstring(L, (const char *) strerrorname_np(errno));
    return 3;
  }

  lua_pushboolean(L, 1);
  return 1;
}

//-- pid, errno, strerrno = fork()
static int _fdl_fork(lua_State *L) {
  pid_t pid = fork();
  if (pid == -1) {
    lua_pushnil(L);
    lua_pushinteger(L, errno);
    lua_pushstring(L, (const char *) strerrorname_np(errno));
    return 3;
  }

  lua_pushinteger(L, pid);
  return 1;
}

//-- fd, errno, strerrno = dup2(oldfd, newfd)
static int _fdl_dup2(lua_State *L) {
  luaL_Stream *oldfd = (luaL_Stream *) luaL_checkudata(L, 1, LUA_FILEHANDLE);
  int newfd = luaL_checkinteger(L, 2);
  int fd = dup2(fileno(oldfd->f), newfd);
  if (fd != newfd) {
    lua_pushnil(L);
    lua_pushinteger(L, errno);
    lua_pushstring(L, (const char *) strerrorname_np(errno));
    return 3;
  }

  lua_pushinteger(L, fd);
  return 1;
}

//-- rx, tx, errno, strerrno = pipe()
static int _fdl_pipe(lua_State *L) {
  int fds[2];
  if (pipe(fds) == -1) {
    lua_pushnil(L);
    lua_pushnil(L);
    lua_pushinteger(L, errno);
    lua_pushstring(L, (const char *) strerrorname_np(errno));
    return 4;
  }

  luaL_Stream *rx = _pushfile(L);
  luaL_Stream *tx = _pushfile(L);
  rx->f = fdopen(fds[0], "r");
  tx->f = fdopen(fds[1], "w");

  return 2;
}

void fdl_libc_luaopen(lua_State *L) {
  struct luaL_Reg lfuncs[] = {
    { "execvp", _fdl_execvp },
    { "fclose", _fdl_fclose },
    { "fork", _fdl_fork },
    { "dup2", _fdl_dup2 },
    { "pipe", _fdl_pipe },
    { NULL, NULL }
  };

	fdl_lua_register_global(L, lfuncs);
}
