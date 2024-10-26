#*****************************************************************************
#*    (c) 2005-2018 Copyright, LJK, All rights reserved.              *
#*                                                                           *
#*         Permission to modify and use for internal purposes granted.       *
#* This software is provided "as is", without warranty, express or implied.  *
#*                                                                           *
#*****************************************************************************

load("@build_tools//:target_paths.bzl", "PATH_NAMES")  


CPU_NAMES = struct(   
    linux_intel = "k8",   
    gcc_x86 = 'gcc_x86',   
)   
   
CPU_CONFIGS = {   
    CPU_NAMES.nvidia_orin: struct(   
        fast_cpp_compilation_flags = [   
            "-std=c++17",   
            "-DHAVE_CXX0X",   
        ],   
        fast_external_dependencies = [   
            "@fastcdr//:fastcdr",   
        ],   
        default_cpp_only_compilation_flags = [   
            "-std=c++17",   
        ],   
        default_compilation_flags = [   
            "-Wall",   
            "-fPIC",   
            "-D_AARCH64_SOURCE",   
        ],   
        default_linker_flags = [   
            "-lpthread",   
            "-lstdc++",   
        ],   
    ),   
    CPU_NAMES.linux_intel: struct(    # 第二个结构体参数  
    )    
}   