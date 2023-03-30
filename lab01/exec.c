#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <assert.h>

int main(int argc, char **argv) {
    pid_t pid;
    int status;
    pid = fork();
    assert(pid >= 0);
    if (pid == 0) {
        printf("Into sub-process\n");
        execlp("neofetch", "");
        printf("Execution failed\n");
        return 1;
    } else {
        printf("Into master process\n");
        wait(&status);
        printf("Sub-process returned %d\n", status);
    }
    return 0;
}