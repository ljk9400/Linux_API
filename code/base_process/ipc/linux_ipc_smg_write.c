/**
 * 文件名: LinuxIPC.c
 * 作者: ljk
 * 创建时间: 2023-08-23
 * 文件描述: 进程间通信消息队列例程
 * 查看消息队列: ipcs -q
 * 删除消息队列：ipcrm -q <msgid>
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
    //key_t key = ftok("message_queue_key", 'A'); // 创建一个唯一的键
    key_t key = 36001; 

    int msgid = msgget(key, 0666 | IPC_CREAT);  // 创建消息队列
    printf("msgid:%d\n", msgid);

    struct message msg;
    msg.mtype = 1; // 消息类型
    strcpy(msg.mtext, "Hello, receiver!");

    msgsnd(msgid, &msg, sizeof(struct message) - sizeof(long), 0); // 发送消息

    printf("Message sent: %s\n", msg.mtext);

    return 0;
}
