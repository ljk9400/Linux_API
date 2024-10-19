/*
 * 文件名: LinuxFileIO.c
 * 作者: ljk
 * 创建时间: 2023-07-15
 * 文件描述: 文件链接操作例程
 */
#include <unistd.h> 
#include <stdlib.h>    // For NULL
#include <stddef.h>    // For size_t and NULL
#include <sys/stat.h>  // For struct stat and file-related functions
#include <sys/types.h> // For uid_t, gid_t, and other types
#include <fcntl.h>
#include <stdio.h>
#include <limits.h>
#include "linux_link.h"
#include "../base_common.h"




// 建立测试文件
static int new_establish_file_link(const char* path) {
    const char* file_path = path;
    // 打开文件，以追加模式打开，如果文件不存在则创建
    FILE* file = fopen(file_path, "a");
    if (file == NULL) {
        printf("无法创建文件！\n");
        return -1;
    }
    // 关闭文件
    fclose(file);
    printf("文件创建成功！\n");
    return 0;
}

// 更改权限
int api_chmod(const char* path, mode_t mode) {
    const char* file_path = path;
    mode_t file_mode = mode;
    if (chmod(path, mode) == 0) {
        printf("权限修改成功！\n");
    } else {
        perror("chmod 失败");
        return 1;
    }
    return 0;
}

// 更改用户和组
int api_chown(const char* path, uid_t owner, gid_t group) {
    const char *file_path = path;
    uid_t file_owner = owner;  // 新的所有者用户 ID
    gid_t file_group = group;  // 新的所属群组群组 ID
    if (chown(file_path, file_owner, file_group) == 0) {
        printf("所有者和群组修改成功！\n");
    } else {
        perror("chown 失败");
        return 1;
    }
    return 0;
}

// 硬链接 link  <原文件路径> <目标文件路径>
// 软连接 ln -s <原文件路径> <符号链接路径>
// 创建硬链接
int api_hardlink(const char *src, const char *dest) {
    if (link(src, dest) == 0) {
        printf("硬链接创建成功：%s -> %s\n", src, dest);
    } else {
        perror("硬链接创建失败");
        return 1;
    }
    return 0;
}

// 创建软链接
int api_symlink(const char *src, const char *dest) {
    if (symlink(src, dest) == 0) {
        printf("软链接创建成功：%s -> %s\n", src, dest);
    } else {
        perror("软链接创建失败");
        return 1;
    }
    return 0;
}

// 删除链接
int api_unlink(const char *path) {
    if (unlink(path) == 0) {
        printf("链接删除成功：%s\n", path);
    } else {
        perror("链接删除失败");
        return 1;
    }
    return 0;
}

int fun_chmod() {
    int mode = 0644;
    char* path = FILE_NAME_LINK;
    return api_chmod(path, mode);
}

int fun_chown() {
    int owner = 1001;
    int group = 1002;
    char* path = FILE_NAME_LINK;
    return api_chown(path, owner, group);
}

int fun_link() {
    // 使用预处理指令 # 进行字符串拼接
    #define STRINGIFY(x,y) x#y
    const char *filepath = FILE_NAME_LINK;
    const char *softpath = STRINGIFY(FILE_NAME_LINK, .softlink);;
    const char *hardpath = STRINGIFY(FILE_NAME_LINK, .hardlink);
    if (link(filepath, softpath) == 0) {
        printf("硬链接创建成功！\n");
    } else {
        perror("link 失败");
    }
    if (symlink(filepath, hardpath) == 0) {
        printf("软链接创建成功！\n");
    } else {
        perror("symlink 失败");
    }
#if 1
    if (unlink(filepath) == 0) {
        printf("文件删除成功！\n");
    } else {
        perror("unlink 失败");
    }
    if (unlink(softpath) == 0) {
        printf("软连接删除成功！\n");
    } else {
        perror("unlink 失败");
    }
    if (unlink(hardpath) == 0) {
        printf("硬链接删除成功！\n");
    } else {
        perror("unlink 失败");
    }
#endif
    return 0;
}

// 测试例程
int main_test(int argc, char* argv[]) {
    PRINT_CMD_ARGS(argc, argv);
    if(new_establish_file_link(FILE_NAME_LINK) == RET_OK) {
        FUNCTION_PRINT_API(fun_chmod);
        FUNCTION_PRINT_API(fun_chown);
        FUNCTION_PRINT_API(fun_link);
    }
    return 0;
}

// main函数
int main(int argc, char* argv[])
{
    main_test(argc, argv);
    return 0;
}