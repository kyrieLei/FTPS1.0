//
// Created by kyrie on 2023/12/30.
//

#include <sys/stat.h>
#include "path_operation.h"
#include "communication.h"
#include "../IO.h"

int LIST(FTP *ftp){
    int		n;
    char	buff[BUFSIZ];
    char	tmp_dir_path[PATH_MAX];


    if(ftp->AUTHMODE){
        pasvconn(ftp);
//        if(ftp->socket_pasv_conn <= 0){
//            printf("LIST: TLS not established\n");
//            ftp->sendresponse(ftp);
//            sprintf(ftp->response,
//                    "550 Failed on AUTH.\r\n");
//            closesocket(ftp);
//            ftp->sendresponse(ftp);
//            return -1;
//        }
    }else{
        pasvconn(ftp);
    }

    strcpy(ftp->response, "150 Here comes the directory listing.\r\n");
    sendresponse(ftp);


    int havearg = strlen(ftp->arg);
    if(havearg){
        if (!checkdir(ftp->arg)) {
            struct stat	st;
            if (-1 == stat(ftp->arg, &st) && ENOENT == errno) {
                /* file not exists */
                printf("file %s doesn't exsit\n", ftp->arg);
                goto LISTERR;

            }
            goto LISTOK;

        }

        Getcwd(tmp_dir_path, sizeof(tmp_dir_path));
        Chdir(ftp->arg);
    }


    {
        DIR		*dir;
        struct dirent *dent;
        int		off = 0;
        char	*filename;

        char *buf;
        int fd;

        dir = Opendir(".");
        buff[0] = '\0';
        while ((dent = readdir(dir)) != NULL) {
            filename = dent->d_name;
            if ('.' == filename[0])
                continue;
            off = get_file_info(filename, buff , sizeof(buff));
            if(ftp->AUTHMODE){
                //Writen(ftp->socket_pasv_conn, buff, off);
                SSL_send(ftp->socket_pasv_conn, buff, additional, key, iv);
            }else{
                Writen(ftp->socket_pasv_conn, buff, off);
            }
        }

    }

    if (havearg){
        Chdir(tmp_dir_path);
    }
    LISTOK:
    strcpy(ftp->response, "226 Directory send OK.\r\n");
    closesocket(ftp);
    ftp->sendresponse(ftp);
    return 0;
    LISTERR:
    snprintf(ftp->response,1024,
            "550 File \"%s\" specified not exists.\r\n", ftp->arg);
    closesocket(ftp);
    ftp->sendresponse(ftp);
    return -1;

}

int CWD(FTP *ftp){
    char	cur_dir[PATH_MAX];


    if (strlen(ftp->arg) < 1) {
        strcpy(ftp->response, "550 Missing dest dir-path.\r\n");
        ftp->sendresponse(ftp);
        return -1;
    }

    if (-1 == chdir(ftp->arg)) {
        strcpy(ftp->response, "550 Invalid dest dir-path.\r\n");
        ftp->sendresponse(ftp);
        return -1;
    }

    if(NULL == Getcwd(cur_dir, PATH_MAX)){
        strcpy(ftp->response, "550 can't access to dest diretory.\r\n");
        ftp->sendresponse(ftp);
        return -1;
    }
    strcpy(ftp->response, "250 Directory successfully changed.\r\n");
    ftp->sendresponse(ftp);
    return 0;
}

int PWD(FTP *ftp){
    //check login missing;
    Getcwd(ftp->cur_dir, sizeof(ftp->cur_dir));
    sprintf(ftp->response,"257 %s\r\n", ftp->cur_dir);

    ftp->sendresponse(ftp);

    return 0;
}