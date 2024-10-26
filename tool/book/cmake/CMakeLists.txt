### camke模板
# 设置版本 
cmake_minimum_required(VERSION 3.4.1) 
p roject(myspdlog)  
# 设置目录-以此目录为基准设置下面编译的头，源，库位置、CMakeLists.txt所在目录 
set(CURRENT_DIR ${CMAKE_SOURCE_DIR}) 
# 设置编译器 
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99") 
# 生成gdb的调试信息和不进行代码优化 
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -g") 
# C++编译器标志
set(CMAKE_CXX_STANDARD 11)
# 添加编译选项，启用调试信息和生成调试符号
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
# 添加链接选项，生成调试信息
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -g")



### 交叉编译
cmake_minimum_required(VERSION 3.0)
project(MyProject)

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)

# 设置编译选项
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

# 指定交叉编译工具链位置
set(CROSS_COMPILE_PATH /home/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-linux-gnu)
set(CMAKE_C_COMPILER   ${CROSS_COMPILE_PATH}/bin/aarch64-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER ${CROSS_COMPILE_PATH}/bin/aarch64-linux-gnu-g++)

# 添加头文件目录
include_directories(${CROSS_COMPILE_PATH}/aarch64-linux-gnu/include)
# 添加库文件目录
link_directories(${CROSS_COMPILE_PATH}/aarch64-linux-gnu/lib)



# 开源库
link_directories(./thirdparty/protobuf/lib)
# 搜索指定的库文件
find_library(LIB_PROTOBUF protobuf HINTS ./thirdparty/protobuf/lib)


# 添加头文件搜索路径
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libspdlog/)
# 包含源文件 ，aux_source_directory将目录所有源文件自动加入 
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/libspdlog/ SOURCE_API) 

#生成IDPS可执行，这里直接放源文件也行如main.c；太多话，可以设置目录例如SOURCE_COMMON。 
add_executable(myspdlog 
    ${CMAKE_CURRENT_SOURCE_DIR}/spdlog_test.c
    ${SOURCE_API}
) 

#链接我们需要的库，如lpthread系统库，预编译好的库lspdlog，需要提前设置好库路径 
target_link_libraries( myapp 
    -lspdlog 
    -lpthread 
    -lstdc++ 
) 

# 创建名为 "mylib" 的静态库
add_library(mylib STATIC ${SOURCE_FILES})
# 创建名为"mylib"的动态库
add_library(mylib SHARED ${SOURCE_FILES})

# 指定静态库的公开头文件目录
target_include_directories(mylib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)




# 设置编译目标
install(
    TARGETS
    RUNTIME DESTINATION $ENV{INSTALL_DIR}/bin
    LIBRARY DESTINATION $ENV{INSTALL_DIR}/lib
)

# 设置输出路径
set_target_properties(
    ${TARGET} sqlite3
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${BIN_OUTPUT_PATH} # 设置动态库运行时的输出目录。
    LIBRARY_OUTPUT_DIRECTORY ${BIN_OUTPUT_PATH} # 设置动态库本身的输出目录
    ARCHIVE_OUTPUT_DIRECTORY ${BIN_OUTPUT_PATH} # 设置静态库的输出目录
)

# 将头文件复制到输出目录
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/zlog.conf 
    ${BIN_OUTPUT_PATH}/zlog.conf COPYONLY
)




### 子模块
add_subdirectory(../../B_build)
