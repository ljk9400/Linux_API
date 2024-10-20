## bazel   
   
### 官方网址   
https://bazel.build/?hl=zh-cn   
   
### 安装 Bazel   
1. 安装 Java：Bazel 需要 Java 运行环境。      
sudo apt-get install openjdk-8-jdk   
2. 添加 Bazel 的仓库：      
echo "deb [arch=amd64] http://storage.googleapis.com/bazel-apt stable jdk1.8" | sudo tee /etc/apt/sources.list.d/bazel.list   
curl https://bazel.build/bazel-release.pub.gpg | sudo apt-key add -   
3. 安装 Bazel：      
sudo apt-get update && sudo apt-get install bazel   
   
### 使用 Bazel   
一旦安装了 Bazel，你可以按照以下步骤使用它：   
1. 初始化项目：      
在项目目录中运行以下命令以生成 WORKSPACE 文件和初始的构建文件。     
bazel init   
   
2. 构建目标：     
使用以下命令构建一个目标（target）。     
bazel build //path/to/package:target   
   
3. 运行测试：      
使用以下命令运行测试。     
bazel test //path/to/package:target_test   
   
4. 运行查询：     
你可以使用以下命令来查询构建文件中的目标。     
bazel query 'kind(rule, //path/to/...)'   
   
5. 清理输出：     
使用以下命令清理 Bazel 的输出目录。     
bazel clean   
   
### 生成   
- 生成库      
cc_library(      
    name = "hello-time",      
    srcs = ["hello-time.cc"],      
    hdrs = ["hello-time.h"],     
    visibility = ["//main:__pkg__"],     
)     
   
   
- 生成可执行文件   
cc_binary(   
    name = "hello-world",   
    srcs = ["hello-world.cc"],   
    deps = [   
        ":hello-greet",   
        "//lib:hello-time",   
    ],   
   
    copts = [   
        '-DAPI_XDDS',   
    ]   
)   
   
### 名词解释   
- name :包名   
- srcs :源文件   
- hdrs ：头文件   
- deps ：依赖   
- //:绝对路径， 在 MODULE.bazel，每个编译工程根目录下定义。   
- @: 引用外部依赖项。这通常用于引用在WORKSPACE文件中定义的依赖项。   
- //: 引用同一工作空间中的目标。这表示目标位于当前Bazel工作空间的某个包中。   
- :: 在同一BUILD文件中引用目标。这用于在同一BUILD文件中区分不同的目标。   
   
- 编译选择   
   
copts = [   
    '-DAPI_XDDS',   
    ]   
   
linkopts = select({   
    ":sa8295": ["-llog", "-Wl,-soname,libxdds.so", "-Wl,-Bsymbolic"],   
    "//conditions:default": ["-Wl,-Bsymbolic-functions"],   
}),   
   
- load   
load 语句用于在 BUILD 文件或 Bazel 的其他构建文件中导入 Starlark（Bazel 的内置编程语言）模块。这些模块通常包含定义函数、规则和宏的 .bzl 文件。   
重用代码：通过 load，你可以导入在其他 .bzl 文件中定义的函数、规则和宏，从而避免在多个构建文件中重复代码。   
组织代码：可以将相关的函数和规则分组到同一个 .bzl 文件中，这样有助于维护和组织大型项目。   
共享工具：如果你有一些常用的工具或实用程序函数，可以在一个 .bzl 文件中定义它们，并在需要的地方通过 load 导入。   
load 语句的语法如下：   
   
load("//path/to/file:target.bzl", "symbol1", "symbol2")   
//path/to/file: .bzl 文件的路径，通常是相对于 workspace 根目录的路径。   
target.bzl: 导入的 .bzl 文件名。   
"symbol1", "symbol2": 从指定文件中导入的一个或多个符号（函数、规则或宏的名称）。   
   
- struct   
CPU_NAMES = struct(   
    linux_intel = "k8",   
    gcc_x86 = 'gcc_x86',   
    nvidia_orin = 'nvidia_orin',   
    clang_arm64 = 'clang_arm64',   
    qnx = 'qnx',   
    tbox_aarch64 = "tbox_aarch64",   
    sa8295 = 'sa8295',   
)   
   
CPU_CONFIGS = {   
    CPU_NAMES.nvidia_orin: struct(   
        fast_cpp_compilation_flags = [   
            "-std=c++17",   
            "-DHAVE_CXX0X",   
        ],   
        fast_external_dependencies = [   
            "@fastcdr//:fastcdr",   
            "@xos//xdds/core/utils:fastidl_hdr",   
        ],   
        fast_linker_dependencies = [   
        ],   
        default_cpp_only_compilation_flags = [   
            "-std=c++17",   
        ],   
        default_compilation_flags = [   
            "-Wall",   
            "-fPIC",   
            "-D_AARCH64_SOURCE",   
            "-DBAZEL_COMPILE",   
            "-DXPILOT_LINUX",   
            "-DRTI_UNIX",   
            "-DRTI_LINUX",   
            "-DRTI_64BIT",   
            "-DRTI_STATIC",   
            "-no-canonical-prefixes",   
            "-fno-canonical-system-headers",   
        ],   
        default_linker_flags = [   
            "-lpthread",   
            "-lstdc++",   
            "-Dxpilot_clang",   
            "-ldl",   
            "-lrt",   
            "-lm",   
            "-lrt",   
        ],   
        cuda_nvcc_flags = [   
            "-ccbin",   
        ],   
        cuda_gcc_flags = [   
            "--std=c++17",   
            "-gencode arch=compute_70,code=sm_70",   
            "-gencode arch=compute_80,code=sm_80",   
            "-gencode arch=compute_86,code=sm_86",   
            "-gencode arch=compute_87,code=sm_87",   
            "-gencode arch=compute_86,code=compute_86",   
            "--use_fast_math",   
            "-Xcompiler",   
            "-fno-canonical-system-headers",   
            "-Xcompiler",   
            "-fPIC",   
            "-Xcompiler -Wall",   
            "-Xcompiler -W",   
            "--expt-extended-lambda -c",   
        ],   
    ),   
    CPU_NAMES.linux_intel: struct(   
          
    )   
}   
   
