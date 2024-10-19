#设置版本 
cmake_minimum_required(VERSION 3.4.1) 
project(common)

#设置目录-以此目录为基准设置下面编译的头，源，库位置、CMakeLists.txt所在目录 
set(CURRENT_DIR ${CMAKE_SOURCE_DIR}) 
#设置编译器 
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99") 
#生成gdb的调试信息和不进行代码优化 
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -g") 


# 输出路径
set(TARGET_OUTPUT ${CMAKE_SOURCE_DIR}/bin)

# 包含头文件
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/)

# 获取当前目录下的所有 .c 文件
file(GLOB C_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.c")

# 添加执行文件并设置输出路径
foreach(C_FILE ${C_FILES})
    # 获取文件名（不带路径和扩展名）
    get_filename_component(TARGET ${C_FILE} NAME_WE)
    add_executable(${TARGET} ${C_FILE})
    set_target_properties(${TARGET}
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${TARGET_OUTPUT})
endforeach()

# 链接库
foreach(C_FILE ${C_FILES})
    get_filename_component(TARGET ${C_FILE} NAME_WE)
    if(${TARGET} STREQUAL "linux_gui_curses")
        target_link_libraries(${TARGET} -lcurses)
    endif()
endforeach()












