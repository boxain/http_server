# 1. 建立一個名為 "http_lib" 的 shared liberary
add_library(http_lib SHARED
    rio.c
    http_handler.c
    open_listen.c
    open_client.c
)

# 2. 關鍵：告訴 CMake，"http_lib" 這個 target 需要 "公開" (PUBLIC) 
#    它的標頭檔目錄 (也就是當前目錄, ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(http_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})