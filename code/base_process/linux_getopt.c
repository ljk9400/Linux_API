/*
 * 文件名: LinuxGetopt.c
 * 作者: ljk
 * 创建时间: 2023-07-25
 * 文件描述: 程序输入参数处理例程
 */
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "linux_getopt.h"

/*
#include <unistd.h>
#include <getopt.h>
extern char *optarg;  
extern int optind, opterr, optopt;  
int getopt(int argc, char * const argv[],const char *optstring);  
int getopt_long(int argc, char * const argv[], const char *optstring, const struct option *longopts, int *longindex);  
int getopt_long_only(int argc, char * const argv[], const char *optstring, const struct option *longopts, int *longindex);
*/

/*
1、参数形式
形式如“a:b::cd:“，分别表示程序支持的命令行短选项有-a、-b、-c、-d，冒号含义如下：
(1)只有一个字符，不带冒号——只表示选项， 如-c 
(2)一个字符，后接一个冒号——表示选项后面带一个参数，如-a 100
(3)一个字符，后接两个冒号——表示选项后面带一个可选参数，即参数可有可无， 如果带参数，则选项与参数直接不能有空格，形式应该如-b200

2、选项
长选选  --字符串  ./a.out --opt_a=value  只有设置了required_argument选项参考才可以可以使用字符串
短选项  -字符     ./a.out -a value

3、区别
getopt  短选项
getopt_long 长选项、短选项，但是不能混合。
getopt_long_only 可以混合
*/


// getopt 
// 测试用例 短选项./a.out -a -b option_value
int api_getopt(int argc, char* argv[]) {
    int opt;
    int option_a = 0;
    char *option_b = NULL;

    while ((opt = getopt(argc, argv, "ab:")) != -1) {
        switch (opt) {
            case 'a':
                option_a = 1;
                break;
            case 'b':
                option_b = optarg;
                break;
            case '?':
                fprintf(stderr, "Unknown option: -%c\n", optopt);
                return 1;
            case ':':
                fprintf(stderr, "Option -%c requires an argument.\n", optopt);
                return 1;
            default:
                break;
        }
    }

    printf("Option a: %d\n", option_a);
    printf("Option b: %s\n", option_b);

    if (optind < argc) {
        printf("Non-option arguments:\n");
        for (int i = optind; i < argc; i++) {
            printf("%s\n", argv[i]);
        }
    }

    return 0;
}

// getopt_long  
// 测试用例 ./a.out -a  --option_b=value
int api_getopt_long(int argc, char* argv[]) {
    int opt;
    int option_a = 0;
    char *option_b = NULL;

    struct option long_options[] = {
        {"option_a", no_argument, NULL, 'a'},
        {"option_b", required_argument, NULL, 'b'},
        {NULL, 0, NULL, 0} // The last element must be filled with zeros.
    };

    while ((opt = getopt_long(argc, argv, "ab:", long_options, NULL)) != -1) {
        switch (opt) {
            case 'a':
                option_a = 1;
                break;
            case 'b':
                option_b = optarg;
                break;
            case '?':
                // The getopt_long function already prints the error message for unknown options.
                return 1;
            default:
                break;
        }
    }

    printf("Option a: %d\n", option_a);
    printf("Option b: %s\n", option_b);

    // Remaining non-option arguments (if any)
    if (optind < argc) {
        printf("Non-option arguments:\n");
        for (int i = optind; i < argc; i++) {
            printf("%s\n", argv[i]);
        }
    }

    return 0;
}

// getopt_long_only  
// 测试用例 ./a.out -b vlua 或 ./a.out --option_b=vlua
int api_getopt_long_only(int argc, char* argv[]) {
    int opt;
    int option_a = 0;
    char* option_b = NULL;

    struct option long_options[] = {
        {"option_a", no_argument, NULL, 'a'},
        {"option_b", required_argument, NULL, 'b'},
        {NULL, 0, NULL, 0} // The last element must be filled with zeros.
    };

    while ((opt = getopt_long_only(argc, argv, "ab:", long_options, NULL)) != -1) {
        switch (opt) {
            case 'a':
                option_a = 1;
                break;
            case 'b':
                option_b = optarg;
                break;
            case '?':
                // The getopt_long function already prints the error message for unknown options.
                return 1;
            default:
                break;
        }
    }

    printf("Option a: %d\n", option_a);
    printf("Option b: %s\n", option_b);

    // Remaining non-option arguments (if any)
    if (optind < argc) {
        printf("Non-option arguments:\n");
        for (int i = optind; i < argc; i++) {
            printf("%s\n", argv[i]);
        }
    }

    return 0;
}

// 使用打印
int function_usage(int argc, char* argv[]) {
    printf(" usage function \n \
    ./LinuxGetopt 短选项:-a -b option_value\n \
    ./LinuxGetopt 长选项:-a --option_b=value\n \
    ./LinuxGetopt 短选项:-b value 或 长选项:./LinuxGetopt --option_b=value\n");
    printf("输入的命令行参数个数为: %d\n", argc); 
    for (int i = 0; i < argc; ++i) { 
        printf("参数 %d: %s\n", i, argv[i]); 
    }
    return 0;
}


// 测试文件目录
int main_test(int argc, char* argv[])
{
    function_usage(argc, argv);
#if 0
    api_getopt(argc, argv);
#elif 1
    api_getopt_long(argc, argv);
#else
    api_getopt_long_only(argc, argv);
#endif
    return 0;
}

// 测试程序
int main(int argc, char *argv[]) 
{
    main_test(argc, argv);
    return 0;
}
