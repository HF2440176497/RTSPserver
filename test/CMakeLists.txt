
set(TEST_SOURCE_FILES)

list(APPEND TEST_SOURCE_FILES  
    ${SOURCE_DIR}/Live/Buffer.cpp
    # ... 添加其他源文件  
) 

list(APPEND TEST_SOURCE_FILES  
    ${SOURCE_DIR}/Scheduler/SocketsOps.cpp
    # ... 添加其他源文件  
) 

list(APPEND TEST_SOURCE_FILES  
    ${TEST_DIR}/BufferTest.cpp  
    # ... 添加其他源文件  
)

add_executable(server_test ${TEST_SOURCE_FILES})  

# 设置需要链接的库
target_link_libraries(server_test gtest gtest_main pthread)
