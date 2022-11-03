cmake_minimum_required(VERSION 3.8)
project(main)
get_filename_component(PARENT_DIR ${CMAKE_CURRENT_LIST_DIR}/.. ABSOLUTE)

set(QJS_LIB ${PARENT_DIR}/lib/quickjs)

INCLUDE_DIRECTORIES(${QJS_LIB}/include)
LINK_DIRECTORIES(${QJS_LIB}/lib)

link_libraries(${QJS_LIB}/lib/libquickjs.a)

add_executable(main main.c)
target_link_libraries(main libquickjs.a)  # 显示指定链接静态库

set(WASM_FLAG "-O3 -s WASM=1 -s ALLOW_MEMORY_GROWTH ")
if(EMSCRIPTEN)
  message("emcc")
  set(CMAKE_C_COMPILER "emcc")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${WASM_FLAG}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${WASM_FLAG}")
  set(WASM_OUTPUT ".html")
  set(CMAKE_EXECUTABLE_SUFFIX "${WASM_OUTPUT}")
endif()