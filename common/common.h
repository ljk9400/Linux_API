////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//    Copyright© 2023 JK Li. All Rights Reserved.                             //
//                                                                            //
//  All content within this software (including but not limited to text,      //
//  images, audio, video, designs, color schemes, graphic styles, logos, etc.)//
//  is protected under International Copyright Laws. Unauthorized copying,    //
//  publishing, distributing, or any form of usage of the content from this   //
//  software, whether in digital or physical form, is strictly prohibited     //
//  without the prior written consent of JK Li.                               //
//                                                                            //
//  Any unauthorized reproduction or use of the materials contained within    //
//  this software constitutes an infringement of the copyright owner's rights //
//  and may be subject to prosecution under copyright law. For such           //
//  unauthorized activities, you may be held liable to JK Li for any and      //
//  all damages (including the recovery of attorney's fees) that may arise    //
//  as a result of your infringement.                                         //
//                                                                            //
//  Please respect the copyright and contribute to a healthy digital          //
//  environment.                                                              //
////////////////////////////////////////////////////////////////////////////////
#ifndef  _COMMON_5767_H__
#define  _COMMON_5767_H__
// 版本号
#define LINUX_VERSION_MAJOR   "1"
#define LINUX_VERSION_MINOR   "0"
#define LINUX_VERSION_PATCH   "8"
#define LINUX_API_VERSION   "API V"  LINUX_VERSION_MAJOR  "."  LINUX_VERSION_MINOR  "."  LINUX_VERSION_PATCH

#ifdef __cplusplus
extern "C" {
#endif

//文件操作
#include <stdio.h>   //: 标准输入输出
#include <stdlib.h>  //: 标准库功能
#include <unistd.h>  //: POSIX操作系统API
#include <fcntl.h>   //: 文件控制选项
#include <sys/file.h>//: 文件访问权限
#include <sys/stat.h>//: 文件状态
#include <dirent.h>  //: 目录项
#include <ftw.h>     //: 文件树遍历
#include <libgen.h>  //: 路径处理函数

// 字符串和内存操作
#include <string.h>  //: 字符串操作
#include <strings.h> //: 字符串操作（与string.h类似，但遵循BSD风格）
#include <memory.h>  //: 内存操作
#include <wchar.h>   //: 宽字符处理
#include <wctype.h>  //: 宽字符分类

//时间和日期
#include <time.h>     //: 时间和日期函数
#include <sys/time.h> //: 时间值访问
#include <sys/times.h>//: 进程时间

//进程、线程和信号
#include <sys/wait.h> //: 进程等待
#include <sys/ipc.h>  //: 进程间通信
#include <sys/msg.h>  //: 消息队列
#include <sys/sem.h>  //: 信号量
#include <sys/shm.h>  //: 共享内存
#include <pthread.h>  //: POSIX线程
#include <signal.h>   //: 信号处理
#include <ucontext.h> //: 用户上下文

//网络编程
#include <sys/socket.h>//: 套接字接口
#include <netinet/in.h>//: Internet地址族
#include <arpa/inet.h> //: 网络地址转换
#include <netdb.h>     //: 网络数据库操作
#include <sys/un.h>    //: UNIX域套接字

//错误处理
#include <errno.h>      //: 错误码定义
#include <error.h>      //: 错误处理函数

//数学计算
#include <math.h>       //: 数学函数
#include <complex.h>    //: 复数运算
#include <fenv.h>       //: 浮点环境
#include <float.h>      //: 浮点数特性
#include <iso646.h>     //: 逻辑运算符宏

//杂项
#include <assert.h>     //: 断言宏
#include <stdarg.h>     //: 可变参数列表
#include <stdbool.h>    //: 布尔类型
#include <stddef.h>     //: 标准定义
#include <stdint.h>     //: 整型固定宽度
#include <inttypes.h>   //: 格式化输出
#include <limits.h>     //: 实现定义的限制
#include <setjmp.h>     //: 非局部跳转
#include <stdio_ext.h>  //: 扩展输入/输出
#include <syslog.h>     //: 系统日志

#ifdef __cplusplus
}
#endif
#endif