#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>

#define FIFO_PATH "/tmp/svfifo"

int childProcess (int fifo_fd) {
    char time_buffer[128];

    time_t c_time = time(NULL);
    struct tm* ltm = localtime(&c_time);
    strftime(time_buffer,sizeof(time_buffer),"[%d-%m-%Y]:[%H-%M-%S]\n",ltm);

    write(fifo_fd,time_buffer,strlen(time_buffer)+1);

    return 0;
}

int newFifo (char* filepath) {
    if (access(filepath,F_OK)) {
        if (mkfifo(filepath,0666)) {
            perror ("failed to mkfifo");
            return -1;
        }
    }

    int fifo_fd = open(filepath,O_RDWR);

    return fifo_fd;
}

int main () {

    int fifo_fd = newFifo(FIFO_PATH);

    while (1) {
        int pid = fork();
        if (pid<0) {
            perror("Failed to fork\n");
            return -1;
        }
        if (pid==0) return childProcess(fifo_fd);

        sleep(1);
    }

    return 0;
}