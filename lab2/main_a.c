#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>

int main() {
    int fd[2]; 

    if (pipe(fd) == -1) {
        perror("pipe open with erro");
        exit(0);
    }
    
    char str[127];
    pid_t pid = fork();


    if (pid == -1){
        perror("fork err");
        exit(0); 
    }
    else if (pid == 0){
        close(fd[1]); 
        sleep(5);

        if((read(fd[0], str, 127)) == -1) {
            perror("read pipe error");
            exit(0);
        }

        close(fd[0]);
        time_t timer = time(0);
       
        printf("\tchild time:\t%s%s\n", ctime(&timer), str);
        exit(0);
    }
    else{
        close(fd[0]);
        time_t p_timer = time(0);
        sleep(5);
        
        sprintf(str, "%s %s %s %d", "\tparent time: ", ctime(&p_timer), "\tparent pid = ", getpid());
        write(fd[1], str, 127);
        close(fd[1]);

        if (waitpid(pid, 0, 0) == -1) {
            perror("wait pid");
            exit(0);
        }
    }
    return 0;
}