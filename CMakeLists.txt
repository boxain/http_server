cmake_minimum_required(VERSION 3.10)
project(http_server C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

# Define global micro for debug log
# add_compile_definitions(DEBUG_LOG=1) # print more log
# add_compile_definitions(CONN_TEST=1) # let each http request delay 30 seconds.

# For Debug Mode
# if(NOT CMAKE_BUILD_TYPE)
#     set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type (Debug, Release, etc)" FORCE)
# endif()

add_subdirectory(lib)
add_executable(http_server server_main.c)
target_link_libraries(http_server PRIVATE http_lib)


add_executable(http_client client_main.c)
target_link_libraries(http_client PRIVATE http_lib)