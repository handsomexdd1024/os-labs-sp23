#include <sys/file.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

void create_open_file();
void write_file();
void read_file();
void close_file();
void change_file_mod();
void check_file_mod();
void check_file_status();

int fd; //file descriptor
char filenames[100];
char *argv[4]={"ls","-l",NULL,NULL};
int openstate=0;

int main(){
    printf("-------------------------filetools-------------------------\n");
    printf("0.quit   1.create(if exist then open   2.write file\n");
    printf("3.read file  4.change file mode  5.check file mode \n");
    printf("6.check file status 7.close file\n");
    printf("-----------------------------------------------------------\n");
    int option;
    while(1){
        printf("------------------enter your option------------------\n");
        scanf("%d",&option);
        switch(option){
            case 0:{
                printf("quiting\n");
                return 0;
            }
            case 1:{
                create_open_file();
                break;
            }
            case 2:{
                write_file();
                break;
            }
            case 3:{
                read_file();
                break;
            }
            case 4:{
                change_file_mod();
                break;
            }
            case 5:{
                int p=fork();
                if(p) wait(NULL);
                else check_file_mod();
                break;
            }
            case 6: {
                check_file_status();
                break;
            }
            case 7:{
                close_file();
                break;
            }
            default:
                printf("option wrong,enter again\n");
        }
    }
}

void create_open_file(){
    printf("enter the name of the file you want to create/open\n");
    scanf("%s",filenames);
    fd = open(filenames,O_RDWR,0666);
    if(fd>=0){
        printf("%s exists, successfully opened file\n",filenames);
        printf("file descriptor: %d\n",fd);
        argv[2]=(char*)malloc(50);
        strcpy(argv[2],filenames);
        openstate=1;
    }
    else{
        fd=open(filenames,O_CREAT|O_RDWR,0666);
        if(fd<0){
            printf("create failed\n");
            exit(0);
        }
        else {
            printf("%s create success\n",filenames);
            printf("file descriptor: %d\n",fd);
            openstate=1;
        }
    }
}

void write_file(){
    if(!openstate){
        printf("no open file\n");
        return;
    }
    else printf("start to write file:%s\n",filenames);
    char buff[1024];
    scanf("%s",buff);
    ssize_t status= write(fd,buff,strlen(buff)+1);
    fflush(fdopen(fd,"w"));
    sync();
    if(status==-1)printf("write failed\n");
    else printf("write success\n");
}

void read_file(){
    if(!openstate){
        printf("no open file\n");
        return;
    }
    else printf("start to read file:%s\n",filenames);
    char buff[1024];
    lseek(fd, 0, SEEK_SET);
    ssize_t status=read(fd,buff,1024);
    if(status==-1)printf("rbufferead failed\n");
    else {
        printf("successfully read %ld bytes\n", status);
        printf("content of file:\n");
        printf("%s\n",buff);
    }
}

void change_file_mod(){
    if(!openstate){
        printf("no open file\n");
        return;
    }
    else printf("start to change file mod:%s\n",filenames);
    printf("enter the mod you want:\n");
    printf("rwx for 421 and 3bit for owner, group and others\n");
    int mods;
    scanf("%d",&mods);
    int owner=mods/100;
    int group=mods/10%10;
    int others=mods%10;
    int newmods=owner*8*8 + group*8 + others;
    int status=chmod(filenames,newmods);
    if(status==-1)printf("change mod failed\n");
    else {
        printf("change mod success\n");
        printf("new mod:%d\n",mods);
    }
}

void check_file_mod(){
    if(!openstate){
        printf("no open file\n");
        return;
    }
    else printf("start to check file mod:%s\n",filenames);
    printf("the permission of file is:\n");
    int status=execv("/bin/ls",argv);
    if(status==-1)printf("check failed\n");
}

void check_file_status() {
    if(!openstate){
        printf("no open file\n");
        return;
    }
    else printf("start to check file mod:%s\n",filenames);
    struct stat buf;
    int status=stat(filenames,&buf);
    if(status==-1)printf("check failed\n");
    else {
        printf("check success\n");
        printf("file status:\n");
        printf("owner: %d\n",buf.st_uid);
        printf("group: %d\n",buf.st_gid);
        printf("size: %ld\n",buf.st_size);
        printf("owner permission: %d\n",buf.st_mode/8/8%8);
        printf("group permission: %d\n",buf.st_mode/8%8);
        printf("others: %d\n",buf.st_mode%8);
    }
}

void close_file(){
    if(!openstate){
        printf("no file has open, please open first\n");
        return;
    }
    else printf("start to close file:%s\n",filenames);
    if(close(fd)<0)
        printf("close failed\n");
    else{
        printf("%s has been closed\n",filenames);
        openstate=0;
    }
}