/**
 * 文件名: LinuxIPC.c
 * 作者: ljk
 * 创建时间: 2023-07-23
 * 文件描述: 进程间通信FIFO例程
 * 查看FIFO文件 ls -l
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

    // 创建命名管道
    mkfifo(fifo_path, 0666);

    int fd = open(fifo_path, O_WRONLY); // 打开命名管道以写入数据
    if (fd == -1) {
        perror("open");
        return 1;
    }

    const char *message = "Hello, FIFO!";
    write(fd, message, strlen(message) + 1); // 写入数据到命名管道

    close(fd);
    unlink(fifo_path); //删除 FIFO
    return 0;
}