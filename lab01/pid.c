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
    } else {
        printf("bbbbbbbb");
    }
    if(pid > 0) {
        printf("aaaaaaaa");
        pid_t child_status;
        wait(&child_status);
        wait(&child_status);
        printf("\n");
    } else {
        printf("cccccccc");
    }
    return 0;
}
