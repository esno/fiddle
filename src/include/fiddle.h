#ifndef fdl_h
#define fdl_h 1

extern int _argc;
extern const char **_argv;

enum fdl_rcs {
  FDL_NOK = -1,
  FDL_OK = 0
};

void fdl_ftw_luaopen(lua_State *L);
void fdl_stdio_luaopen(lua_State *L);
void fdl_unistd_luaopen(lua_State *L);

#endif
