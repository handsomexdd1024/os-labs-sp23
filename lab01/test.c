#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <assert.h>

int main(int argc, char **argv) {
    pid_t pid = 4;
    pid = fork();
    assert(pid >= 0);
    if (pid == 0) {
        printf("Son %d %d\n", pid, getpid());
    } else {
        printf("Father %d %d\n", pid, getpid());
    }
    return 0;
}