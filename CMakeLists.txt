# 设置版本 
cmake_minimum_required(VERSION 3.4.1) 
project(app_demo)

# 添加选项来选择编译的子模块
option(COMPILE_BASE_FILE     "Compile base_file module" OFF)
option(COMPILE_BASE_PROCESS  "Compile base_proc module" OFF)
option(COMPILE_BASE_RESOURCE "Compile base_reso module" OFF)
option(COMPILE_BASE_NETWORK  "Compile base_netw module" OFF)
option(COMPILE_CLOCK_TIME    "Compile cloc module" OFF)
option(COMPILE_TINY    "Compile tiny module" OFF)
option(COMPILE_COMMON  "Compile tiny module" ON)


# 根据选项添加子目录
add_subdirectory(common)

if(COMPILE_BASE_FILE)
    add_subdirectory(code/base_file)
endif()

if(COMPILE_BASE_PROCESS)
    add_subdirectory(code/base_process)
endif()

if(COMPILE_BASE_RESOURCE)
    add_subdirectory(code/base_resource)
endif()

if(COMPILE_BASE_NETWORK)
    add_subdirectory(code/base_network)
endif()

if(COMPILE_CLOCK_TIME)
    add_subdirectory(code/clock_time)
endif()

if(COMPILE_TINY)
    add_subdirectory(code/tiny)
endif()








