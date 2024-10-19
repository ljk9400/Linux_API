/*
 * 文件名: LinuxIPC.c
 * 作者: ljk
 * 创建时间: 2023-07-23
 * 文件描述: 进程间通信FIFO例程
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>


int main(int argc, char* argv[]) 
{
    const char *fifo_path = "myfifo"; // 命名管道路径

    int fd = open(fifo_path, O_RDONLY); // 打开命名管道以读取数据
    if (fd == -1) {
        perror("open");
        return 1;
    }

    char buffer[256];
    ssize_t bytes_read = read(fd, buffer, sizeof(buffer)); // 从命名管道读取数据
    if (bytes_read > 0) {
        printf("Received: %s\n", buffer);
    }

    close(fd);
    return 0;
}