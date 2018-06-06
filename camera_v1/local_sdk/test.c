#include <stdio.h>  
#include <string.h>  
#include <stdlib.h>  
#include <iconv.h>  
  
#define BUFSIZE 1024  
  
int utf2gbk(char *buf, size_t len)  
{  
    iconv_t cd = libiconv_open("GBK", "UTF-8");  
    if (cd == (iconv_t)-1) {  
        perror("获取字符转换描述符失败！\n");  
        return -1;  
    }  
    printf("%D\n",(int)cd);
    size_t sz = BUFSIZE * BUFSIZE;  
    char *tmp_str = (char *)malloc(sz);  
    // 不要将原始的指针传进去，那样会改变原始指针的  
    size_t inlen = len;  
    size_t outlen = sz;  
    char *in = buf;  
    char *out = tmp_str;  
    if (tmp_str == NULL) {  
        libiconv_close(cd);  
        fprintf(stderr, "分配内存失败！\n");  
        return -1;  
    }  
    memset(tmp_str, 0, sz);  
    if (libiconv(cd, &in, &inlen, &out, &outlen) == (size_t)-1) {  
        libiconv_close(cd);  
        return -1;  
    }  
    libconv_close(cd);  
    return 0;  
}  
  
int main(int argc, char *argv[])  
{  
    char in[BUFSIZE] = "nihao1nih年后";  
    int len = strlen(in);  
    printf("%s\n", in);  
    utf2gbk(in, len);  
    printf("%s\n", in);  
    return 0;  
} 
