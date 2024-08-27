set(FIDDLE_SRC
  bindings/log.c
  log.c
  lua.c
  ${CMAKE_PROJECT_NAME}.c)

add_executable(${CMAKE_PROJECT_NAME} ${FIDDLE_SRC})
target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC FDL_LOG_JOURNALD=${WITH_JOURNALD})
target_link_libraries(${CMAKE_PROJECT_NAME}
  ${LUA_LIBRARIES}
  ${CMAKE_THREAD_LIBS_INIT})
