#ifndef _WRITE_AT_READ_RESULT_H_
#define _WRITE_AT_READ_RESULT_H_
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <getopt.h>

typedef struct arg{
int fd;
int cmd_number;
pthread_mutex_t *mutex1;
pthread_mutex_t *mutex2;
}Pthread_Arg;

int set_parity(int fd,int databits,int stopbits,int parity);
void set_speed(int fd,int speed);
void send_at(int fd,char *cmd, int send_len);
void *read_thread(Pthread_Arg *arg);
int at_cmd(int fd, char *at_cmd);

#endif
