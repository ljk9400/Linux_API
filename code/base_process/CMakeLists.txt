#设置版本 
cmake_minimum_required(VERSION 3.4.1) 
project(base_process)

#根路径
#CMAKE_SOURCE_DIR
#当前路径
#CMAKE_CURRENT_SOURCE_DIR}

#设置目录-以此目录为基准设置下面编译的头，源，库位置、CMakeLists.txt所在目录 
set(CURRENT_DIR ${CMAKE_SOURCE_DIR}) 
#设置编译器 
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99") 
#生成gdb的调试信息和不进行代码优化 
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -g") 
# 输出路径
set(TARGET_OUTPUT ${CMAKE_SOURCE_DIR}/bin)
# 设置目标
set(TARGETS  "linux_getopt" 
    "linux_process"       "linux_process_exe" 
    "linux_thread_mutex"  "linux_signal"
)
set(TARGETS_IPC 
    "linux_ipc_fifo_read" "linux_ipc_fifo_write" 
    "linux_ipc_shm_read"  "linux_ipc_shm_write" 
    "linux_ipc_smg_read"  "linux_ipc_smg_write" 
    "linux_ipc_pipe" 
)

# 包含头文件
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/ipc)

# 添加执行文件
foreach(TARGET ${TARGETS})
    add_executable(${TARGET} ${CMAKE_CURRENT_SOURCE_DIR}/${TARGET}.c)
endforeach()
foreach(TARGETS_IPC ${TARGETS_IPC})
    add_executable(${TARGETS_IPC} ${CMAKE_CURRENT_SOURCE_DIR}/ipc/${TARGETS_IPC}.c)
endforeach()

# 链接动态库
target_link_libraries(linux_thread_mutex 
    -lpthread
)  

# cmake阶段设置输出路径
set_target_properties(${TARGETS} ${TARGETS_IPC}
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${TARGET_OUTPUT})

# 安装目标
#install(${TARGETS}    RUNTIME DESTINATION ${TARGET_OUTPUT})
