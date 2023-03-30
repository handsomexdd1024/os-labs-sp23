#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>

int que_id;

int main() {
    pid_t pid;
    que_id = msgget(IPC_PRIVATE, IPC_CREAT);
    pid = fork();
    if(pid == 0) {
        exec
    }
}
