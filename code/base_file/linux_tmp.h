#ifndef  _LINUX_TEMP_H__
#define  _LINUX_TEMP_H__
#ifdef __cplusplus
extern "C" {
#endif

// 返回一个唯一的临时文件名, 并不会创建实际的文件。
int api_tmpnam();
// 创建一个唯一的临时文件，并返回一个指向该文件的文件指针。创建的临时文件在程序结束后会自动删除。
int api_tmpfile();
// 返回一个唯一的临时文件名, 并不会创建实际的文件。
int api_mktemp();
// 返回一个唯一的临时文件名, 创建实际的文件目录。
int api_mkdtemp();
// 返回一个描述符和文件名字, 创建实际的文件。
int api_mkstemp();

#ifdef __cplusplus
}
#endif
#endif