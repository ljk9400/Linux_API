#!/bin/bash

# 使用说明
function usage() {
    echo "Usage: $0 {b|r|clean} [module]"
    echo "  b     - 编译项目"
    echo "  r     - 清除并重新建立项目"
    echo "  clean - 清空项目"
    echo "  module - 可选，指定要编译的单独模块:"
    echo "    f - base_file"
    echo "    p - base_process"
    echo "    r - base_resource"
    echo "    n - base_network"
    echo "    c - clock"
    echo "    t - tiny"
    echo "    无参数- 编译全部"
    exit 1
}

# 创建或清空 build 目录
function prepare_build_dir() {
    if [ -d "build" ]; then
        echo "清空 build 目录"
        rm -rf build/*
        rm -rf bin/*
    else
        echo "创建 build 目录"
        mkdir build
    fi
}

# 配置 CMake
function configure_cmake() {
    case "$1" in
        f)
            cmake -DCOMPILE_BASE_FILE=ON ..
            ;;
        p)
            cmake -DCOMPILE_BASE_PROCESS=ON ..
            ;;
        r)
            cmake -DCOMPILE_BASE_RESOURCE=ON ..
            ;;
        n)
            cmake -DCOMPILE_BASE_NETWORK=ON ..
            ;;
        c)
            cmake -DCOMPILE_CLOCK_TIME=ON ..
            ;;
        t)
            cmake -DCOMPILE_TINY=ON ..
            ;;
        *)
            cmake -DCOMPILE_BASE_FILE=ON \
                  -DCOMPILE_BASE_PROCESS=ON \
                  -DCOMPILE_BASE_RESOURCE=ON \
                  -DCOMPILE_BASE_NETWORK=ON \
                  -DCOMPILE_CLOCK_TIME=ON \
                  -DCOMPILE_TINY=ON \
                  -DCOMPILE_COMMON=ON \
                  ..
            ;;
    esac
}

# 编译项目
function build_project() {
    prepare_build_dir
    cd build || exit
    configure_cmake "$1"
    make
}

# 清空项目
function clean_project() {
    if [ -d "build" ]; then
        echo "清空 build 目录"
        rm -rf build/*
        rm -rf bin/*
    else
        echo "build 目录不存在"
    fi
}

# 清除并重新建立项目
function rebuild_project() {
    clean_project
    build_project "$1"
}

# 检查输入参数
if [ $# -eq 0 ]; then
    usage
fi

# 根据输入参数执行相应操作
case "$1" in
    b)
        build_project "$2"
        ;;
    r)
        rebuild_project "$2"
        ;;
    clean)
        clean_project
        ;;
    *)
        usage
        ;;
esac