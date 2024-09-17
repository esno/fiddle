#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include <pthread.h>

#include <lua.h>
#include <lauxlib.h>

#include "fiddle.h"
#include "fiddle/lua.h"

typedef struct fdl_threads fdl_threads;
struct fdl_threads {
  pthread_t tid;
  lua_State *L;
  const char *routine;
};

static void *_pthread_routine(void *arg) {
  fdl_threads *p = (fdl_threads *) arg;
  fdl_lua_dofile(p->L, _argv[1]);
  lua_getglobal(p->L, p->routine);
  fdl_lua_pcall(p->L, 0, 0);

  pthread_exit(NULL);
}

//-- pthread = pthread_create(routine)
static int _fdl_pthread_create(lua_State *L) {
  const char *routine = luaL_checkstring(L, 1);
  fdl_threads *udata = (fdl_threads *) lua_newuserdata(L, sizeof(fdl_threads));
  udata->L = fdl_lua_new(_argc, _argv);
  udata->routine = routine;

  int rc = pthread_create(&udata->tid, NULL, &_pthread_routine, udata);
  if (rc != 0) {
    lua_pushnil(L);
    lua_pushinteger(L, rc);
    lua_pushstring(L, (const char *) strerrorname_np(rc));
    return 3;
  }

  return 1;
}

//-- rc, errno, strerrno = pthread_join(pthread);

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
    { "pthread_create", _fdl_pthread_create },
    { "waitpid", _fdl_waitpid },
    { NULL, NULL }
  };

	fdl_lua_register_global(L, lfuncs);
}
