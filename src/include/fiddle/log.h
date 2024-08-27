#ifndef fdl_log_h
#define fdl_log_h 1

#include <lua.h>

void EMERG(const char *format, ...);
void ALERT(const char *format, ...);
void CRIT(const char *format, ...);
void ERROR(const char *format, ...);
void WARN(const char *format, ...);
void NOTICE(const char *format, ...);
void INFO(const char *format, ...);
void DEBUG(const char *format, ...);

void PRINT(const char *format, ...);

void fdl_log_setlevel(int lvl);

void fdl_log_luaopen(lua_State *L);

#endif
