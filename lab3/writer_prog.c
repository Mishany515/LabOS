#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <time.h>
#include <sys/shm.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>

#define FILE "reader_prog.c"

int shm_id;

typedef struct value {
    char str[20];
    pid_t pid;
} shared_mem_value;


void funcExit(int sig) {
    struct shmid_ds *buf = 0;
    shmctl(shm_id, IPC_RMID, buf);
    exit(0);
}

int main() {
    signal(SIGINT, funcExit);
    key_t ipc = ftok(FILE, 1);

    shm_id = (shmget(ipc, sizeof(shared_mem_value), IPC_CREAT | 0666));

    if (shm_id == -1) {
        printf("Can't create shared memory\n");
        exit(0);
    }

    shared_mem_value *shared_value; 

    shared_value = (shared_mem_value *) shmat(shm_id, NULL, 0);
    if (shared_value == (shared_mem_value *)-1) {
        printf("error\n");
        exit(0);
    }

    time_t timer = time(0);

    if (shared_value->pid != 0) {
        printf("Sender has already been started.\n");
        exit(0);
    }

    while(1) {
        timer = time(0);

    	shared_mem_value tmp_value;
    	strncpy((char*) tmp_value.str, ctime(&timer), 19);

    	tmp_value.str[19] = '\0';
    	tmp_value.pid = getpid();

    	*shared_value = tmp_value;
    }
    return 0;
}