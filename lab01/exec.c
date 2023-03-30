#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <assert.h>

int main(int argc, char **argv) {
    pid_t pid, status;
    pid = fork();
    assert(pid >= 0);
    if (pid == 0) {
        printf("Into sub-process\n");
        execlp("neofetch", "");
    } else {
        printf("Into master process");
        wait(&status);
    }
    return 0;
}