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
#include <stdio.h>
#include <string.h>

#define RET_OK   0
#define RET_ERR  -1
#define API_DIR_PATH     "../build/"  // 文件目录测试  
#define API_FILE_NAME    API_DIR_PATH  "file.txt"   // 读写文件测试
#define MKD_DIR_NAME     API_DIR_PATH  "build_dir"  // 创建目录测试 
#define FILE_NAME_LINK   API_DIR_PATH  "link"       // 软连接测试
#define FILE_NAME_DEV    API_DIR_PATH  "file.txt"   
#define FILE_NAME_MEM    API_DIR_PATH  "file.txt" 

// 定义宏来打印命令行参数
#define PRINT_CMD_ARGS(argc, argv) \
    do { \
        printf("输入的命令行参数个数为: %d\n", argc); \
        for (int i = 0; i < argc; ++i) { \
            printf("参数 %d: %s\n", i, argv[i]); \
        } \
    } while (0)
// 定义宏来打印函数名
#define FUNCTION_PRINT_API(name)  function_print(#name, name)
// main函数定义宏
#define DEFINE_MAIN() \
int main(int argc, char* argv[]) { \
    main_test(argc, argv); \
    return 0; \
}

// 打印标题
void print_separator(const char* name, const char* status, const char* symbol) {
    int name_len = strlen(name);
    int prefix_len = strlen("[ test] ");
    int status_len = strlen(status);
    int total_len = name_len + prefix_len + status_len;
    int separator_len = 50; // 可以调整宽度
    int padding_len = (separator_len - total_len) / 2;
    int extra_padding = (separator_len - total_len) % 2; // 处理奇数情况

    printf("[");
    printf("%s test] ", name);
    for (int i = 0; i < padding_len; i++) {
        printf("%s", symbol);
    }
    printf(" %s ", status);
    for (int i = 0; i < padding_len + extra_padding; i++) {
        printf("%s", symbol);
    }
    printf("\n");
    if (strcmp(status, "end") == 0) {
        printf("\n\n");
    }
}

// 打印函数
int function_print(char* name, int (*callback)()) {
    int ret = 0;
    if (name == NULL) {
        name = "default";
    }

    if (callback == NULL) {
        printf("[%s test] Error: callback is NULL\n", name);
        return -1;
    }

    print_separator(name, "start", "=");
    ret = callback();
    printf("[%s test] ret: %d\n", name, ret);
    print_separator(name, "end", "-");
    return ret;
}

