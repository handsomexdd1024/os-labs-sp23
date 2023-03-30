#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <assert.h>

int main(int argc, char** argv) {
    pid_t pid = 0;
    pid = fork();
    assert(pid >= 0);
    if (pid > 0) {
        pid = fork();
        if (pid > 0) {
            printf("aaaaaaaa");
            pid_t child_status;
            wait(&child_status);
            wait(&child_status);
            printf("\n");
        } else {
            printf("cccccccc");
        }
    } else {
        printf("bbbbbbbb");
    }
    return 0;
}
