#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <fcntl.h>

#define Share_size 512



char buffer[Share_size+1];  //buffer the stdin
int main(){
    key_t key=1111;  //key is the name of shared memory
    int shmid;
    char* shm;//pointer to share memory

    sem_t* mutex_read = sem_open("/mysem1", O_CREAT, 0666, 1);
    sem_t* mutex_write = sem_open("/mysem2", O_CREAT, 0666, 0);
    sem_t* sem = sem_open("/mysem3", O_CREAT, 0666, 1);
    //sem_init(&mutex_read,0,1);//initiate semaphore
    //sem_init(&mutex_write,0,0);
    //sem_init(&sem,0,1);  //mutex

    //create share memory
    if((shmid=shmget(key,Share_size,IPC_CREAT|0666))<0){  //0666 means can read and write
        perror("shmget");
        exit(1);
    }
    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1){  //enable the share memory space
        perror("shmat");
        exit(1);
    }
    memset(shm,0,Share_size);
    //struct msg *ptr=(struct msg *)shm;

    int pid =fork();
    if(pid>0){  //process A:read
        while(1){
            sem_wait(mutex_write);   //use a semaphore to prevent parallal read
            sem_wait(sem);   //wait for somthing to read
            printf("Read: %s\n",shm);
            fflush(stdout);
            sem_post(sem);
            sem_post(mutex_read);   //unlock the mutex
            if(strncmp(shm,"quit",4)==0)break;
            
        }
    }
    else if(pid==0){  //process B:write
        while(1){
            sem_wait(mutex_read);  //use a semaphore to prevent parallal write
            sem_wait(sem);
            printf("Enter your message here:");
            fgets(buffer,Share_size,stdin);   //stdin to get message
            memset(shm,0,Share_size);  //clean the space(initiate)
            strncpy(shm,buffer,Share_size);
            
            sem_post(sem);
            sem_post(mutex_write);  //unlock other write
              
            if(strncmp(buffer,"quit",4)==0)break;
        }
    }

    //delete semaphore
    sem_close(mutex_read);
    sem_close(mutex_write);
    sem_close(sem);
    sem_unlink("/mysem1");
    sem_unlink("/mysem2");
    sem_unlink("/mysem3");

    if(shmdt(shm)==-1){
        perror("shmdt");
        exit(1);    
    }; //drop from sharespace
    if(shmctl(shmid,IPC_RMID,0)==-1){
        perror("shmctl");
        exit(1);
    }
    exit(1);
}