#ifndef  _LINUX_FILE66536_H__
#define  _LINUX_FILE66536_H__
#ifdef __cplusplus
extern "C" {
#endif

// 文件打开和关闭函数
int api_fopen(int action, char* path, char* mode, FILE** _fp);
// 文件写函数
int api_fwrite(const char* path, const char* data);
// 文件读函数
int api_fread(const char* path);
// 文件指针重置函数
int api_fseek(FILE* fp, long offset, int whence);
// getc/fgetc标准输入,也可以读取文件,getchar只能标准输入
int api_fgetc();
// fputc/putc标准输出,也可以写入到文件,putchar只能标准输出
int api_fputc();
// fgets/puts字符串测试,也可以对文件进行操作
int api_fgets();
// print/scanf相关函数
int api_fprint();
// open()、socket()、pipe()等描述符打开文件
int api_fdopen();
// 附加示例函数
int fun_fother();

#ifdef __cplusplus
}
#endif
#endif