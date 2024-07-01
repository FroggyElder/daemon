#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include "daemon.h"

#define FIFO_PATH "/tmp/svfifo"
#define LOG_PATH "/home/cy/C Workspace/date/6.25/homework/log.txt"

void setNoBlock (int fd) {
    int flg = fcntl(fd,F_GETFL);
    flg |= O_NONBLOCK;
    fcntl(fd,flg);
}

int main () {
    toDaemon();

    if(access(FIFO_PATH,F_OK)) {
        if(mkfifo(FIFO_PATH,0777)) {
            perror("mkfifo error");
            return -1;
        }
    }

    int log_fd = open(LOG_PATH,O_WRONLY|O_CREAT,0777);
    lseek(log_fd,0,SEEK_END);

    char read_str[128] = {0};
    int fifo_fd = open(FIFO_PATH,O_RDWR);

    while (read(fifo_fd,read_str,sizeof(read_str))==0);
    while(1) {
        int res = read(fifo_fd,read_str,sizeof(read_str));
        if(res==0) {
            printf ("Read complete\n");
            break;
        }
        else if (res<0) {
            perror ("Read");
            break;
        }
        else write(log_fd,read_str,strlen(read_str));

        usleep(1000*100);
    }

    close(fifo_fd);
    close(log_fd);
    remove(FIFO_PATH);
}