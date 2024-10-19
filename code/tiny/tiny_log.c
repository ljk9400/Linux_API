
/*log systems*/
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include "tiny_log.h"

#define LOG_PRINT_MODE      1
#define LOG_WRITE_MODE      1
#define LOG_SAVEALLFILE     4
#define LOG_BUF_SIZE     (1024*10)  // 每次写入最大数据量 byte
#define LOG_FILE_SIZE    (1)     // 每个日志文件最大大小 KB
#define LOG_FILE_PATH    "../build/"
#define LOG_FILE_NAME    LOG_FILE_PATH "log"

pthread_mutex_t mutexlog = PTHREAD_MUTEX_INITIALIZER;
log_st *logs;

static void log_checksize()  
{  
    struct stat stat_buf; 
    char new_path[LOG_PATH_SIZE] = {0};  
    char bak_path[LOG_PATH_SIZE] = {0};  

    if(NULL == logs || 1 == logs->level || '\0' == logs->path[0]) 
        return;  
        
    memset(&stat_buf, 0, sizeof(struct stat));  
    fstat(logs->fd, &stat_buf);  
    if(stat_buf.st_size > logs->size) {  
        close(logs->fd);  
        if(logs->num) { 
            //snprintf(new_path, LOG_PATH_SIZE, "%s%d", logs->path, (int)time(NULL)); 
            snprintf(bak_path, LOG_PATH_SIZE, "%s.%c", logs->path, logs->num+'A');
            remove(bak_path);
            for(int i=0; i<logs->num; i++)
            {
                snprintf(bak_path, LOG_PATH_SIZE, "%s.%c", logs->path, 'A'+logs->num-i);  
                snprintf(new_path, LOG_PATH_SIZE, "%s.%c", logs->path, 'A'+logs->num-i-1);   
                rename(new_path, bak_path);
            } 
        }
        else {  
            snprintf(bak_path, LOG_PATH_SIZE, "%s.log.old", logs->path);  
            snprintf(new_path, LOG_PATH_SIZE, "%s.log", logs->path);  
            remove(bak_path);  
            rename(new_path, bak_path);
        }  
        //create a new file  
        logs->fd = open(new_path, O_RDWR|O_APPEND|O_CREAT|O_SYNC, S_IRUSR|S_IWUSR|S_IROTH);  
    }  
}

/*print msg*/
void log_debug(const char *msg, ...)  
{  
    va_list ap;  
    time_t now;  
    char *pos;  
    char _n = '\n';  
    char message[LOG_BUF_SIZE] = {0};  
    int nMessageLen = 0;  
    int sz; 

    if(NULL == logs || 0 == logs->level) 
        return; 

    now = time(NULL); 
   	pos = ctime(&now);
    sz = strlen(pos); 
    pos[sz-1] =']'; 
    snprintf(message, LOG_BUF_SIZE, "[%s\n", pos);  
    for (pos = message; *pos; pos++);  
	sz = pos - message;  

    va_start(ap, msg);  
    nMessageLen = vsnprintf(pos, LOG_BUF_SIZE - sz, msg, ap);  
    va_end(ap);  
    if (nMessageLen <= 0) 
        return;  
    if (2 != logs->level)
        printf("%s\n", message);   
    if (1 < logs->level) {   
        pthread_mutex_lock(&mutexlog);
        log_checksize();
        int ret = write(logs->fd, message, strlen(message));
        if (ret != strlen(message)) {
            fprintf(stderr, "Log write error");
        }
        ret = write(logs->fd, &_n, 1); 
        if (ret != 1) {
            fprintf(stderr, "Log write error");
        }
        fsync(logs->fd); 
        pthread_mutex_unlock(&mutexlog);
    }
}  

/*creat and init log file*/
static log_st *_log_init(char *path, int size, int level, int num)  
{  
	char new_path[LOG_PATH_SIZE] = {0};  

	if (NULL == path || 0 == level) 
        return NULL; 

    log_st *logs = (log_st *)malloc(sizeof(log_st));  
    memset(logs, 0, sizeof(log_st));  
    if (level > 1) {  
        //the num use to control file naming  
        logs->num = num;  
        if(num)  
            //snprintf(new_path, LOG_PATH_SIZE, "%s%d", path, (int)time(NULL));
            snprintf(new_path, LOG_PATH_SIZE, "%s.A", path);  
        else  
            snprintf(new_path, LOG_PATH_SIZE, "%s.log", path);  
        if(-1 == (logs->fd = open(new_path, O_RDWR|O_APPEND|O_CREAT|O_SYNC, S_IRUSR|S_IWUSR|S_IROTH))) {  
            free(logs);  
            logs = NULL;  
            return NULL;  
        }  
    }  
    strncpy(logs->path, path, LOG_PATH_SIZE);  
	logs->size = (size > 0 ? size:0);  
	logs->level = (level > 0 ? level:0);  

	return logs;  
}

log_st *log_init() 
{
    int flieNum = 0, printWrite = 0;
    struct stat stat_buf; //useless, but ...
	if(stat(LOG_FILE_PATH, &stat_buf) == -1) {
		/*create irtouch log dir*/
		mkdir(LOG_FILE_PATH, 0750);
	}
    if( LOG_PRINT_MODE )
        printWrite += 1;
    if( LOG_WRITE_MODE )
        printWrite += 2;
    if(LOG_SAVEALLFILE)
        flieNum = LOG_SAVEALLFILE;
    logs = _log_init(LOG_FILE_NAME, LOG_FILE_SIZE*500, printWrite, flieNum); // 1K * 100 & print file 2     
	return logs;
}


// 测试程序
int main(int argc, char* argv[])
{
    log_st *fd = log_init();
    for(int i=0; i<100; i++){
        log_debug("log init");
    }
}
 