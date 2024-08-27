cmake_minimum_required(VERSION 3.30)
project(fiddle VERSION 0.0.1)
# dependencies
find_package(Lua 5.4 REQUIRED)
find_package(Threads REQUIRED)

include(GNUInstallDirs)
include(cmake/options.cmake)
include_directories(src/include ${LUA_INCLUDE_DIR})
add_subdirectory(src)
