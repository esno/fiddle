#include <stdlib.h>

#include "fiddle.h"
#include "fiddle/log.h"
#include "fiddle/lua.h"

void print_usage(int argc, const char *argv[]) {
  PRINT("USAGE: %s <script>\n", argv[0]);
}

int main(int argc, const char *argv[]) {
  if (argc < 2) {
    print_usage(argc, argv);
    return EXIT_FAILURE;
  }

  const char *script = argv[1];
  lua_State *L = fdl_lua_new();
  int rc = EXIT_SUCCESS;
  if (fdl_lua_dofile(L, script) == FDL_NOK)
    rc = EXIT_FAILURE;

  lua_close(L);
  return rc;
}
