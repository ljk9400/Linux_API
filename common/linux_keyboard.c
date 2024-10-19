/*
 * 文件名: LinuxKey.c
 * 作者: ljk
 * 创建时间: 2023-07-25
 * 文件描述: 键盘输入操作例程
 */
#include <termios.h>
#include <stdio.h>
#include <unistd.h>



static struct termios initial_settings, new_settings;
static int peek_character = -1;

// 键盘初始化
void init_keyboard() {
    tcgetattr(0, &initial_settings);
    new_settings = initial_settings;
    new_settings.c_lflag &= ~ICANON;
    new_settings.c_lflag &= ~ECHO;
    new_settings.c_lflag &= ~ISIG;
    new_settings.c_cc[VMIN] = 1;
    new_settings.c_cc[VTIME] = 0;
    tcsetattr(0, TCSANOW, &new_settings);
}

// 关闭设置
void close_keyboard() {
    tcsetattr(0, TCSANOW, &initial_settings);
}

// 是否敲击键盘
int kbhit() {
    char ch;
    int nread;

    if(peek_character != -1)
        return 1;
    new_settings.c_cc[VMIN] = 0;
    tcsetattr(0, TCSANOW, &new_settings);
    nread = read(0, &ch, 1);
    new_settings.c_cc[VMIN] = 1;
    tcsetattr(0, TCSANOW, &new_settings);

    if(nread == 1)
    {
        peek_character = ch;
        return 1;
    }
    return 0;
}

// 读取字符
int readch() {
    char ch;

    if(peek_character != -1)
    {
        ch = peek_character;
        peek_character = -1;
        return ch;
    }
    read(0, &ch, 1);
    return ch;
}

// 键盘功能
int fun_keyboard() {
    int ch = 0;

    init_keyboard();
    while(ch != 'q')
    {
        printf("looping\n");
        sleep(1);
        if(kbhit())
        {
            ch = readch();
            printf("you hit %c\n", ch);
        }
    }
    return 0;
}

// 测试例程
int main(int argc, char* argv[])
{
    fun_keyboard();
    return 0;
}