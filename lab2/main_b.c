#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

int main() {
    unlink("fifo");

    if (mkfifo("fifo", 0666) == -1) {
        perror("fifo created with err.");
        exit(0);
    }

    pid_t pid = fork();
    char str[127];
    if(pid == -1){
        perror("Fork failure");
        exit(0);
    }
    else if (pid == 0)
    {
        int fd_fifo = open("fifo", O_RDONLY);

        if (fd_fifo == -1) {
            perror("fifo open with err\n");
            exit(0);
        }

           
        if ((read(fd_fifo, str, 127)) == -1) {
            perror("read fifo err\n");     
            exit(0);
        }

        close(fd_fifo);

        time_t timer = time(0);            
        printf("\tchild time:\t%s%s\n", ctime(&timer), str);

        exit(0);

    }
    else{
        int fd_fifo = open("fifo", O_WRONLY);
        if (fd_fifo == -1) {
            perror("fifo open with err.\n");
            exit(0);
        }

        time_t p_timer = time(0);
        sleep(5);

        sprintf(str, "%s %s %s %d", "\tparent time: ", ctime(&p_timer), "\tparent pid = ", getpid());
        write(fd_fifo, str, 100);
        close(fd_fifo);

        if (waitpid(pid, 0, 0) == -1) {
            perror("wait pid");
            exit(0);
        }
    }


    return 0;
}