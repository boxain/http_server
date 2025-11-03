## 簡易 HTTP 伺服器 (Simple HTTP Server)
這是一個使用 C 語言編寫的極簡 HTTP 1.1 伺服器，主要用於學習 Unix 網路程式設計、系統呼叫以及 C 語言的底層執行原理。

此專案實作了一個併發伺服器，能夠接受 HTTP 請求，解析請求行 (request line)，並回傳一個靜態的 "Hello World" 回應。同時，專案也包含一個多執行緒的客戶端，用於模擬高併發連線測試。

功能
- http_server
    - 監聽指定埠號 (Port 8000)。

    - 使用 fork() 或 pthread() 實現併發，處理多個客戶端連線。

    - 解析 HTTP 請求的第一行 (Method, URI, Version)。

    - 回傳一個 HTTP/1.1 200 OK 回應，Content-Type 為 text/plain。

- http_client
    - 實現 HTTP 客戶端，使用多個執行緒 (Threads) 建立連線，並發送 GET 請求到伺服器。


### 專案結構
本專案將網路 I/O、Socket 建立以及 HTTP 協定處理等核心邏輯，全部封裝在一個 共享函式庫 (libhttp_lib.so) 中。兩個主程式 (server_main.c 和 client_main.c) 只負責各自的連線迴圈和併發控制，並連結此函式庫。
```
http_server/
│
├── server_main.c       # 主程式：包含 accept() 迴圈和併發控制 (fork/pthread)
│
├── CMakeLists.txt      # 根 CMake 檔案，用於編譯 server_main.c, client_main.c 並連結函式庫
│
└── lib/                # 核心功能靜態函式庫
    │
    ├── http_handler.h  # 定義 request_handle() 介面及常數
    ├── http_handler.c  # 實作 HTTP 請求的解析與回應
    │
    ├── open_listen.h   # 定義 open_listenfd() 介面及常數
    ├── open_listen.c   # 實作 socket, bind, listen 的設定
    │
    ├── open_client.h   # 定義 open_clientfd() 介面及連線常數
    ├── open_client.c   # 實作 getaddrinfo/socket/connect 的客戶端連線邏輯
    ├
    ├── rio.h           # Robust I/O 函式庫介面 (readline, write)
    ├── rio.c           # 實作 readline_rio 和 write_rio
    │
    └── CMakeLists.txt  # 用於將 lib/ 中的 .c 檔案打包成一個共享函式庫
```

### 併發模型 (Concurrency Model)
本專案可以透過 main.c 檔案中的一個巨集 (macro) 來切換兩種不同的併發處理模型。
```
// 在 main.c 的頂部修改此行
#define USE_THREADING 1
```

- USE_THREADING = 0：Process (行程) 模型
    - 主行程 accept() 一個新連線後，會呼叫 fork() 產生一個子行程 (child process)。

    - 子行程負責呼叫 request_handle() 處理請求，完成後 exit(0)。

    - 父行程 (parent process) 則關閉連線的 file descriptor，並繼續 accept() 下一個連線。

    - 使用 SIGCHLD 訊號處理器 (signal handler) 搭配 waitpid() 迴圈來回收已終止的子行程，防止殭屍行程 (zombie process) 產生。

- USE_THREADING = 1：Thread (執行緒) 模型

    - 主執行緒 accept() 一個新連線後，會呼叫 pthread_create() 產生一個新的執行緒。

    - 新執行緒負責呼叫 request_handle() 處理請求。

    - 為了安全地傳遞 connected_fd，主執行緒使用 malloc 為 fd 分配獨立記憶體，並將指標傳遞給新執行緒。

    - 新執行緒在啟動後會立刻呼叫 pthread_detach(pthread_self())，讓系統在執行緒結束時自動回收其資源。

### 使用 Debugger 追蹤 Assembly 學習報告
為了深入理解 C 語言在底層的實際運作，我使用了 VS Code 搭配 GDB (透過 C/C++ 擴充套件) 進行了組合語言層級的追蹤，可點擊 Assembly Code Analysis.pdf 查看詳情

#### 除錯環境設定：

- 編譯： 使用 CMAKE_BUILD_TYPE=Debug 進行編譯，確保 -g 旗標被加入，使執行檔包含原始碼和行號資訊。

- launch.json： 在 VS Code 的 .vscode/launch.json 中設定 "stopAtEntry": true。這使得 debugger 會在程式的入口點 (_start 或 main) 就立刻暫停，而不是等到第一個中斷點。

#### 追蹤：

- 在 VS Code 中按下 F5 啟動除錯。

- 在編輯器視窗中按右鍵，選擇「Open Disassembly View」(打開反組譯檢視)。

- 使用除錯工具列中的「Step Into (Instruction)」(快捷鍵 Ctrl+F11 或 Opt+F11)，逐一執行單行組合語言指令，而非 C 語言行。


### 參考資源
- GDB:        https://zhuanlan.zhihu.com/p/1919803617872938161
- Deassembly: https://huc0day.blog.csdn.net/article/details/151019122?spm=1001.2101.3001.6650.4&utm_medium=distribute.pc_relevant.none-task-blog-2%7Edefault%7EYuanLiJiHua%7EPosition-4-151019122-blog-86557911.235%5Ev43%5Epc_blog_bottom_relevance_base1&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2%7Edefault%7EYuanLiJiHua%7EPosition-4-151019122-blog-86557911.235%5Ev43%5Epc_blog_bottom_relevance_base1&utm_relevant_index=9
- Ubuntu ps command : https://blog.csdn.net/weixin_42148809/article/details/135104281
- CMake: https://ithelp.ithome.com.tw/articles/10221101
