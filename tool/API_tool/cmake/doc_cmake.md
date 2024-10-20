### camke
https://mp.weixin.qq.com/s/B4jISWt7M1wBh-1WR93eew

1.1 camke模板
```cmake
#设置版本 
cmake_minimum_required(VERSION 3.4.1) 
project(myspdlog)  
#设置目录-以此目录为基准设置下面编译的头，源，库位置、CMakeLists.txt所在目录 
set(CURRENT_DIR ${CMAKE_SOURCE_DIR}) 
#设置编译器 
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99") 
#生成gdb的调试信息和不进行代码优化 
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -g") 

set(CMAKE_CXX_STANDARD 11)
# 添加编译选项，启用调试信息和生成调试符号
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
# 添加链接选项，生成调试信息
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -g")


# 开源库
#include_directories(./thirdparty/protobuf/include)
#link_directories(./thirdparty/protobuf/lib)

# 搜索指定的库文件
#find_library(LIB_PROTOBUF protobuf HINTS ./thirdparty/protobuf/lib)


#包含源文件 ，aux_source_directory将目录所有源文件自动加入 
#aux_source_directory(${CMAKE_SOURCE_DIR}/src/common SOURCE_COMMON) 


# 添加头文件搜索路径
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libspdlog/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libspdlog/include/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libspdlog/include/adapter)

#包含源文件 ，aux_source_directory将目录所有源文件自动加入 
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/libspdlog/adapter/ SOURCE_COMMON) 
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/libspdlog/         SOURCE_API) 

#生成IDPS可执行，这里直接放源文件也行如main.c；太多话，可以设置目录例如SOURCE_COMMON。 
add_executable(myspdlog 
        ${CMAKE_CURRENT_SOURCE_DIR}/spdlog_test.c
        ${SOURCE_API}
        ${SOURCE_COMMON} 
    ) 

#链接我们需要的库，如lpthread系统库，预编译好的库lspdlog，需要2.5设置好路径 
target_link_libraries( myapp 
#        -lspdlog 
#        -lpthread 
#        -lstdc++ 
    ) 

# 设置输出路径
set_target_properties(myspdlog PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../bin)

set_target_properties(client PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
    
# 设置 server 的构建产物输出目录
set_target_properties(server PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set_target_properties(server PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

# 设置 client 的构建产物输出目录
set_target_properties(client PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set_target_properties(client PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

# 将头文件复制到输出目录
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/libspdlog/spdloglib.h ${CMAKE_CURRENT_SOURCE_DIR}/../bin COPYONLY)

# 添加子目录，包含 gtest 的构建
add_subdirectory(base64)
add_subdirectory(cjson)
add_subdirectory(spdlog)
add_subdirectory(sqlite3)
add_subdirectory(zlog)
```

1.2 注册安装
```cmake
install(
    TARGETS node1 node2 dagnode1 dagnode2 node_server node_client app_control datarec_demo
    RUNTIME DESTINATION $ENV{INSTALL_DIR}/bin
    LIBRARY DESTINATION $ENV{INSTALL_DIR}/lib)

install(
    DIRECTORY config
    DESTINATION DESTINATION $ENV{INSTALL_DIR})
```

1.3 生成静态库、动态库
```cmake
# 创建名为 "mylib" 的静态库
add_library(mylib STATIC ${SOURCE_FILES})


# 创建名为"mylib"的动态库
add_library(mylib SHARED ${SOURCE_FILES})

# 指定静态库的公开头文件目录
target_include_directories(mylib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
```

1.4 c++、c
```cmake
# 设置当前源代码目录变量
set(CURRENT_DIR ${CMAKE_SOURCE_DIR})
# 设置 C++ 编译器标志
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11") # 选择合适的C++标准
# 生成 GDB 调试信息和不进行代码优化
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g")


set(CURRENT_DIR ${CMAKE_SOURCE_DIR}) 
#设置编译器 
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99") 
#生成gdb的调试信息和不进行代码优化 
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -g") 
1.5 添加其他模块下子模块
A文件夹下cmake包含B文件夹下cmake
add_subdirectory(../../B B_build)
B_build为当前目录下生成目录名称，随便起；当B下cmake是指定相对目录，B_build不生效
```

1.6 交叉编译
```cmake
cmake_minimum_required(VERSION 3.0)
project(MyProject)

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)

# 设置编译选项
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

# 指定交叉编译工具链位置
set(CROSS_COMPILE_PATH /home/lixiang/ljk_test/my_Tool/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-linux-gnu)
set(CMAKE_C_COMPILER   ${CROSS_COMPILE_PATH}/bin/aarch64-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER ${CROSS_COMPILE_PATH}/bin/aarch64-linux-gnu-g++)

# 添加头文件目录
include_directories(${CROSS_COMPILE_PATH}/aarch64-linux-gnu/include)
#include_directories(/path/to/your_project/include)

# 添加库文件目录
link_directories(${CROSS_COMPILE_PATH}/aarch64-linux-gnu/lib)
#link_directories(/path/to/your_project/lib)

add_executable(core_test 
    core_test.cpp)

# 链接你的项目所需要的库，例如：libmylib.a or -lmylib
#target_link_libraries(MyProject mylib)
```

1.7 执行
./cmake    生成makefile
make        编译
主要camke对大小写不敏感
 
1.8 常用变量
CMAKE_SOURCE_DIR：根路径
CMAKE_CURRENT_SOURCE_DIR：当前路径
CMAKE_MAJOR_VERSION：cmake 主版本号
CMAKE_MINOR_VERSION：cmake 次版本号
CMAKE_C_FLAGS：设置 C 编译选项
CMAKE_CXX_FLAGS：设置 C++ 编译选项
PROJECT_SOURCE_DIR：工程的根目录
PROJECT_BINARY_DIR：运行 cmake 命令的目录
CMAKE_CURRENT_SOURCE_DIR：当前CMakeLists.txt 所在路径
CMAKE_CURRENT_BINARY_DIR：目标文件编译目录
EXECUTABLE_OUTPUT_PATH：重新定义目标二进制可执行文件的存放位置
LIBRARY_OUTPUT_PATH：重新定义目标链接库文件的存放位置
UNIX：如果为真，表示为UNIX-like的系统，包括AppleOSX和CygWin
WIN32：如果为真，表示为 Windows 系统，包括 CygWin
APPLE：如果为真，表示为 Apple 系统
CMAKE_SIZEOF_VOID_P：表示void*的大小（例如为4或者8），可以使用其来判断当前构建为32位还是64位
CMAKE_CURRENT_LIST_DIR：表示正在处理的CMakeLists.txt文件所在目录的绝对路径
CMAKE_ARCHIVE_OUTPUT_DIRECTORY：用于设置ARCHIVE目标的输出路径
CMAKE_LIBRARY_OUTPUT_DIRECTORY：用于设置LIBRARY目标的输出路径
CMAKE_RUNTIME_OUTPUT_DIRECTORY：用于设置RUNTIME目标的输出路径
 
1.9 常用命令
(1) project命令：
命令语法:project(<projectname> [languageName1 languageName2 ...])
命令简述:用于指定项目的名称
使用范例:project(Main)
 
(2) cmake_minimum_required命令:
命令语法:cmake_minimum_requried(VERSION major[.minor[.patch)
命令简述:用于指定需要的CMake的最低版本
使用范例:cmake_minimum_requried(VERSION 2.8.3)
 
(3) aux_source_directory命令:
命令语法:aux_source_directory(<dir> <variable>)
命令简述:用于包含源文件目录，dir目录下的所有源文件的名字保存在变量variable中
使用范例：aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src  DIR_SRCS)
 
(4) add_executable命令:
命令语法：add_executable(<name> [WIN32] [MACOSX_BUNDLE][EXCLUDE_FROM_ALL] source1 source2 … sourceN)
命令简述:用于指定从一组源文件source1 source2 ... sourceN 编译出一个可执行文件且命名为name
使用范例:add_executable(Main $(DIR_SRCS))
 
(5) add_library命令:
命令语法：add_library([STATIC | SHARED | MODULE] [EXCLUDE_FROM_ALL] source1source2 … sourceN)
命令简述：用于指定从一组源文件 source1 source2 ... sourceN编译出一个库文件且命名为name
使用范例：add_library(Lib $(DIR_SRCS))
 
(6) add_dependencies命令:
命令语法：add_dependencies(target-name depend-target1 depend-target2 …)
命令简述：用于指定某个目标（可执行文件或者库文件）依赖于其他的目标。
        这里的目标必须是add_executable、add_library、add_custom_target命令创建的目标
 
(7) add_subdirectory命令:
命令语法：add_subdirectory(source_dir [binary_dir] [EXCLUDE_FROM_ALL])
命令简述:用于添加一个需要进行构建的子目录
使用范例:add_subdirectory(Lib)
 
(8) target_link_libraries命令:
命令语法：target_link_libraries(<target> [item1 [item2 […]]][[debug|optimized|general] ] …)
命令简述:用于指定target需要链接item1 item2 ...。这里target必须已经被创建，链接的item可以是已经存在的target（依赖关系会自动添加）
使用范例:target_link_libraries(Main Lib)
 
(9) set命令：
命令简述：用于设定变量 variable 的值为 value。如果指定了 CACHE 变量将被放入 Cache（缓存）中。
命令语法：set(<variable> <value> [[CACHE <type><docstring> [FORCE]] | PARENT_SCOPE])
使用范例：set(ProjectName Main)
 
(10) unset命令：
命令语法：unset(<variable> [CACHE])
命令简述：用于移除变量 variable。如果指定了 CACHE 变量将被从 Cache 中移除。
使用范例：unset(VAR CACHE)
 
(11) message命令：
命令语法：message([STATUS|WARNING|AUTHOR_WARNING|FATAL_ERROR|SEND_ERROR] “message todisplay”…)
命令简述:用于输出信息
使用范例:message(“Hello World”)
 
(12) include_directories命令：
命令语法：include_directories([AFTER|BEFORE] [SYSTEM] dir1 dir2 …)
命令简述：用于设定目录，这些设定的目录将被编译器用来查找 include 文件
使用范例：include_directories(${PROJECT_SOURCE_DIR}/lib)
 
(13) find_path命令：
命令语法：find_path(<VAR> name1 [path1 path2 …])
命令简述：用于查找包含文件name1的路径，如果找到则将路径保存在VAR中（此路径为一个绝对路径），如果没有找到则结果为<VAR>-NOTFOUND.默认情况下，VAR会被保存在Cache中，这时候我们需要清除VAR才可以进行下一次查询（使用unset命令）
find_path(LUA_INCLUDE_PATH lua.h ${LUA_INCLUDE_FIND_PATH})
if(NOT LUA_INCLUDE_PATH)
     message(SEND_ERROR "Header file lua.h not found")
endif()

(14) find_library命令：
命令语法：find_library(<VAR> name1 [path1 path2 …])
命令简述：用于查找库文件 name1 的路径，如果找到则将路径保存在 VAR 中（此路径为一个绝对路径），
        如果没有找到则结果为 <VAR>-NOTFOUND。
        一个类似的命令 link_directories 已经不太建议使用了
 
(15) add_definitions命令：
命令语法：add_definitions(-DFOO -DBAR …)
命令简述：用于添加编译器命令行标志（选项），通常的情况下我们使用其来添加预处理器定义
使用范例：add_definitions(-D_UNICODE -DUNICODE)
 
(16) file命令：
命令简述：此命令提供了丰富的文件和目录的相关操作（这里仅说一下比较常用的）
使用范例：
目录的遍历
- GLOB 用于产生一个文件（目录）路径列表并保存在variable 中
- 文件路径列表中的每个文件的文件名都能匹配globbing expressions（非正则表达式，但是类似）
- 如果指定了 RELATIVE 路径，那么返回的文件路径列表中的路径为相对于 RELATIVE 的路径
file(GLOB variable [RELATIVE path][globbing expressions]...)
- 获取当前目录下的所有的文件（目录）的路径并保存到 ALL_FILE_PATH 变量中
file(GLOB ALL_FILE_PATH ./*)
- 获取当前目录下的 .h 文件的文件名并保存到ALL_H_FILE 变量中
- 这里的变量CMAKE_CURRENT_LIST_DIR 表示正在处理的 CMakeLists.txt 文件的所在的目录的绝对路径（2.8.3 以及以后版本才支持）
 
1.10 设置选项
1、添加宏定义到C文件
add_definitions(-DSERVER_TEST)
-D后面为添加宏
2、设置 strip
strip设置减少编译大小
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -s")
set(CMAKE_BUILD_TYPE "RELEASE")
3、选择设置
option(BUILD_SERVER "Build test application" ON)
if(BUILD_SERVER)
    #操作1
else()
    #操作2
endif()

1.11 cmake_policy
CMake 政策（policies）是一种管理不同版本的 CMake 之间兼容性的方法。随着 CMake 的发展，某些行为和特性可能会发生变化，政策提供了一种机制，可以以受控的方式处理这些变化。通过使用政策，可以确保您的构建脚本在不同版本的 CMake 上保持一致的行为。
cmake_policy(VERSION)
设置 CMake 版本，这会隐式地将该版本之前引入的政策设置为 NEW。
cmake_policy(VERSION 3.18)
cmake_policy(SET)
显式将某个政策设置为 OLD 或 NEW。
cmake_policy(SET CMP0048 NEW)
cmake_policy(PUSH) 和 cmake_policy(POP)
保存和恢复当前的政策设置，这对于在特定范围内临时更改政策非常有用。
cmake_policy(PUSH)
cmake_policy(SET CMP0048 OLD)
一些 CMake 命令
cmake_policy(POP)
使用示例
下面是一个在 CMake 项目中管理政策的示例：
cmake_minimum_required(VERSION 3.10)
project(MyProject)
设置 CMake 政策到特定版本
cmake_policy(VERSION 3.10)
显式将政策 CMP0048 设置为 NEW
cmake_policy(SET CMP0048 NEW)
在某个范围内临时更改政策
cmake_policy(PUSH)
cmake_policy(SET CMP0063 NEW)
需要新政策行为的一些命令
cmake_policy(POP)
检查政策状态
您可以使用 cmake_policy(GET) 检查当前的政策状态：
cmake_policy(GET CMP0048 status)
message(STATUS "CMP0048 is set to ${status}")
