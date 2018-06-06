#ifndef _OS_INI_H
#define _OS_INI_H
#include <sys/types.h>
#include <assert.h>
#include <stdint.h>

#ifndef IN
#define IN
#endif
#ifndef OUT
#define OUT
#endif
#ifndef INOUT
#define INOUT
#endif

#ifndef WIN32
typedef int             HANDLE;
#endif

typedef int             RESULT_t;

#define INVAILD_HANDLE_VALUE    (-1)
#define INVALID_PARAM_VALUE   -2

#ifndef BOOL_D
#define BOOL_D 1
typedef int BOOL;
#endif
#ifndef NULL
#define NULL  0
#endif

#ifndef true
#define true  1
#endif

#ifndef TRUE
#define TRUE  1
#endif

#ifndef false
#define false 0
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef SUCCESS
#define SAME_PARAM_VALUE   1
#define SUCCESS 0  //返回结果成功
#define FAIL -1 //返回错误
#endif

#ifndef NEED_WRITE_MORE
#define NEED_WRITE_MORE  1
#define NEED_READ_MORE   2
#define READ_NONE_DATA   3
#define WRITE_NONE_DATA   4
#endif

#ifdef _HY_DEBUG
#define HY_ASSERT(arg)  assert(arg)
#else
#define HY_ASSERT(arg)
#endif

#define INVAILD_CHANNEL_NUM     0xFF



#ifdef WIN32

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN		// 从 Windows 头中排除极少使用的资料
#endif //WIN32_LEAN_AND_MEAN

#define FD_SETSIZE 1024


#include <windows.h>
#include <windef.h>	
#include <winbase.h>	
#include <winsock2.h>	
#include <iphlpapi.h>	
#include <ws2tcpip.h>

#ifndef UINT16_t
typedef UINT16 UINT16_t;
typedef UINT32 UINT32_t;
typedef UINT64 UINT64_t;
#endif //UINT16_t

#define INLINE_t

#ifndef uint64_t
typedef UINT64	uint64_t;
typedef UINT32 uint32_t;
typedef UINT16 uint16_t;
typedef UINT8  uint8_t;
#endif //uint64_t

#ifndef int32_t
 typedef INT32  int32_t;
 typedef INT64  int64_t;
 typedef INT16  int16_t;
 typedef INT8   int8_t;
#endif //int32_t


#else //WIN32
#include <pthread.h>
#include <unistd.h>
#include <stdint.h>
//#include <arpa/inet.h>  /* for inet_pton() */	
//#include <net/if.h>	
//#include <netinet/in.h>
//#include <sys/time.h>
#include <fcntl.h>
/*! This typedef makes the code slightly more WIN32 tolerant.	 
* On WIN32 systems, SOCKET is unsigned and is not a file	 
* descriptor. */	
typedef int SOCKET;	
/*! INVALID_SOCKET is unsigned on win32. */
#ifndef INVALID_SOCKET
#define INVALID_SOCKET (-1)
#endif
/*! select() returns SOCKET_ERROR on win32. */	
#ifndef SOCKET_ERROR
#define SOCKET_ERROR (-1)
#endif


#ifndef UINT16_t
    typedef uint16_t    UINT16_t;
    typedef uint32_t    UINT32_t;
    typedef uint64_t    UINT64_t;
#endif //UINT16_t
#define INLINE_t inline
#include <strings.h>
#endif //WIN32

typedef char          T_S8;
typedef int16_t         T_S16;
typedef int32_t         T_S32;
typedef int64_t         T_S64; 

typedef unsigned char        T_U8;
typedef uint16_t       T_U16;
typedef uint32_t       T_U32;
typedef uint64_t       T_U64;

typedef const char      T_CS8;
typedef const unsigned char T_CU8;

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdarg.h>

#ifdef WIN32
#pragma   comment(lib,   "ws2_32.lib ")
#endif

#ifdef WIN32
    typedef HANDLE HANDLE_t;
    typedef SOCKET SOCKET_t;
#else
    typedef int HANDLE_t;
    typedef int SOCKET_t;
#endif

#ifndef INVAILD_HANDLE_VALUE
#define INVAILD_HANDLE_VALUE    (-1)
#endif

#ifdef WIN32

#pragma  warning(disable:4267)
#pragma  warning(disable:4996)
#pragma	 warning(disable:4244)
#pragma warning(disable:4267)
#pragma warning(disable:4101)
#pragma warning(disable:4018)

typedef CRITICAL_SECTION    mutex_t;
#define mutex_lock(mutex)   EnterCriticalSection((mutex))
#define mutex_unlock(mutex)   LeaveCriticalSection((mutex))
#define mutex_init(mutex)       InitializeCriticalSection((mutex))
#define mutex_destroy(mutex)    DeleteCriticalSection((mutex))
typedef struct _stat    file_stat_t; 
#define file_stat(file,buff)    stat((file),(buff))
#define strncasecmp_my	strnicmp
#define snprintf_my _snprintf
#else
typedef pthread_mutex_t     mutex_t;
#define mutex_lock(mutex)   pthread_mutex_lock((mutex))
#define mutex_unlock(mutex)     pthread_mutex_unlock((mutex))
#define mutex_init(mutex)       pthread_mutex_init((mutex),NULL)
#define mutex_destroy(mutex)    pthread_mutex_destroy((mutex))
typedef struct stat     file_stat_t;
#define file_stat(file,buff)    stat((file),(buff))

#define strncasecmp_my	 strncasecmp
#define snprintf_my    snprintf
#endif
#include <errno.h>

#ifdef WIN32
static INLINE_t void  DBGPrintf(
                                const char *DbgFileName,
                                int DbgLineNo,
                                const char *FunctionName,
                                const char *FmtStr,
                                ...)
{
    va_list ArgList;
    FILE *fp = stdout;
    DbgFileName = DbgFileName;
    fprintf(fp, "(::%s), line %d",FunctionName, DbgLineNo);
    if (FmtStr) {
        fprintf(fp, ": ");
        va_start(ArgList, FmtStr);
        vfprintf(fp, FmtStr, ArgList);
        fflush(fp);
        va_end(ArgList);
    } else {
        fprintf(fp, "\n");
    }
}

#define dbg_print(fmt,...)  do{ if(debug) DBGPrintf(__FILE__,__LINE__,__FUNCTION__,fmt);}while(0)    

#else

#define dbg_print(format,arg...) do{ if(debug) printf("%s :%d # " format " \n",__FUNCTION__,__LINE__,##arg);}while(0)

#endif

void set_socket_noblock(SOCKET_t);
void dbg_error(const char *str);

#ifdef WIN32
int socketPipe(SOCKET_t fd[2]);
void my_closesocket( SOCKET_t s );
int my_read(SOCKET_t s,char *buff,int len);
int my_write(SOCKET_t s,const char *buff,size_t len);
int my_gettimeofday(struct timeval *tv, struct timezone *tz);
#else
#define socketPipe pipe
#define my_read  read
#define my_closesocket close
#define my_write    write
#endif


#ifdef _HY_DEBUG
#define HY_ASSERT(arg)  assert(arg)
#else
#define HY_ASSERT(arg)
#endif

#endif
