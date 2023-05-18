//
// Created by xdd on 23-5-18.
//

#ifndef LAB04_TOY_PCB_H
#define LAB04_TOY_PCB_H

enum TASK_STATE {
    TASK_RUNNING,
    TASK_INTERRUPTIBLE,
    TASK_UNINTERRUPTIBLE,
    TASK_ZOMBIE,
    TASK_STOPPED
};

#define FINISHED 1
#define TIME_SLICE_LEN 2 //时间片长度

struct PCB {
    int pid;  //进程的pid
    TASK_STATE states;  //进程的状态
    int arrive_time;  //进入的时间
    int start_time;  //开始执行的时间
    int left_time;  //剩余的时间
    int end_time;  //最后结束的时间
    int serve_time;  //一共需要执行多长时间
    int priority;  //优先级
    int status;  //是否完成
    int turnaround_time;  //周转时间
};

#endif //LAB04_TOY_PCB_H
