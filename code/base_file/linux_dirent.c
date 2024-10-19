/*
 * 文件名: LinuxDir.c
 * 作者: ljk
 * 创建时间: 2023-07-15
 * 文件描述: 目录寻找操作例程
 */
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include "../base_common.h"
#include "linux_dirent.h"




// 遍历文件
int api_finddir(char* dir, int depth) {
    DIR *dirp = NULL;
    struct stat statbuf;
    struct dirent *entry = NULL;
    char original_path[PATH_MAX];

    // 记录当前工作目录
    if (getcwd(original_path, sizeof(original_path)) == NULL) {
        perror("getcwd 失败");
        return 1;
    }

    if ((dirp = opendir(dir)) == NULL) {
        fprintf(stderr, "无法打开目录: %s\n", dir);
        return 1;
    }

    chdir(dir); // 切换目录
    while ((entry = readdir(dirp)) != NULL) {
        lstat(entry->d_name, &statbuf);
        if (S_ISDIR(statbuf.st_mode)) {
            if (strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0)
                continue;
            printf("%*s%s/\n", depth, "", entry->d_name);
            api_finddir(entry->d_name, depth + 4);
        } else {
            printf("%*s%s\n", depth, "", entry->d_name);
        }
    }
    chdir(original_path); // 恢复到原来的工作目录
    closedir(dirp);
    return 0;
}

// 记录文件位置，从这个位置再次打印
int api_seekdir(char* dir, char* target) {
    DIR *dirp;
    struct dirent *entry;
    long position;
    // 要查找的目标文件名
    char* targetFile = target; 

    // 打开目录
    dirp = opendir(dir);
    if (dirp == NULL) {
        perror("opendir failed");
        return 1;
    }

    // 遍历目录并记录当前位置
    while ((entry = readdir(dirp)) != NULL) {
        printf("File: %s\n", entry->d_name);
        // 检查是否找到目标文件
        if (strcmp(entry->d_name, targetFile) == 0) {
            printf("发现文件: %s\n", targetFile);
            // 记录当前位置
            position = telldir(dirp);
        }
    }

    // 恢复到之前记录的位置
    printf("--重新定位--:\n");
    seekdir(dirp, position);

    // 继续遍历剩余的目录项
    while ((entry = readdir(dirp)) != NULL) {
        printf("File: %s\n", entry->d_name);
    }
    // 关闭目录
    closedir(dirp);
    return 0;
}

// 创建目录
int api_mkdir(const char *path, unsigned int mode) {
    if (mkdir(path, mode) == 0) {
        printf("目录创建成功：%s\n", path);
    } else {
        perror("mkdir 失败");
        return 1;
    }
    return 0;
}

// 删除目录
int api_rmdir(const char *path) {
    if (rmdir(path) == 0) {
        printf("目录删除成功：%s\n", path);
    } else {
        perror("rmdir 失败");
        return 1;
    }
    return 0;
}

// 获取当前工作目录
int api_getcwd() {
    char buf[PATH_MAX];
    if (getcwd(buf, sizeof(buf)) != NULL) {
        printf("当前工作目录：%s\n", buf);
    } else {
        perror("getcwd 失败");
        return 1;
    }
    return 0;
}

// 更改当前工作目录
int api_chdir(const char *path) {
    if (chdir(path) == 0) {
        printf("当前工作目录修改成功：%s\n", path);
    } else {
        perror("chdir 失败");
        return 1;
    }
    return 0;
}

int fun_finddir() { 
    return api_finddir(API_DIR_PATH, 0);
}

int fun_seekdir() {
    return api_seekdir(API_DIR_PATH, "Makefile");
}

int fun_mkdir() {
    const char* mk_dir = MKD_DIR_NAME;
    // 创建目录
    api_mkdir(mk_dir, 0777);
    // 获取当前工作目录
    api_getcwd();
    // 更改当前工作目录
    api_chdir(mk_dir);
    // 获取当前工作目录
    api_getcwd();
    // 返回上级目录
    api_chdir("..");
#if 1
    // 删除目录
    api_rmdir(mk_dir);
#endif
    return 0;
}

// test函数
int main_test(int argc, char* argv[]) {
    PRINT_CMD_ARGS(argc, argv);
    FUNCTION_PRINT_API(fun_finddir);
    FUNCTION_PRINT_API(fun_seekdir);
    FUNCTION_PRINT_API(fun_mkdir);
    return 0;
}

// main函数
int main(int argc, char* argv[])
{
    main_test(argc, argv);
    return 0;
}