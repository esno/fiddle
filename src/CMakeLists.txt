set(FIDDLE_SRC
  lua.c
  ${CMAKE_PROJECT_NAME}.c)

add_executable(${CMAKE_PROJECT_NAME} ${FIDDLE_SRC})
target_link_libraries(${CMAKE_PROJECT_NAME}
  ${LUA_LIBRARIES})
