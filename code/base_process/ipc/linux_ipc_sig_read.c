/*
 * 文件名: LinuxIPC.c
 * 作者: ljk
 * 创建时间: 2023-07-23
 * 文件描述: 进程间通信信号量例程
 */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void signal_handler(int signum) 
{
    printf("Receiver: Signal %d received!\n", signum);
}

int main(int argc, char* argv[]) 
{
    struct sigaction sa;
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGUSR1, &sa, NULL) != 0) {
        perror("Receiver: Error setting signal handler");
        return 1;
    }

    while (1) {
        printf("Receiver: Waiting for a signal...\n");
        sleep(5); // 等待信号
    }

    return 0;
}

