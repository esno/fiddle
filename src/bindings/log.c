#include <lua.h>
#include <lauxlib.h>

#include <fiddle/log.h>
#include <fiddle/lua.h>

#define DECLARE_LUA_LOG_FUNC(__name)          \
	static int _log_##__name(lua_State *L) {    \
		const char *msg = luaL_checkstring(L, 1); \
		__name("%s\n", msg);                      \
		return 0;                                 \
	}

DECLARE_LUA_LOG_FUNC(EMERG)
DECLARE_LUA_LOG_FUNC(ALERT)
DECLARE_LUA_LOG_FUNC(CRIT)
DECLARE_LUA_LOG_FUNC(ERROR)
DECLARE_LUA_LOG_FUNC(WARN)
DECLARE_LUA_LOG_FUNC(NOTICE)
DECLARE_LUA_LOG_FUNC(INFO)
DECLARE_LUA_LOG_FUNC(DEBUG)

DECLARE_LUA_LOG_FUNC(PRINT)

static int _log_setloglvl(lua_State *L) {
	int lvl = luaL_checkinteger(L, 1);
	fdl_log_setlevel(lvl);
	lua_pushnil(L);
	return 1;
}

void fdl_log_luaopen(lua_State *L) {
	struct luaL_Reg lfuncs[] = {
		{ "_alert", _log_ALERT },
		{ "_crit", _log_CRIT },
		{ "_debug", _log_DEBUG },
		{ "_emerg", _log_EMERG },
		{ "_error", _log_ERROR },
		{ "_info", _log_INFO },
		{ "_notice", _log_NOTICE },
		{ "_warn", _log_WARN },

		{ "print", _log_PRINT },
		{ "setloglvl", _log_setloglvl },
		{ NULL, NULL }
	};

	fdl_lua_register_global(L, lfuncs);
}
