#ifndef __LOGPRINT_H
#define __LOGPRINT_H
#define LOG_PATH_SIZE    (255)
#ifdef __cplusplus
extern "C" {
#endif
typedef struct _log_st  
{  
    char path[LOG_PATH_SIZE-10];  
    int fd;  
    int size;  
    int level;  
    int num;         
}log_st;
 
log_st *log_init(); 
void log_debug( const char *msg, ...);
#ifdef __cplusplus
}
#endif
#endif // __LOG_H__
