#ifndef _CONVERT_H
#define _CONVERT_H
int UnicodeToUtf8(char* pInput, char *pOutput);
int Utf8ToUnicode(char* pInput, char* pOutput);
int GbkToUnicode(char *gbk_buf, unsigned short *unicode_buf, int max_unicode_buf_size);
int UnicodeToGbk(unsigned short *unicode_buf, char *gbk_buf, int max_gbk_buf_size);
#endif
