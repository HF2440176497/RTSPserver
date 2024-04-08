

# 初始化一个变量来存储所有的源文件  
set(SOURCE_FILES)  
  
# # 递归地获取所有子目录中的源文件  
# function(add_source_files dir)  
#     file(GLOB_RECURSE src_list "${dir}/*.cpp")  
#     list(APPEND SOURCE_FILES ${src_list})  
# endfunction()

# add_source_files(${CMAKE_CURRENT_SOURCE_DIR})  

list(APPEND SOURCE_FILES  
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp  
    # ... 添加其他源文件  
)

list(APPEND SOURCE_FILES  
    ${CMAKE_CURRENT_SOURCE_DIR}/Scheduler/SocketsOps.cpp
    # ... 添加其他源文件  
) 

# 添加可执行文件
add_executable(server ${SOURCE_FILES})  

# 设置需要链接的库
target_link_libraries(server pthread)
