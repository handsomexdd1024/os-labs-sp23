#include <stdio.h>
#include <pthread.h>

struct demo {
    int num;
    pthread_mutex_t lock;
};
void write(struct demo *num) {

}
void read(struct demo *num) {

}
int main() {
    struct demo my_num;
    pthread_t producer, consumer;
}
