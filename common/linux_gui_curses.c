/*
 * 文件名: LinuxGui.c
 * 作者: ljk
 * 创建时间: 2023-08-01
 * 文件描述: 界面显示操作例程
 */
// 需要安装工具libncurses-dev
// gcc LinuxGui.c -lcurses
#include <curses.h>

// 界面显示测试
int fun_curses() {
    // 初始化Curses
    initscr();
    // 输出文本
    printw("Hello, Curses!");
    // 获取用户输入
    getch();
    // 结束Curses模式
    endwin();
    return 0;
}

// 测试例程
int main(int argc, char* argv[])
{
    fun_curses();
    return 0;
}
