#设置版本 
cmake_minimum_required(VERSION 3.4.1) 
project(base_network)

# 输出路径
set(TARGET_OUTPUT ${CMAKE_SOURCE_DIR}/bin)
set(TARGETS 
    "socket_tcp_client"    "socket_tcp_server" 
    "socket_server_epoll"  "socket_server_select" "socket_server_poll" 
    "socket_server_poll_thread" 
    "socket_udp_server" "socket_udp_client"
)

set(TARGET1 "network_com")

# 包含头文件
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/socket)

# 添加执行文件
foreach(TARGET ${TARGETS})
    add_executable(${TARGET} ${CMAKE_CURRENT_SOURCE_DIR}/socket/${TARGET}.c)
    target_link_libraries(${TARGET} -lpthread)
endforeach()

add_executable(${TARGET1} 
    ${CMAKE_CURRENT_SOURCE_DIR}/${TARGET1}.c
) 

# cmake阶段设置输出路径
set_target_properties(${TARGETS} ${TARGET1}
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${TARGET_OUTPUT}
)











