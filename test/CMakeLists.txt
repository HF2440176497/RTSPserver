
# Google Test 测试程序

set(TEST_SOURCE_FILES  
    ${SOURCE_FILES}  
    BufferTest.cpp
    # ... test目录下的其他源文件 ...  
) 

add_executable(server_test ${TEST_SOURCE_FILES})  

# 设置需要链接的库
target_link_libraries(server_test gtest gtest_main pthread)
