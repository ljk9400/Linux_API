/*
 * 文件名: LinuxMmap.c
 * 作者: ljk
 * 创建时间: 2023-07-15
 * 文件描述: 内存RAM测试
 */
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "../base_common.h"



// 将文件映射到内存ARM
int api_mmap(int fd, long size, const char* data) {
    // 将文件扩展到指定大小
    off_t file_size = size;
    if (lseek(fd, file_size - 1, SEEK_SET) == -1) {
        perror("Error calling lseek");
        close(fd);
        exit(EXIT_FAILURE);
    }

    if (write(fd, "", 1) == -1) {
        perror("Error writing to file");
        close(fd);
        exit(EXIT_FAILURE);
    }

    // 映射文件到内存
    char *mapped_addr = (char *)mmap(NULL, file_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (mapped_addr == MAP_FAILED) {
        perror("Error mapping file to memory");
        close(fd);
        exit(EXIT_FAILURE);
    }

    // 写入数据到映射区域
    const char *message = data;
    for (int i = 0; message[i] != '\0'; ++i) {
        mapped_addr[i] = message[i];
    }

    // 刷新映射区域到文件
    if (msync(mapped_addr, file_size, MS_SYNC) == -1) {
        perror("Error syncing memory to file");
        close(fd);
        exit(EXIT_FAILURE);
    }

    // 从映射区域读取数据
    printf("Read from memory-mapped file: %s\n", mapped_addr);

    // 解除映射并关闭文件
    if (munmap(mapped_addr, file_size) == -1) {
        perror("Error unmapping memory");
        close(fd);
        exit(EXIT_FAILURE);
    }
    return 0;
}

// mmap测试
int fun_mmap() {
    const char *message = "Hello, mmap!";
    const char *file_path = FILE_NAME_MEM;

    int fd = open(file_path, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1) 
    {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    api_mmap(fd, 100, message);
    close(fd);
}

// 打印数组内容的函数
void print_array(const int* array, int count, const char* title) {
    if (array != NULL) {
        const char* print_title = title ? title : "default";
        printf("%s: ", print_title);
        for (int i = 0; i < count; ++i) {
            printf("%d ", array[i]);
        }
        printf("\n");
    }
}

// 内存分配测试函数
int fun_memory_allocation() {
    // 使用malloc动态分配内存并检查是否分配成功
    int* array1 = (int*)malloc(5 * sizeof(int));
    if (array1 == NULL) {
        perror("malloc 内存分配失败");
        return 1;
    }
    // 使用calloc动态分配内存，并初始化为0
    int* array2 = (int*)calloc(5, sizeof(int));
    if (array2 == NULL) {
        free(array1); // 在退出前释放已分配的内存
        perror("calloc 内存分配失败");
        return 1;
    }
    // 初始化动态分配的内存
    for (int i = 0; i < 5; ++i) {
        array1[i] = i + 1;
        array2[i] = i + 1;
    }
    // 打印数组内容
    print_array(array1, 5, "array1");
    print_array(array2, 5, "array2");

    // 使用realloc重新调整分配内存的大小
    int* array3 = (int*)realloc(array1, 10 * sizeof(int));
    if (array3 == NULL) {
        free(array2); // 在退出前释放已分配的内存
        perror("realloc 内存分配失败");
        return 1;
    }
    // 注意：realloc后，array1可能不再有效，应使用array3
    array1 = NULL;
    // 初始化realloc后的内存
    for (int i = 5; i < 10; ++i) {
        array3[i] = i + 1;
    }
    // 打印realloc后的数组内容
    print_array(array3, 10, "array3");

    // 释放动态分配的内存
    free(array2);
    free(array3);
    return 0;
}

int main_test(int argc, char* argv[]) {
    PRINT_CMD_ARGS(argc, argv);
    FUNCTION_PRINT_API(fun_mmap);
    return 0;
}

// 环境测试例程
int main(int argc, char* argv[])
{
    main_test(argc, argv);
    return 0;
}
