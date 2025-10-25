# 1. 建立一個名為 "http_lib" 的函式庫
#    STATIC 表示我們要的是靜態函式庫 (.a)
#    列出所有要編譯進這個函式庫的 .c 檔案
add_library(http_lib STATIC
    rio.c
    http_handler.c
    open_listen.c
)

# 2. 關鍵：告訴 CMake，"http_lib" 這個 target 需要 "公開" (PUBLIC) 
#    它的標頭檔目錄 (也就是當前目錄, ${CMAKE_CURRENT_SOURCE_DIR})
#
#    PUBLIC 的意思是：
#    a) 編譯 http_lib "自己" 時，會知道去哪裡找 .h 檔 (例如 http_handler.c 找 rio.h)
#    b) 任何 "連結到" http_lib 的 target (例如上層的 http_server)，
#       也會自動被告知要去這個目錄找 .h 檔 (例如 main.c 找 lib/http_handler.h)
target_include_directories(http_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})