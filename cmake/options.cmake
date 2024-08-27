set(WITH_JOURNALD 1 CACHE STRING "Define using journald style logging syntax or not")

set(FDL_CPATH "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}/${CMAKE_PROJECT_NAME}/?.so" CACHE STRING "custom lua package.cpath")
set(FDL_PATH "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_DATAROOTDIR}/${CMAKE_PROJECT_NAME}/lua/?.lua;${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_DATAROOTDIR}/${CMAKE_PROJECT_NAME}/lua/?/init.lua" CACHE STRING "custom lua package.path")

message("")
message("-- fiddle - lua driven application framework")
message("")
message("-- Options:")
message("--   -DWITH_JOURNALD=[0|1]  Enables/Disables journald style logging syntax")
message("--                          DEFAULT: 1")
message("--   -DFDL_CPATH=<cpath>    set custom package.cpath")
message("--   -DFDL_PATH=<path>      set custom package.path")
message("")
