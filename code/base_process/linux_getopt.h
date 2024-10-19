#ifndef  _LINUX_GETOPT_H__
#define  _LINUX_GETOPT_H__
#ifdef __cplusplus
extern "C" {
#endif

// getopt
int api_getopt(int argc, char* argv[]);
// getopt_long  
int api_getopt_long(int argc, char* argv[]);
// getopt_long_only  
int api_getopt_long_only(int argc, char* argv[]);

#ifdef __cplusplus
}
#endif
#endif