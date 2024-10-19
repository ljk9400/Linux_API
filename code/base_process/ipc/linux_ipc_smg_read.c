/**
 * 文件名: LinuxIPC.c
 * 作者: ljk
 * 创建时间: 2023-08-23
 * 文件描述: 进程间通信消息队列例程
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct message {
    long mtype;
    char mtext[100];
};

int main(int argc, char* argv[]) 
{
    //key_t key = ftok("message_queue_key", 'A'); // 使用相同的键
    key_t key = 36001; 

    int msgid = msgget(key, 0666);  // 打开消息队列
    printf("msgid:%d\n", msgid);

    struct message msg;
    msgrcv(msgid, &msg, sizeof(struct message) - sizeof(long), 1, 0); // 接收消息

    printf("Message received: %s\n", msg.mtext);

    // 删除消息队列，
    msgctl(msgid, IPC_RMID, NULL);

    return 0;
}
