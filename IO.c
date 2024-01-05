#include "IO.h"
#include "tls.h"
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <grp.h>
#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>


#define MAXLINE 1024



long Readn(int fd, void *buf, int n){
    ssize_t nleft;
    ssize_t nread;
    char* ptr;

    ptr = buf;
    nleft = n;

    while(nleft > 0) {
        nread = read(fd, ptr, nleft);
        if (nread == -1)
            return -1;
        else if (nread == 0)
            break;
        nleft -= nread;
        ptr += nread;
    }
    return (n-nleft);
}


long Writen(int fd, const void *buf, int n){
    long bytewrite;
    int byte2write = n;
    const char *ptr = NULL;

    for(byte2write = n, ptr = buf; \
        byte2write > 0; \
        byte2write -= bytewrite, ptr += bytewrite){

        if((bytewrite = write(fd, ptr, byte2write)) < 0){
            if(EINTR == errno || EWOULDBLOCK == errno || EAGAIN == errno){
                bytewrite = 0;
            }else {
                perror("Writen error:");
                return -1;
            }
        }else if(bytewrite == 0){
            printf("Writen socket closed\n");
            break;
        }
    }

    return byte2write ? -1 : 0;
}



long Read2F(int fd, FILE *f){
    char buff[BUFSIZ];
    fd_set rs;
    int trueblocking = 1;
    int err;
    int fileerror = 0;
    do{
        if(trueblocking){
            FD_ZERO(&rs);
            FD_SET(fd, &rs);
            select(fd+1, &rs, NULL, NULL, NULL);
        }
        err = read(fd, buff, sizeof(buff));
        if(err < 0){
            if(EINTR == errno){
                trueblocking = 0;
                continue;
            }else if(EWOULDBLOCK == errno){
                trueblocking = 1;
                continue;
            }else{
                return -1;
            }
        }else if(err == 0){

            if(fileerror)return -2;
            return 0;
        }
        trueblocking = 0;
        fwrite(buff, sizeof(char), err, f);
        if(ferror(f)){
            perror("RETR fwrite error:");
            fileerror = 1;
        }
    }while(1);

}
long SSL_Read2F(int fd,FILE *f, unsigned char *additional, unsigned char *key, unsigned char *iv, int type){
    char buff[BUFSIZ];
    fd_set rs;
    int trueblocking = 1;
    long err;
    int fileerror = 0;
    do{
        if(trueblocking){
            FD_ZERO(&rs);
            FD_SET(fd, &rs);
            select(fd+1, &rs, NULL, NULL, NULL);
        }
        err =  SSL_receive(fd,buff, additional, key, iv,type);
        buff[strlen(buff)] = '\0';
        if(err < 0){
            if(EINTR == errno){
                trueblocking = 0;
                continue;
            }else if(EWOULDBLOCK == errno){
                trueblocking = 1;
                continue;
            }else{
                return -1;
            }
        }else if(err == 0){
            if(fileerror)return -2;
            return 0;
        }
        trueblocking = 0;
        fwrite(buff, sizeof(char), err, f);
        if(ferror(f)){
            perror("RETR fwrite error:");
            fileerror = 1;
        }
    }while(1);

//    SSL_receive(fd,buff, additional, key, iv,type);
//    //buff[strlen(buff)] = '\0';
//
//    fwrite(buff, sizeof(char), strlen(buff), f);
//    if(ferror(f)){
//            perror("RETR fwrite error:");
//            fileerror = 1;
//    }
//    return 0;


}




/*file and directory*/
int checkdir(const char *path){
    DIR *dir;
    if(NULL == (dir = opendir(path)))return 0;
    closedir(dir);
    return 1;
}



int get_file_info(const char *filename, char buff[], int len)
{
    char	timebuf[MAXLINE];
    int		timelen, off = 0;

    struct tm *ptm;
    struct stat st;

    if (-1 == stat(filename, &st)) {
        perror("stat error:");
        return -1;
    }
    off += snprintf(buff + off, len - off, " %*d KB", 8,
            (int) st.st_size);

    ptm = localtime(&st.st_mtime);
    if (ptm != NULL
    && (timelen = strftime(timebuf, sizeof(timebuf), " %b %d %H:%S", ptm)) > 0) {
        timebuf[timelen] = '\0';
        off += snprintf(buff + off, len - off, "%s", timebuf);
    } else {
        perror("localtime error:");
        return -1;
    }
    off += snprintf(buff + off, len - off, " %s\r\n", filename);
    return off;
}

char *Getcwd(char *buf, int size)
{
    char	*ptr;
    if (NULL == (ptr = getcwd(buf, size)))
        perror("getcwd error");
    return ptr;
}


void Chdir(const char *path)
{
    if (-1 == chdir(path))
        perror("Chdir error");
}

DIR *Opendir(const char *dirname)
{
    DIR		*dirp;

    if (NULL == (dirp = opendir(dirname)))
        perror("Opendir error");
    return dirp;
}
