#ifndef IOCONTROL_H
#define IOCONTROL_H

#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>


long Read(int fd, void *buf, int count);

//从文件描述符中读取n bytes 到buf中
long Readn(int fd, void *buf, int n);


long Write(int fd, const void *buf, int count);

//缓存区中读取n bytes 到fd中
long Writen(int fd, const void *buf, int n);

long Read2F(int fd, FILE *f);
long SSL_Read2F(int sock,FILE *f,unsigned char *additional, unsigned char *key, unsigned char *iv, int type);

int checkdir(const char *path);
int get_file_info(const char *filename, char buff[], int len);
char *Getcwd(char *buf, int size);
void Chdir(const char *path);
DIR *Opendir(const char *dirname);

#endif // IOCONTROL_H
