/*
 * 文件名: LinuxIPC.c
 * 作者: ljk
 * 创建时间: 2023-07-23
 * 文件描述: 进程间通信例程
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>




// 进程间管道通信
int api_pipe() {
    int pipefd[2];
    pid_t pid;
    char  buffer[255];
    char* message = "Hello, Process A!\n";

    // 创建管道
    if(pipe(pipefd) == -1){
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // 创建子进程
    pid = fork();
    if (pid == -1){
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if(pid == 0) {
        // 子进程关闭读端
        close(pipefd[0]);

        // 向管道写入消息
        write(pipefd[1], message, strlen(message) + 1);
        printf("%d Send:%s", getpid(), message);

        // 关闭写端
        close(pipefd[1]);
        exit(EXIT_SUCCESS);
    } 
    else {
        // 父进程关闭写端
        close(pipefd[1]);

        // 读取管道中的消息
        read(pipefd[0], buffer, sizeof(buffer));
        printf("%d Rece: %s\n",pid,  buffer);

        // 关闭读端
        close(pipefd[0]);
        // 等待子进程结束
        // wait(NULL);  
    }
    return 0;
}

// 测试例程
int main(int argc, char* argv[])
{
    api_pipe();
    return 0;
}