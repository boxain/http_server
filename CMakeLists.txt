cmake_minimum_required(VERSION 3.10)
project(http_server C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CHCHE STRING "Build type (Debug, Release, etc)" FORCE)
endif()


add_subdirectory(lib)
add_executable(http_server main.c)
target_link_libraries(http_server PRIVATE http_lib)
