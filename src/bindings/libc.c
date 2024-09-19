#include <unistd.h>
#include <string.h>
#include <errno.h>

#include <lua.h>
#include <lauxlib.h>

#include "fiddle/lua.h"

static int _fdl_fclose(lua_State *L);

//-- fd, errno, strerrno = dup2(oldfd, newfd)
static int _fdl_dup2(lua_State *L) {
  int oldfd = luaL_checkinteger(L, 1);
  int newfd = luaL_checkinteger(L, 2);
  int fd = dup2(oldfd, newfd);
  if (fd != newfd) {
    lua_pushnil(L);
    lua_pushinteger(L, errno);
    lua_pushstring(L, (const char *) strerrorname_np(errno));
    return 3;
  }

  lua_pushinteger(L, fd);
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

//-- file = fdopen(fd)
static int _fdl_fdopen(lua_State *L) {
  int fd = luaL_checkinteger(L, 1);
  const char *mode = luaL_checkstring(L, 2);

  FILE *stream = fdopen(fd, mode);
  if (fd == -1) {
    lua_pushnil(L);
    lua_pushinteger(L, errno);
    lua_pushstring(L, (const char *) strerrorname_np(errno));
    return 3;
  }

  luaL_Stream *p = (luaL_Stream *) lua_newuserdata(L, sizeof(luaL_Stream));
  p->closef = &_fdl_fclose;
  p->f = stream;
  luaL_setmetatable(L, LUA_FILEHANDLE);
  return 1;
}

//-- fd, errno, strerrno = fileno(stream)
static int _fdl_fileno(lua_State *L) {
  luaL_Stream *udata = (luaL_Stream *) luaL_checkudata(L, 1, LUA_FILEHANDLE);
  int fd = fileno(udata->f);

  if (fd == -1) {
    lua_pushnil(L);
    lua_pushinteger(L, errno);
    lua_pushstring(L, (const char *) strerrorname_np(errno));
    return 3;
  }

  lua_pushinteger(L, fd);
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

  lua_pushinteger(L, fds[0]);
  lua_pushinteger(L, fds[1]);

  return 2;
}

void fdl_libc_luaopen(lua_State *L) {
  struct luaL_Reg lfuncs[] = {
    { "dup2", _fdl_dup2 },
    { "execvp", _fdl_execvp },
    { "fclose", _fdl_fclose },
    { "fdopen", _fdl_fdopen },
    { "fileno", _fdl_fileno },
    { "fork", _fdl_fork },
    { "pipe", _fdl_pipe },
    { NULL, NULL }
  };

	fdl_lua_register_global(L, lfuncs);
}
