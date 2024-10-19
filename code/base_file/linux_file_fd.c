/*
 * 文件名: LinuxFileIO.c
 * 作者: ljk
 * 创建时间: 2023-07-15
 * 文件描述: 文件IO读写操作例程,  POSIX 系统调用
 */
// 0、1 和 2 标准输入、标准输出和标准错误
#include <unistd.h>   // For access, read, write, close
#include <fcntl.h>    // For open, O_RDONLY, O_WRONLY, O_CREAT, O_TRUNC
#include <stdlib.h>   // For exit
#include <string.h>   // For strlen
#include <sys/stat.h> // For S_IRUSR, S_IWUSR
#include <stdio.h>
#include "linux_file_fd.h"
#include "../base_common.h"


// 文件权限确认
int api_access(const char* path) {
    if (access(path, R_OK | W_OK | X_OK | F_OK) != 0) {
        perror("access");
        return 1;
    }
    printf("File %s is accessible\n", path);
    return 0;
}

// 文件IO写
int api_write(const char* path, const char* data) {
    int fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("open");
        return 1;
    }
    if (write(fd, data, strlen(data)) == -1) {
        perror("write");
        close(fd);
        return 1;
    }
    if (fsync(fd) == -1) {
        perror("fsync");
        close(fd);
        return 1;
    }
    if (close(fd) == -1) {
        perror("close");
        return 1;
    }
    printf("write: %s, len: %ld\n", data, strlen(data));
    return 0;
}

// 文件IO读
int api_read(const char* path) {
    char buffer[256];
    int fd = open(path, O_RDONLY);
    if (fd == -1) {
        perror("open");
        return 1;
    }
    ssize_t bytesRead = read(fd, buffer, sizeof(buffer) - 1);
    if (bytesRead == -1) {
        perror("read");
        close(fd);
        return 1;
    }
    buffer[bytesRead] = '\0'; // Null-terminate the buffer
    printf("read: %s, len: %ld\n", buffer, bytesRead);
    if (close(fd) == -1) {
        perror("close");
        return 1;
    }
    return 0;
}

// 文件lseek
int api_seek(const char* path) {
    int fd = open(path, O_RDONLY);
    if (fd == -1) {
        perror("open");
        return 1;
    }
    off_t newOffset = lseek(fd, 0, SEEK_SET);
    if (newOffset == -1) {
        perror("lseek");
        close(fd);
        return 1;
    }
    printf("lseek: %lld\n", (long long)newOffset);
    if (close(fd) == -1) {
        perror("close");
        return 1;
    }
    return 0;
}

// 文件stat/fstat/lstat
int api_stat(const char* path) {
    struct stat fileStat;
    if (stat(path, &fileStat) == -1) {
        perror("stat");
        return 1;
    }
    printf("File Size:  %lld bytes\n", (long long)fileStat.st_size);
    printf("File Mode:  %o\n", fileStat.st_mode & 0777);
    printf("File Owner: %d\n", fileStat.st_uid);
    printf("File Group: %d\n", fileStat.st_gid);
    return 0;
}

// 文件fstat
int api_fstat(const char* path) {
    int fd = open(path, O_RDONLY);
    if (fd == -1) {
        perror("open");
        return 1;
    }
    struct stat fileStat;
    if (fstat(fd, &fileStat) == -1) {
        perror("fstat");
        close(fd);
        return 1;
    }
    printf("File Size:  %lld bytes\n", (long long)fileStat.st_size);
    printf("File Mode:  %o\n", fileStat.st_mode & 0777);
    printf("File Owner: %d\n", fileStat.st_uid);
    printf("File Group: %d\n", fileStat.st_gid);
    if (close(fd) == -1) {
        perror("close");
        return 1;
    }
    return 0;
}

// 文件lstat
int api_lstat(const char* path) {
    struct stat fileStat;
    if (lstat(path, &fileStat) == -1) {
        perror("lstat");
        return 1;
    }
    printf("File Size:  %lld bytes\n", (long long)fileStat.st_size);
    printf("File Mode:  %o\n", fileStat.st_mode & 0777);
    printf("File Owner: %d\n", fileStat.st_uid);
    printf("File Group: %d\n", fileStat.st_gid);
    return 0;
}

// 文件dup
int api_dup2(const char* path) {
    int fd1 = open(path, O_RDONLY);
    if (fd1 == -1) {
        perror("open");
        return 1;
    }
    int fd2 = dup(fd1);
    if (fd2 == -1) {
        perror("dup");
        close(fd1);
        return 1;
    }
    int fd3 = dup2(fd1, 5);
    if (fd3 == -1) {
        perror("dup2");
        close(fd1);
        close(fd2);
        return 1;
    }
    close(fd1);
    close(fd2);
    close(fd3);
    printf("fd: %d\n", fd1);
    printf("dup__fd: %d\n", fd2);
    printf("dup2_fd: %d\n", fd3);
    return 0;
}

int fun_access() {
    return api_access(API_FILE_NAME);
}

int fun_write() {
    return api_write(API_FILE_NAME, "Hello, File!");
}

int fun_read() {
    return api_read(API_FILE_NAME);
}

int fun_seek() {
    return api_seek(API_FILE_NAME);
}

int fun_stat() {
    return api_stat(API_FILE_NAME);
}

int fun_dup2() {
    return api_dup2(API_FILE_NAME);
}

// 测试例程
int main_test(int argc, char* argv[]) {
    PRINT_CMD_ARGS(argc, argv);
    FUNCTION_PRINT_API(fun_write);
    FUNCTION_PRINT_API(fun_read);
    FUNCTION_PRINT_API(fun_seek);
    FUNCTION_PRINT_API(fun_stat);
    FUNCTION_PRINT_API(fun_dup2);
    FUNCTION_PRINT_API(fun_access);
    return 0;
}

// main 函数
int main(int argc, char* argv[])
{
    main_test(argc, argv);
    return 0;
}