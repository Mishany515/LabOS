#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <time.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <string.h>

#define FILE "reader_prog.c"

typedef struct value {
    char str[20];
    pid_t pid;
} shared_mem_value;

int shmem_id;

int main() {
    key_t ipc = ftok(FILE, 1);
    if (ipc == -1) {
        perror("IPCkey error\n");
        exit(0);
    }

    shmem_id = shmget(ipc, sizeof(shared_mem_value), ipc | 0666);
    if (shmem_id == -1) {
        perror("shared memory error\n");
        exit(0);
    }

    shared_mem_value *received_value = (shared_mem_value*) shmat(shmem_id, NULL, 0);
    if (received_value == (shared_mem_value *)-1) {
        perror("shared memory link error\n");
        exit(0);
    }

    while(1) {
        time_t timer = time(0);
        shared_mem_value buff_value;
        strncpy((char *) buff_value.str, ctime(&timer), 19);

        buff_value.str[19] = '\0';
        buff_value.pid = getpid();

        printf("\ntime_Reader = %s\npid_Reader = %d\n", buff_value.str, buff_value.pid);
        printf("\ntime_Writer = %s\npid_Writer = %d\n", received_value->str, received_value->pid);
        sleep(1);
    }
    return 0;
}
