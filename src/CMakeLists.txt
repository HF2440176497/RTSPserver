
# SOURCE_DIR 由项目根目录 cmake 文件定义

set(SOURCE_FILES)

list(APPEND SOURCE_FILES  
    ${SOURCE_DIR}/main.cpp  
    # ... 添加其他源文件  
)

list(APPEND SOURCE_FILES  
    ${SOURCE_DIR}/Live/Buffer.cpp
    # ... 添加其他源文件  
) 

list(APPEND SOURCE_FILES  
    ${SOURCE_DIR}/Scheduler/SocketsOps.cpp
    # ... 添加其他源文件  
)

# 添加可执行文件
add_executable(server ${SOURCE_FILES})  

# 设置需要链接的库
target_link_libraries(server pthread)
