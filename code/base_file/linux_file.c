/*
 * 文件名: LinuxFile.c
 * 作者: ljk
 * 创建时间: 2023-07-15
 * 文件描述: 文件读写操操作例程, C 标准库中
 */
// strlen包括结束符，sizeof类型的长度，初学者尤其注意字符串指针计算的是指针长度
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include "../base_common.h"
#include "linux_file.h"




// 文件打开和关闭函数
int api_fopen(int action, char* path, char* mode, FILE** _fp) {
    if (action) {
        printf("打开文件: %s, mode: %s\n", path, mode);
        FILE *fp = fopen(path, mode);
        *_fp = fp;
        if (fp == NULL) {
            perror("fopen");
            return 1;
        }
    } else {
        if (*_fp != NULL) {
            fclose(*_fp);
            *_fp = NULL;
        }
    }
    return 0;
}

// 文件写函数
int api_fwrite(const char* path, const char* data) {
    FILE *fp = NULL;
    if (api_fopen(1, (char*)path, "w+", &fp) != 0) {
        return 1;
    }

    size_t file_len = fwrite(data, sizeof(char), strlen(data), fp);
    if (file_len <= 0) {
        perror("fwrite");
        api_fopen(0, NULL, NULL, &fp);
        return 1;
    }
    printf("写入文件: %s, ret len: %ld, write len: %lu\n", data, file_len, strlen(data));

    if (fflush(fp) != 0) {
        perror("fflush");
        api_fopen(0, NULL, NULL, &fp);
        return 1;
    }

    // 重置文件指针到文件开头
    if (fseek(fp, 0, SEEK_SET) != 0) {
        api_fopen(0, NULL, NULL, &fp);
        return 1;
    }

    api_fopen(0, NULL, NULL, &fp);
    return 0;
}

// 文件读函数
int api_fread(const char* path) {
    FILE *fp = NULL;
    char buffer[256]; // 缓冲区用于读取文件

    if (api_fopen(1, (char*)path, "r", &fp) != 0) {
        return 1;
    }

    // 重置文件指针到文件开头
    if (fseek(fp, 0, SEEK_SET) != 0) {
        api_fopen(0, NULL, NULL, &fp);
        return 1;
    }

    size_t read_len = fread(buffer, sizeof(char), sizeof(buffer), fp);
    if (read_len <= 0) {
        perror("fread");
        api_fopen(0, NULL, NULL, &fp);
        return 1;
    }

    long offset = ftell(fp);
    if (offset == -1L) {
        perror("ftell");
        api_fopen(0, NULL, NULL, &fp);
        return 1;
    }

    api_fopen(0, NULL, NULL, &fp);
    printf("读出文件: %s, ret len: %lu, ftell len: %ld\n", buffer, read_len, offset);

    return 0;
}

// 文件指针重置函数
int api_fseek(FILE* fp, long offset, int whence) {
    long file_size;

    // 获取文件大小
    fseek(fp, 0, SEEK_END);
    file_size = ftell(fp);
    rewind(fp);

    // 检查偏移量是否超出范围
    if ((whence == SEEK_SET && (offset < 0 || offset > file_size)) ||
        (whence == SEEK_CUR && (ftell(fp) + offset < 0 || ftell(fp) + offset > file_size)) ||
        (whence == SEEK_END && (offset > 0 || file_size + offset < 0))) {
        fprintf(stderr, "Error: Offset out of range\n");
        return 1;
    }

    // 执行 fseek 操作
    if (fseek(fp, offset, whence) != 0) {
        perror("fseek");
        return 1;
    }
    
    return 0;
}

// getc/fgetc标准输入,也可以读取文件,getchar只能标准输入
int api_fgetc() {
    int ch;

    // 使用fgetc() 逐字符读取文件内容
    printf("fgetc请输入字符:");
    while ((ch = fgetc(stdin)) != EOF) {
        printf("%c", ch);
        if (ch == '\n')
            break;
    }

    // 使用getc() 逐字符读取标准输入
    printf("getc请输入字符:");
    while ((ch = getc(stdin)) != EOF) {
        printf("%c", ch);
        if (ch == '\n')
            break;
    }

    // 使用getchar() 逐字符读取标准输入
    printf("getchar请输入字符:");
    while ((ch = getchar()) != EOF) {
        printf("%c", ch);
        if (ch == '\n')
            break;
    }

    return 0;
}

// fputc/putc标准输出,也可以写入到文件,putchar只能标准输出
int api_fputc() {
    char ch = 'A';

    // 使用fputc() 逐字符写入文件内容
    printf("fputc 输出字符:");
    if (fputc(ch, stdout) == EOF) {
        perror("fputc");
    }
    fputc('\n', stdout);

    // 使用putc() 逐字符输出标准输出
    printf("putc 输出字符:");
    if (putc(ch, stdout) == EOF) {
        perror("putc");
    }
    putc('\n', stdout);

    // 使用putchar() 逐字符输出标准输出
    printf("putchar 输出字符:");
    if (putchar(ch) == EOF) {
        perror("putchar");
    }
    putchar('\n');

    return 0;
}

// fgets/puts字符串测试,也可以对文件进行操作
int api_fgets() {
    char input[100] = {0};

    printf("fgets请输入字符串:");
    fgets(input, sizeof(input), stdin);

    printf("fputs输入的字符串:");
    fputs(input, stdout);

    return 0;
}

// fscanf标准输入解析,也可以读取文件里字符进行解析
// %*[^0-9]跳过非数字
int api_fprint() {
    int number = 10;
    char buffer[100];

    // 使用 sprintf 格式化字符串
    sprintf(buffer, "sprintf: %d\n", number);
    fprintf(stdout, "fprintf解析: %s", buffer);

    // 使用 scanf 解析标准输入
    number = 0;
    printf("请输入一个数字\n");
    scanf("%d", &number);
    printf("scanf解析: %d\n", number);

    // 使用 fscanf 解析标准输入
    number = 0;
    printf("请输入一个数字\n");
    fscanf(stdin, "%d", &number);
    printf("fscanf解析: %d\n", number);

    // 使用 sscanf 解析字符串中的数据
    const char *instr = "John Doe 25";
    printf("sscanf解析:%s\n", instr);
    number = 0;
    memset(buffer, 0, 100);
    int num = sscanf(instr, "%s%*[^0-9]%d", buffer, &number);

    if (num == 2) {
        printf("Name:   %s\n", buffer);
        printf("Number: %d\n", number);
    } else {
        printf("Failed to parse data.\n");
    }

    return 0;
}

// open()、socket()、pipe()等描述符打开文件
int api_fdopen(const char* path) {
    printf("打开文件: %s\n", path);

    int fd = open(path, O_RDONLY);
    if (fd != -1) {
        FILE* fp = fdopen(fd, "r");
        if (fp != NULL) {
            char buffer[256];
            size_t read_len = fread(buffer, sizeof(char), sizeof(buffer), fp);
            if (read_len <= 0) {
                perror("fread");
                fclose(fp);
                return 1;
            }
            printf("读出文件: %s\n", buffer);
            fclose(fp);
        } else {
            perror("创建文件指针时出错");
        }
    } else {
        perror("打开文件时出错");
    }

    return 0;
}

// 附加示例函
// 展示 fgetpos, fsetpos, rewind, freopen, setvbuf, remove 的用法
int fun_fother() {
    FILE *fp;
    fpos_t pos;
    char buffer[100];

    // 打开文件进行读写
    fp = fopen("example.txt", "w+");
    if (fp == NULL) {
        perror("fopen");
        return 1;
    }

    // 写入一些数据到文件
    fputs("Hello, World!", fp);

    // 获取当前文件位置
    if (fgetpos(fp, &pos) != 0) {
        perror("fgetpos");
        fclose(fp);
        return 1;
    }

    // 重置文件指针到文件开头
    rewind(fp);

    // 读取文件内容
    if (fgets(buffer, sizeof(buffer), fp) == NULL) {
        perror("fgets");
        fclose(fp);
        return 1;
    }
    printf("读取内容: %s\n", buffer);

    // 恢复文件位置
    if (fsetpos(fp, &pos) != 0) {
        perror("fsetpos");
        fclose(fp);
        return 1;
    }

    // 追加一些数据到文件
    fputs(" Welcome to C programming.", fp);

    // 关闭并重新打开文件
    if (freopen("example.txt", "r", fp) == NULL) {
        perror("freopen");
        return 1;
    }

    // 设置文件缓冲区
    if (setvbuf(fp, NULL, _IOFBF, 1024) != 0) {
        perror("setvbuf");
        fclose(fp);
        return 1;
    }

    // 读取并打印文件内容
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        printf("%s", buffer);
    }

    // 关闭文件
    fclose(fp);

    // 删除文件
    if (remove("example.txt") != 0) {
        perror("remove");
        return 1;
    }

    printf("展示额外函数功能\n");
    return 0;
}

int fun_fwrite() {
    return api_fwrite(API_FILE_NAME, "Hello, World!");
}

int fun_fread() {
    return api_fread(API_FILE_NAME);
}

int fun_fdopen() {
    return api_fdopen(API_FILE_NAME);
}

// test函数
int main_test(int argc, char* argv[]) {
    PRINT_CMD_ARGS(argc, argv);
    FUNCTION_PRINT_API(fun_fwrite);
    FUNCTION_PRINT_API(fun_fread);
    FUNCTION_PRINT_API(api_fgetc);
    FUNCTION_PRINT_API(api_fputc);
    FUNCTION_PRINT_API(api_fgets);
    FUNCTION_PRINT_API(api_fprint);
    FUNCTION_PRINT_API(fun_fdopen);
    return 0;
}

// main函数
int main(int argc, char* argv[])
{
    main_test(argc, argv);
    return 0;
}